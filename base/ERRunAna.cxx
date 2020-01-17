/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERRunAna.h"

#include <iostream>
#include "signal.h"

#include "TObjArray.h"
#include "TG4RunConfiguration.h"
#include "TGeant4.h"

#include "G4EmCalculator.hh"
#include "G4NistManager.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairLogger.h"
#include "FairEventHeader.h"
#include "FairTask.h"
#include "FairTrajFilter.h"

#include "ERRecoMCApplication.h"

using namespace std;

ERRunAna* ERRunAna::fInstance = NULL;
TCut      ERRunAna::fUserCut = "";
TH1I*     ERRunAna::fEventsForProcessing = NULL;
Bool_t gFRAIsInterrupted;
//--------------------------------------------------------------------------------------------------
ERRunAna* ERRunAna::Instance() {
  if (!fInstance) {
    fInstance = new ERRunAna();
  }
  return fInstance;
}
//--------------------------------------------------------------------------------------------------
ERRunAna::ERRunAna()
: FairRunAna(),
fHoldEventsCount(kFALSE)
{
  LOG(DEBUG) << "ERRunAna constructor" << FairLogger::endl;
}

double fDivision = 1000;

bool AreSame(double a, double b, double eps)
{
    // cout << "a " << a << " b " << b << " eps " << eps << endl;
    return fabs(a - b) < eps;
}
//--------------------------------------------------------------------------------------------------
Double_t CalcElossIntegralVolStepGet (Double_t T, const G4ParticleDefinition* ion, 
                                      const G4Material* mat, const Double_t range) { 
  //FIXME copy-past from ERBeamDetSetup
  assert(mat);
  assert(ion);
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = 0.01; // range / fDivision;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (!AreSame(curStep, range, intStep / 2.)) {
    Double_t eloss = calc->GetDEDX(T,ion,mat)*intStep*1e-4*10;
    integralEloss += eloss;
    T += eloss;
    curStep += intStep;
  }
  return integralEloss;
}//--------------------------------------------------------------------------------------------------
Double_t CalcElossIntegralVolStepCompute (Double_t T, G4ParticleDefinition* ion, 
                                          G4Material* mat, Double_t range) 
{ 
  //FIXME copy-past from ERBeamDetSetup
  assert(mat);
  assert(ion);
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = 0.01; // range / fDivision;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (!AreSame(curStep, range, intStep / 2.)) {
    Double_t eloss = calc->ComputeDEDX(T,ion,"ionIoni",mat)*intStep*1e-4*10;
    integralEloss += eloss;
    T += eloss;
    curStep += intStep;
  }
  // cout << "range " << range << " | curStep " << curStep << endl;
  return integralEloss;
}
//--------------------------------------------------------------------------------------------------
Double_t CalcElossIntegralVolStepGetInvDir (Double_t T, const G4ParticleDefinition* ion, 
                                      const G4Material* mat, const Double_t range) { 
  //FIXME copy-past from ERBeamDetSetup
  assert(mat);
  assert(ion);
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = 0.01; // range / fDivision;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (!AreSame(curStep, range, intStep / 2.)) {
    Double_t eloss = calc->GetDEDX(T,ion,mat)*intStep*1e-4*10;
    integralEloss += eloss;
    T -= eloss;
    curStep += intStep;
  }
  return integralEloss;
}//--------------------------------------------------------------------------------------------------
Double_t CalcElossIntegralVolStepComputeInvDir (Double_t T, G4ParticleDefinition* ion, 
                                          G4Material* mat, Double_t range) 
{ 
  //FIXME copy-past from ERBeamDetSetup
  assert(mat);
  assert(ion);
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = 0.01; // range / fDivision;
  // cout << "intStep " << intStep << endl;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (!AreSame(curStep, range, intStep / 2.)) {
    Double_t eloss = calc->ComputeDEDX(T,ion,"ionIoni",mat)*intStep*1e-4*10;
    integralEloss += eloss;
    T -= eloss;
    curStep += intStep;
  }
  // cout << "range " << range << " | curStep " << curStep << endl;
  return integralEloss;
}
//--------------------------------------------------------------------------------------------------
void ERRunAna::Init(){ 
  
  FairGeoLoader* loader=new FairGeoLoader("TGeo", "Geo Loader");
  FairGeoInterface* GeoInterFace=loader->getGeoInterface();
  GeoInterFace->SetNoOfSets(0);
  TString erPath = gSystem->Getenv("VMCWORKDIR");
  GeoInterFace->setMediaFile(erPath+"/geometry/media.geo");
  GeoInterFace->readMedia();

  ERRecoMCApplication* fApp= new ERRecoMCApplication("Fair","The Fair VMC App",new TObjArray(), erPath+"/geometry/media.geo");

  FairRunAna::Init();
  // initialisation of FairRootManager for getting tree to implement
  // user cuts for input data
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  if (fUserCut != "") {
    LOG(INFO) << "User cut " << fUserCut << " implementation" << FairLogger::endl;
    TTree* tree = ioman->GetInTree();
    fEventsForProcessing =  new TH1I ("hist", "Events for processing", tree->GetEntries(), 1, tree->GetEntries());
    tree->Draw("Entry$>>hist",fUserCut,"goff");
    if (!fEventsForProcessing->GetEntries()) {
      LOG(FATAL) << "ERRunAna: No data for analysis with defined user cut: "
                << fUserCut << FairLogger::endl;
      return;
    }
  }
  
  TG4RunConfiguration* runConfiguration
             = new TG4RunConfiguration("geomRoot", "QGSP_BERT_HP", "specialCuts+stackPopper");

  TGeant4* geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  geant4->ProcessGeantMacro(erPath+"/gconfig/g4config.in");
  geant4->Init();
  geant4->ProcessRun(0);


  cout << "ERRunAna integration " << endl; 
  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(1000020040);
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* mat = nist->FindOrBuildMaterial("silicon");
  double range = 1e-4;
  double T = 7.68690;
  std::vector<double> energies = {4.7844, 6.0024, 7.6869}; // [MeVs]
  // std::vector<double> energies = {4.7844}; // [MeVs]
  // std::vector<double> thickCalc_dE = {2, 4, 6, 8, 12, 16, 20, 24, 28, 32, 36, 40}; // [um]
  // std::vector<double> thickCalc_dE = {8, 12, 16, 20, 24, 28, 32, 36}; // [um]
  // std::vector<double> thickCalc_dE = {1, 2, 3, 4, 5, 6}; // [um]
  std::vector<double> thickCalc_dE = {2.33}; // [um]
  // std::vector<double> thickCalc_dE  = {2, 4, 5, 5.56}; // [um]
  cout << "d | edep_comp | edep_get " << endl;
  // for (double div = 500; div <= 5000; div *= 10) {
  //   fDivision = div;
    for (auto itEnergies: energies) {
      cout << itEnergies << endl;
      for (auto itThickness: thickCalc_dE) {
        double edep_comp_inv = CalcElossIntegralVolStepComputeInvDir(itEnergies, ion, mat, itThickness); 
        double edep_get_inv = CalcElossIntegralVolStepGetInvDir(itEnergies, ion, mat, itThickness); 
        double T_comp_after_dead = itEnergies - edep_comp_inv;
        double T_get_after_dead = itEnergies - edep_get_inv;
        // cout << itThickness << " & " << edep_comp_inv << " & " << edep_get_inv
        //      << " & " << edep_comp_inv - edep_get_inv
        //      << " \\\\ " << endl
        //      << "\\hline" << endl;
        cout << itThickness << " " << edep_comp_inv << " " << T_comp_after_dead << endl;


        double edep_comp = CalcElossIntegralVolStepCompute(T_comp_after_dead, ion, mat, itThickness); 
        double edep_get = CalcElossIntegralVolStepGet(T_get_after_dead, ion, mat, itThickness); 
        double T_restored_comp = T_comp_after_dead + edep_comp;
        double T_restored_get = T_get_after_dead + edep_get;
        // cout << itThickness << " " << (itEnergies - T_restored_comp)*1e3 << " | " << (itEnergies - T_restored_get)*1e3 << endl;
        cout << itThickness << " " << edep_comp << " " << T_restored_comp << endl;
        // cout << (itEnergies - T_restored_comp)*1e3 << " | " << (itEnergies - T_restored_get)*1e3 << endl;

      }
    }  
  // }
  exit(1);
}

