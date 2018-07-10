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

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairLogger.h"
#include "FairEventHeader.h"

#include "ERRecoMCApplication.h"

using namespace std;

ERRunAna* ERRunAna::fInstance = NULL;
TCut      ERRunAna::fUserCut = "";
TH1I*     ERRunAna::fEventsForProcessing = NULL;
//--------------------------------------------------------------------------------------------------
ERRunAna* ERRunAna::Instance() {
  if (!fInstance) {
    fInstance = new ERRunAna();
  }
  return fInstance;
}
//--------------------------------------------------------------------------------------------------
ERRunAna::ERRunAna()
: FairRunAna()
{
  LOG(DEBUG) << "ERRunAna constructor" << FairLogger::endl;
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
    tree->Draw("MCEventHeader.GetEventID()>>hist",fUserCut,"goff");
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

}
//--------------------------------------------------------------------------------------------------
bool ERRunAna::ContentForAnalysis() {
  Int_t mcEvent = FairRun::Instance()->GetEventHeader()->GetMCEntryNumber();
  LOG(INFO) << "Event " << mcEvent <<" ERQTelescopePID: " << FairLogger::endl;
  if (fUserCut != "") {
    if (!fEventsForProcessing->GetBinContent(mcEvent)){
      LOG(INFO) << "  Skip event with user cut"<< FairLogger::endl;
      return kFALSE;
    }
  }
  return kTRUE;
}
/*Example of energy deposit calculation
  
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* mat = nist->FindOrBuildMaterial("BC408");
  cout << calc->GetDEDX(0.5, G4Proton::Definition(),mat) << endl;
*/
//--------------------------------------------------------------------------------------------------
ClassImp(ERRunAna)

