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


  // Double_t range = 8e-3;
  // Double_t T = 7.68690;
  // Double_t integralEloss = 0.;
  // Double_t intStep = range / 1e1;
  // Double_t curStep = 0.;

  // G4IonTable* ionTable = G4IonTable::GetIonTable();
  // G4ParticleDefinition* ion = ionTable->GetIon(1000020040);
  // G4NistManager* nist = G4NistManager::Instance();
  // G4Material* mat = nist->FindOrBuildMaterial("silicon");

  // G4EmCalculator* calc = new G4EmCalculator();
  // while (curStep <= range) {
  //   Double_t eloss = calc->GetDEDX(T,ion,mat)*intStep;
  //   integralEloss += eloss;
  //   T -= eloss;
  //   curStep += intStep;
  // }
  // cout << "integralEloss " << integralEloss << endl;
cout << "ERRunAna integration " << endl; 
G4IonTable* ionTable = G4IonTable::GetIonTable();
G4ParticleDefinition* ion =  ionTable->GetIon(1000020040);
G4EmCalculator* calc = new G4EmCalculator();
G4NistManager* nist = G4NistManager::Instance();
G4Material* mat = nist->FindOrBuildMaterial("silicon");
double range = 1e-4;
double T = 7.68690;
std::vector<double> energies = {4.7844, 6.0024, 7.6869}; // [MeVs]
// std::vector<double> thickCalc_E  = {2, 4, 5, 5.56}; // [um]
// std::vector<double> thickCalc_dE = {2, 4, 5, 5.56, 8, 12, 16, 20, 24, 28, 32, 36, 40}; // [um]
// std::vector<double> thickCalc_dE = {2, 4, 6, 8, 12, 16, 20, 24, 28, 32, 36, 40}; // [um]
// std::vector<double> thickCalc_dE = {8, 12, 16, 20, 24, 28, 32, 36}; // [um]
std::vector<double> thickCalc_dE = {1, 2, 3, 4, 5, 6}; // [um]
// std::vector<double> thickCalc_dE ={2.27}; // [um]
// std::vector<double> thickCalc_dE  = {2, 4, 5, 5.56}; // [um]

// for (auto itEnergies: energies) {
//   double curStep = 0.;  // [um]
//   double energy = itEnergies;
//   double curDE = 0;
//   cout << "itEnergies " << itEnergies << endl;
//   auto itThickCalc_dE = thickCalc_dE.begin();
//   while (itThickCalc_dE != thickCalc_dE.end()) {
//     Double_t edep = calc->GetDEDX(energy,ion,mat)*intStep*1e-4*10;
//     curDE += edep;
//     energy -= edep;
//     curStep += intStep;
//     if (curStep >= *itThickCalc_dE) {
//       itThickCalc_dE++;
//       cout << curDE << " ";
//     }
//   } 
//   cout << endl;
// }  
double intStep;// = 2. / 100.; // [um]
for (auto itEnergies: energies) {
  double curStep = 0.;  // [um]
  double energy = itEnergies;
  double energy_comp = itEnergies;
  double curDE = 0;
  double curDE_comp = 0;
  cout << itEnergies << endl;
  auto itThickCalc_E = thickCalc_dE.begin();
  intStep = *itThickCalc_E / 1e3;
  while (itThickCalc_E != thickCalc_dE.end()) {
    Double_t edep = calc->GetDEDX(energy,ion,mat)*intStep*1e-4*10;
    Double_t edep_comp = calc->ComputeDEDX(energy,ion,"ionIoni",mat)*intStep*1e-4*10;
    curDE += edep;
    curDE_comp += edep_comp;
    energy_comp -= edep_comp;
    energy -= edep;
    curStep += intStep;
    if (curStep > *itThickCalc_E) {
      // cout << *itThickCalc_E 
      //      << " & " << curDE 
      //      << " & " << curDE_comp 
      //      << " & " << (curDE - curDE_comp)*1e3 
      //      << " \\\\"
      //      << endl << "\\hline" << endl;
      itThickCalc_E++;
      cout << energy << " "  << curDE << " | " << energy_comp << " " << curDE_comp << endl;
      // cout << energy << " " << curDE << endl;
      // cout << itEnergies - energy_comp << endl;
      curDE = 0;
      curDE_comp = 0;
      curStep = 0.;
      energy = itEnergies;
      energy_comp = itEnergies;
      intStep = *itThickCalc_E / 1e3;
    }
  }
  cout << endl;
}  
// double energy4H = 7.6869;
// std::vector<double> dEdx;
// std::vector<double> E;
// intStep = 1e-3;
// while (energy4H > 0) {
//   Double_t edep = calc->GetDEDX(energy4H,ion,mat)*1e-3;
//   dEdx.push_back(edep);
//   E.push_back(energy4H);
//   cout << edep << " " << energy4H << endl;
//   energy4H -= 0.1;
// }
// cout << endl;

// auto canv = new TCanvas();
// auto gr = new TGraph(dEdx.size(), &E[0], &dEdx[0]);
// gr->SetLineColor(kBlue);
// gr->SetMarkerColor(kBlue);
// gr->SetLineWidth(2);
// gr->SetMarkerSize(1.5);
// gr->SetMarkerStyle(21);
// gr->SetName("Bethe-Bloch");
// gr->SetTitle("Bethe-Bloch Alpha");
// gr->Draw("APL");
// gr->GetXaxis()->SetTitle("E,[MeV]");
// gr->GetYaxis()->SetTitle("dE/dx,[MeV/um]");
// canv->SaveAs("BetheBloch.root");
// for (auto itEnergies: energies) {
//   cout << "E = " << itEnergies << endl;
//   for (auto it_thick_E: thickCalc_E) {
//     Double_t edep = calc->GetDEDX(itEnergies,ion,mat)*it_thick_E*1e-4*10;
//     cout << itEnergies - edep <<  " ";
//   }
//   cout << endl << endl;
//   for (auto it_thick_dE: thickCalc_dE) {
//     Double_t edep = calc->GetDEDX(itEnergies,ion,mat)*it_thick_dE*1e-4*10;
//     cout << edep << " ";
//   }
//   cout << endl << endl;    
//   for (auto it_thick_dE: thickCalc_E) {
//     Double_t edep = calc->GetDEDX(itEnergies,ion,mat)*it_thick_dE*1e-4*10;
//     Double_t edep_compute = calc->ComputeDEDX(itEnergies,ion,"",mat)*it_thick_dE*1e-4*10;
//     cout << "edep_compute " << edep_compute << endl;
//     cout << "| " << edep - edep_compute << " |; ";
//   }
//   cout << endl;
//   cout << "===================" << endl;
// }

// std::cout << endl << "=====" << endl << "edep " << edep << std::endl;
// exit(0);
// exit(0);  
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

