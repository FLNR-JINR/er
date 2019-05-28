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
fMarkFillEnable(kTRUE)
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
  if (fMarkFillEnable)
    fMarkFill = flag;
}
ClassImp(ERRunAna)

