/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERElossCalculatorUtils.h"

#include <iostream>
#include "signal.h"

#include "G4EmCalculator.hh"
#include "G4NistManager.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"

#include "TObjArray.h"
#include "TG4RunConfiguration.h"
#include "TGeant4.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairLogger.h"
#include "FairEventHeader.h"
#include "FairTask.h"
#include "FairTrajFilter.h"

#include "FairRuntimeDb.h"
#include "FairParRootFileIo.h"

#include "ERRecoMCApplication.h"

using namespace std;

ERElossCalculatorUtils* ERElossCalculatorUtils::fInstance = NULL;
//--------------------------------------------------------------------------------------------------
ERElossCalculatorUtils* ERElossCalculatorUtils::Instance() {
  if (!fInstance) {
    fInstance = new ERElossCalculatorUtils();
  }
  return fInstance;
}
//--------------------------------------------------------------------------------------------------
ERElossCalculatorUtils::ERElossCalculatorUtils()
: FairRunAna()
{
}
//--------------------------------------------------------------------------------------------------
ERElossCalculatorUtils::~ERElossCalculatorUtils() {
  fGeant4.reset();
}
//--------------------------------------------------------------------------------------------------
void ERElossCalculatorUtils::Init(){ 
  
  FairGeoLoader* loader=new FairGeoLoader("TGeo", "Geo Loader");
  FairGeoInterface* GeoInterFace=loader->getGeoInterface();
  GeoInterFace->SetNoOfSets(0);
  TString erPath = gSystem->Getenv("VMCWORKDIR");
  GeoInterFace->setMediaFile(erPath+"/geometry/media.geo");
  GeoInterFace->readMedia();
  ERRecoMCApplication* fApp= new ERRecoMCApplication("Fair","The Fair VMC App",new TObjArray(), erPath+"/geometry/media.geo");

  TString input_path = erPath + "/input/rootfiles/";
  TString fake_in = input_path + "sim_digi_Calib.root";
  TString fake_par = input_path + "par_Calib.root";
  // TString fake_out = "/home/komyour/soft/expertroot_clone/macro/EXP1904_H7/sim/sim_digi.root";
  // auto file = TFile::Open("fakein.root", "RECREATE");
  // file->Close();
  this->SetInputFile(fake_in);
  // Fake initialization of runtime db
  FairRuntimeDb* rtdb = this->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(fake_par, "UPDATE");
  rtdb->setFirstInput(parInput);
  this->SetOutputFile("fakeout.root");
  FairRunAna::Init();
  TG4RunConfiguration* runConfiguration
              // = new TG4RunConfiguration("geomRoot", "emStandard", "specialCuts+stackPopper");
              = new TG4RunConfiguration("geomRoot", "emStandard", "specialCuts+stackPopper");
  fGeant4 = std::make_shared<TGeant4>("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  fGeant4->ProcessGeantMacro(erPath+"/gconfig/g4config.in");
  fGeant4->Init();
  fGeant4->ProcessRun(0);
  // This line needed to write full Geant4 initialization DEBUG to console
  // before printing other's code outputs
  G4cout << "======================================================" << G4endl;
  G4cout << "[ERElossCalculatorUtils::Init()] Geant4 initialized" << G4endl;
  G4cout << "======================================================" << G4endl;
}

float ERElossCalculatorUtils::IntegralEloss(
  const unsigned pid, const float kinetic_enegry /*[MeV]*/, const TString& material, 
  const float thickness /*[um]*/, const float number_of_steps, const bool if_step_back = false) {
  if (thickness <= 0.)
    return 0;
  float current_enegry = kinetic_enegry;
  G4IonTable* iot_table = G4IonTable::GetIonTable();
  G4ParticleDefinition* particle_g4 = iot_table->GetIon(pid);
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* material_g4 = nist->FindOrBuildMaterial(material.Data());

  const float direction = if_step_back ? -1.0 : 1.0;
  float integral_eloss = 0.;
  const float intStep = thickness / number_of_steps;
  float curStep = 0.;
  auto* calc = new G4EmCalculator();
  while (!AreSame(curStep, thickness, intStep / 2.)) {
    const float eloss = direction * calc->GetDEDX(current_enegry, particle_g4, material_g4) 
                      * intStep * 1e-3 ; // MeV
    integral_eloss += eloss;
    current_enegry -= eloss;
    curStep += intStep;
    // LOG(DEBUG) << "[calculator] integral_eloss " << integral_eloss << FairLogger::endl;
    // LOG(DEBUG) << "[calculator] kinetic_enegry " << kinetic_enegry << FairLogger::endl;
    // LOG(DEBUG) << "[calculator] curStep " << curStep << FairLogger::endl;
  }
  // TODO: add 'absolutely' accurate calculation of the integral
  // if (curStep < range) {
  // }
  // if (curStep > range) {
  // }
  return TMath::Abs(integral_eloss);
}

float ERElossCalculatorUtils::BackAndForthPropagation(
  const unsigned pid = 1000020040, const float energy = 7.6869 /*MeV*/, 
  const TString& material = "silicon", const float thickness = 40 /*[um]*/,
  const float number_of_steps = 1000.0) {
  LOG(DEBUG) << "--- Direct and inverse propagation info ---" << FairLogger::endl;
  LOG(DEBUG) << "thickness " << thickness  << " [um]" << FairLogger::endl;
  LOG(DEBUG) << "pid " << pid << FairLogger::endl;
  LOG(DEBUG) << "material " << material <<  FairLogger::endl;
  const float eloss_direct = IntegralEloss(
     pid, energy, material, thickness, number_of_steps, false
  ); 
  LOG(DEBUG) << eloss_direct << " [MeV]" << " - eloss_direct " << FairLogger::endl;
  const float energy_after_direct_propagation = energy - eloss_direct;
  LOG(DEBUG) <<  energy_after_direct_propagation << " [MeV]" 
             << " - energy_after_direct_propagation " << FairLogger::endl;
  const float eloss_back = IntegralEloss(
    pid, energy_after_direct_propagation, material, thickness, number_of_steps, true
  );
  const float direct_back_eloss_diff = eloss_direct - eloss_back;
  LOG(DEBUG) <<  eloss_back << " [MeV]" << " - eloss_back " << FairLogger::endl;
  LOG(DEBUG) <<  direct_back_eloss_diff << " [MeV]" << " - eloss_direct - eloss_back " << FairLogger::endl;
  LOG(DEBUG) << "-------------------------------------------" << FairLogger::endl;
  return direct_back_eloss_diff;
}

ClassImp(ERElossCalculatorUtils)