//--------------------------------------------------------------------------------------------------
void FRA_handler_ctrlc(int)
{
  LOG(INFO) << "*********** CTRL C PRESSED *************";
  gFRAIsInterrupted = kTRUE;
}

//--------------------------------------------------------------------------------------------------
void ERRunAna::Run(Int_t Ev_start, Int_t Ev_end)
{
  gFRAIsInterrupted = kFALSE;

  if (fTimeStamps) {
    RunTSBuffers();
  } else {
    UInt_t tmpId =0;
    //  if (fInputFile==0) {
    if (!fInFileIsOpen) {
      DummyRun(Ev_start,Ev_end);
      return;
    }

   Int_t MaxAllowed=fRootManager->CheckMaxEventNo(Ev_end);
    if ( MaxAllowed != -1 ) {
      if (Ev_end==0) {
        if (Ev_start==0) {
          Ev_end=MaxAllowed;
        } else {
          Ev_end =  Ev_start;
          if ( Ev_end > MaxAllowed ) {
            Ev_end = MaxAllowed;
          }
          Ev_start=0;
        }
      } else {
        if (Ev_end > MaxAllowed) {
          cout << "-------------------Warning---------------------------" << endl;
          cout << " -W FairRunAna : File has less events than requested!!" << endl;
          cout << " File contains : " << MaxAllowed  << " Events" << endl;
          cout << " Requested number of events = " <<  Ev_end <<  " Events"<< endl;
          cout << " The number of events is set to " << MaxAllowed << " Events"<< endl;
          cout << "-----------------------------------------------------" << endl;
          Ev_end = MaxAllowed;
        }
      }
      LOG(INFO) << "FairRunAna::Run() After checking, the run will run from event " << Ev_start << " to " << Ev_end << ".";
    }
    else {
      LOG(INFO) << "FairRunAna::Run() continue running without stop";
    }

    Int_t readEventReturn = 0;

    for (int i=Ev_start; i< Ev_end || MaxAllowed==-1 ; i++) {

      if (!ContentForAnalysis(i)) {
        continue;
      }

      gSystem->IgnoreInterrupt();
      //  gFRAIsInterrupted = kFALSE;
      signal(SIGINT, FRA_handler_ctrlc);

      if ( gFRAIsInterrupted ) {
        LOG(WARNING) << "FairRunAna::Run() Event loop was interrupted by the user!";
        break;
      }

      readEventReturn = fRootManager->ReadEvent(i);

      if ( readEventReturn != 0 ) {
        LOG(WARNING) << "FairRunAna::Run() fRootManager->ReadEvent(" << i << ") returned " << readEventReturn << ". Breaking the event loop";
        break;
      }

      fRootManager->FillEventHeader(fEvtHeader);

      tmpId = fEvtHeader->GetRunId();
      if ( tmpId != fRunId ) {
        fRunId = tmpId;
        if ( !fStatic ) {
          Reinit( fRunId );
          fTask->ReInitTask();
        }
      }
      //std::cout << "WriteoutBufferData with time: " << fRootManager->GetEventTime();
      fRootManager->StoreWriteoutBufferData(fRootManager->GetEventTime());
      fTask->ExecuteTask("");
      Fill();
      fRootManager->DeleteOldWriteoutBufferData();
      fTask->FinishEvent();

      if (NULL !=  FairTrajFilter::Instance()) {
        FairTrajFilter::Instance()->Reset();
      }

    }

    fRootManager->StoreAllWriteoutBufferData();
    fTask->FinishTask();
    fRootManager->LastFill();
    fRootManager->Write();
  }
}
//--------------------------------------------------------------------------------------------------
bool ERRunAna::ContentForAnalysis(Int_t iEvent) {
  if (fUserCut != "") {
    if (!fEventsForProcessing->GetBinContent(iEvent)){
      LOG(INFO) << "  Skip event with user cut"<< FairLogger::endl;
      return kFALSE;
    }
  }
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERRunAna::MarkFill(Bool_t flag){
  if (!fHoldEventsCount)
    fMarkFill = flag;
}
ClassImp(ERRunAna)

