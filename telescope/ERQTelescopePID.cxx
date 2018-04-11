/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopePID.h"

#include "TVector3.h"
#include "TMath.h"
#include "TGeoNode.h"
#include "TGeoManager.h"

#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmCalculator.hh"
#include "G4NistManager.hh"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "FairEventHeader.h"

#include "ERBeamDetTrack.h"
#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"

#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID()
  : FairTask("ER qtelescope particle identification scheme"),
  fEventsForProcessing(NULL),
  fQTelescopeSetup(NULL),
  fUserCut("")
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID(Int_t verbose)
  : FairTask("ER qtelescope particle identification scheme", verbose),
  fEventsForProcessing(NULL),
  fQTelescopeSetup(NULL),
  fUserCut("")
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::~ERQTelescopePID() {
}
//--------------------------------------------------------------------------------------------------
InitStatus ERQTelescopePID::Init() {
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;

  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    
    if (bFullName.Contains("Digi") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
      LOG(DEBUG) << "Digi branche " << brName << FairLogger::endl; 
    }

    if (bFullName.Contains("Track") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeTrack[brName] = (TClonesArray*) ioman->GetObject(bFullName);

      //Creating particles collections for every track collection
      for (auto itPDG : fStationParticles[brName]){
        TString brParticleName;
        brParticleName.Form("%s_%d",brName.Data(),itPDG);
        fQTelescopeParticle[brName][itPDG] = new TClonesArray("ERQTelescopeParticle");
        ioman->Register("ERQTelescopeParticle_" + brParticleName, "QTelescope", 
                    fQTelescopeParticle[brName][itPDG], kTRUE);
      }
    }
  }

  if (fUserCut != ""){
    LOG(INFO) << "User cut " << fUserCut << " implementation" << FairLogger::endl;
    TTree* tree = ioman->GetInTree();
    fEventsForProcessing =  new TH1I ("hist", "Events for processing", tree->GetEntries(), 1, tree->GetEntries());
    tree->Draw("MCEventHeader.GetEventID()>>hist",fUserCut,"goff");
  }

  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fQTelescopeSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Exec(Option_t* opt) { 
  
  Reset();
  
  Int_t mcEvent = FairRun::Instance()->GetEventHeader()->GetMCEntryNumber();
  LOG(INFO) << "Event " << mcEvent <<" ERQTelescopePID: " << FairLogger::endl;
  if (fUserCut != "")
    if (!fEventsForProcessing->GetBinContent(mcEvent)){
      LOG(INFO) << "  Skip event with user cut"<< FairLogger::endl;
      return;
    }

  for (const auto itTrackBranches : fQTelescopeTrack) {

    LOG(DEBUG) << " Work with traks in " << itTrackBranches.first << FairLogger::endl;

    for (Int_t iTrack(0); iTrack < itTrackBranches.second->GetEntriesFast(); iTrack++){

      ERQTelescopeTrack* track = (ERQTelescopeTrack*)itTrackBranches.second->At(iTrack);

      for (const auto itParticesBranches : fQTelescopeParticle[itTrackBranches.first]){

        Int_t pdg = itParticesBranches.first;

        //sum edep on road from telescope hit produce station to CsI and edep in CsI digi
        Double_t csIedep = FindCsIEdepByTrack(track,pdg);
        
        Float_t T = track->GetSumEdep() + csIedep;
        Double_t deadEloss = CalcEloss(itTrackBranches.first,track, pdg,T);

        //mass 
        G4IonTable* ionTable = G4IonTable::GetIonTable();
        G4ParticleDefinition* ion =  ionTable->GetIon(pdg);
        Float_t mass = ion->GetPDGMass()/1000.; //GeV

        //LotentzVector on telescope
        Double_t P = sqrt(pow(T,2) + 2*mass*T);
        TVector3 direction = track->GetTelescopeVertex()-track->GetTargetVertex();
        TLorentzVector lvTelescope (P*sin(direction.Theta())*cos(direction.Phi()),
                                    P*sin(direction.Theta())*sin(direction.Phi()),
                                    P*cos(direction.Theta()),
                                    sqrt(pow(P,2)+pow(mass,2)));
        //LorentVector on target
        T -= deadEloss;
        P = sqrt(pow(T,2) + 2*mass*T);
        TLorentzVector lvTarget (P*sin(direction.Theta())*cos(direction.Phi()),
                                    P*sin(direction.Theta())*sin(direction.Phi()),
                                    P*cos(direction.Theta()),
                                    sqrt(pow(P,2)+pow(mass,2)));

        AddParticle(lvTelescope, lvTarget, deadEloss,itParticesBranches.second);

      }

    }

  }

}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Reset() {
  for (const auto itTrackBranches : fQTelescopeParticle) {
    for (const auto itParticleBranches : itTrackBranches.second)
      if (itParticleBranches.second) {
        itParticleBranches.second->Delete();
      }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Finish() {   
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeParticle* ERQTelescopePID::AddParticle(TLorentzVector lvTelescope, TLorentzVector lvTarget, Double_t deadEloss, TClonesArray* col) 
{
  ERQTelescopeParticle *particle = new((*col)
                                        [col->GetEntriesFast()])
                                        ERQTelescopeParticle(lvTelescope,lvTarget,deadEloss);
  return particle;
}
//------------------------------------------------------------------------------------s--------------
void ERQTelescopePID::SetParContainers() {
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::CalcEloss(TString station, ERQTelescopeTrack* track, Int_t pdg, Double_t T){
  
 /* FairRun* run = FairRun::Instance();
  if (!TString(run->ClassName()).Contains("ERRunAna") or !TString(run->ClassName()).Contains("ERRunSim")){
    LOG(FATAL) << "Use ERRunAna for ERQTelescopePID::CalcEloss!!!" << FairLogger::endl;
    return 0;
  }*/

  //calclculation ion energy loss in BeamDet volumes
  TVector3 telescopeVertex = track->GetTelescopeVertex();
  TVector3 direction = track->GetTargetVertex() - telescopeVertex;
  direction.SetMag(1.);

  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(pdg);
  Float_t mass = ion->GetPDGMass()/1000.; //GeV
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();


  LOG(DEBUG) << " [CalcEloss] Dead eloss calculation for station " << station << " for pdg " << pdg 
              << " with mass = " << mass << " with telescope vertex = (" << telescopeVertex.X() << ","
              << telescopeVertex.Y() << "," << telescopeVertex.Z() 
              << " with direction = " << direction.X() << "," << direction.Y() << "," << direction.Z()
              << FairLogger::endl;
  
  TGeoNode* node;
  node = gGeoManager->InitTrack(telescopeVertex.X(),telescopeVertex.Y(),telescopeVertex.Z(),
                                direction.X(),direction.Y(),direction.Z());
  
  Float_t sumLoss = 0.;
  

  Bool_t inTarget = kFALSE;
  Float_t tarEdep = 0.;
  Bool_t firstSens = kTRUE;

  while(!gGeoManager->IsOutside()){
    
    TString matName = node->GetMedium()->GetMaterial()->GetName();
    G4Material* mat = nist->FindOrBuildMaterial(matName.Data());
    
    node = gGeoManager->FindNextBoundary();

    LOG(DEBUG) <<" [CalcEloss]  path  = " <<  gGeoManager->GetPath() << FairLogger::endl;

    if (inTarget && !(TString(gGeoManager->GetPath()).Contains("target")))
      break;
    if (TString(gGeoManager->GetPath()).Contains("Sensitive")){
      if (firstSens){
        firstSens = kFALSE;
        LOG(DEBUG) <<" [CalcEloss]    Pass first sens volume" << FairLogger::endl;
      }
      else{
        LOG(DEBUG) <<" [CalcEloss]    Sensetive Volume -> get Eloss from digi" << FairLogger::endl;
        Double_t edep = FindDigiEdepByNode(node);
        T += edep;
      }
      node = gGeoManager->Step();
      continue;
    }
    
    Double_t range = gGeoManager->GetStep();
    Double_t edep = calc->GetDEDX(T*1e3,ion,mat)*range*10*1e-3;

    node = gGeoManager->GetCurrentNode();
    
    LOG(DEBUG) <<" [CalcEloss]    Kinetic Energy  = " << T << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    medium " << matName << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    range  = " << range << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    edep = " << edep << FairLogger::endl;

    if (TString(gGeoManager->GetPath()).Contains("target"))
      inTarget = kTRUE;

    if (inTarget)
      tarEdep+=edep;

    T += edep;
    sumLoss += edep;
  
    node = gGeoManager->Step();
  }
  
  T += tarEdep/2.;
  sumLoss -= tarEdep/2.;
  
  LOG(DEBUG) <<" [CalcEloss] Target Eloss = " <<  tarEdep << FairLogger::endl;
  LOG(DEBUG) <<" [CalcEloss] Sum Eloss = " <<  sumLoss << FairLogger::endl;

  return sumLoss;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::FindDigiEdepByNode(TGeoNode* node){
  //@ TODO Now working only for SingleSi
  Double_t edep = 0.;

  TString brNamePrefix = node->GetMotherVolume()->GetName();
  LOG(DEBUG) <<" [CalcEloss]    Branch name prefix " << brNamePrefix << FairLogger::endl;
  
  TString brName = "";
  for (auto digiBranch : fQTelescopeDigi){
    TString currentBrNamePrefix(digiBranch.first(0,digiBranch.first.Last('_')));
    if (currentBrNamePrefix == brNamePrefix)
      brName = digiBranch.first;
  }
  if (brName == ""){
    LOG(WARNING) << " [CalcEloss]   Branch not found in telescope branches name" << FairLogger::endl;
    return 0.;
  }
  else{
    TString sensVolName = node->GetName();
    Int_t bLastPostfix = sensVolName.Last('_'); 
    TString stripNbStr(sensVolName(bLastPostfix + 1, sensVolName.Length()));
    Int_t stripNb = stripNbStr.Atoi();
    Bool_t found = kFALSE;

    for (Int_t iDigi = 0; iDigi < fQTelescopeDigi[brName]->GetEntriesFast(); iDigi++){
      ERQTelescopeSiDigi* digi = (ERQTelescopeSiDigi*)fQTelescopeDigi[brName]->At(iDigi);
      if (digi->GetStripNb() == stripNb){
        found = kTRUE;
        LOG(DEBUG) << " [CalcEloss]   Found digi with edep " << digi->GetEdep() << FairLogger::endl;
        edep = digi->GetEdep();
        break;
      }
    }
    if (!found)
      LOG(WARNING) << " [CalcEloss]   Digi with strip number " << stripNb << " not found in collection" << FairLogger::endl;
  }


  return edep;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::FindCsIEdepByTrack(ERQTelescopeTrack* track, Int_t pdg){
  Double_t edep = 0;
  Int_t CsInb = -1;

  TVector3 telescopeVertex = track->GetTelescopeVertex();
  TVector3 direction = telescopeVertex - track->GetTargetVertex();
  direction.SetMag(1.);

  TGeoNode* node;
  node = gGeoManager->InitTrack(telescopeVertex.X(),telescopeVertex.Y(),telescopeVertex.Z(),
                                direction.X(),direction.Y(),direction.Z());

  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(pdg);
  Float_t mass = ion->GetPDGMass()/1000.; //GeV
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();

  std::vector<Double_t> ranges;
  std::vector<TString> materials;
  
  Bool_t finish = kFALSE;
  Bool_t first = kTRUE;
  while(!gGeoManager->IsOutside()){
    TString path =  gGeoManager->GetPath();
    LOG(DEBUG) <<" [FindCsIByTrack]  path  = " << path  << FairLogger::endl;

    if (first && path.Contains("Sensitive")){
      LOG(DEBUG) << " [FindCsIByTrack] Skip first Sensitive " << FairLogger::endl; 
      first = kFALSE;
      node = gGeoManager->FindNextBoundary();
      node = gGeoManager->Step();
      continue;
    }
    
    if (TString(node->GetName()).Contains("CsIBoxShell")){
      
      LOG(DEBUG) << " [FindCsIByTrack] CsI found" << FairLogger::endl;
      
      TString sensVolName = node->GetName();
      Int_t bLastPostfix = sensVolName.Last('_'); 
      TString CsINbStr(sensVolName(bLastPostfix + 1, sensVolName.Length()));
      CsInb = CsINbStr.Atoi();

      LOG(DEBUG) << " [FindCsIByTrack] CsI crystall nb " << CsInb << FairLogger::endl;

      for (auto branch : fQTelescopeDigi){
        if (path.Contains(branch.first)) {
          LOG(DEBUG) << " [FindCsIByTrack] CsI Branch found " << branch.first << FairLogger::endl;
          for (Int_t iDigi = 0; iDigi < branch.second->GetEntriesFast(); iDigi++){
            ERQTelescopeCsIDigi* digi = (ERQTelescopeCsIDigi*)branch.second->At(iDigi);
            if (digi->BlockNb() == CsInb){
              LOG(DEBUG) << " [FindCsIByTrack] Found CsI with edep " << digi->Edep() << FairLogger::endl;
              edep = digi->Edep();
            }
          }
        }
      }
      
      finish = kTRUE;
    }
  

    //LOG(DEBUG) << " [FindCsIByTrack] node name " << node->GetName() << FairLogger::endl;
    LOG(DEBUG) << " [FindCsIByTrack] material  " << node->GetMedium()->GetMaterial()->GetName();

    materials.push_back(node->GetMedium()->GetMaterial()->GetName());
    node = gGeoManager->FindNextBoundary();

    ranges.push_back(gGeoManager->GetStep());
    Double_t range = gGeoManager->GetStep();
    LOG(DEBUG) << " range " <<range << FairLogger::endl;
   
    
    if (finish)
      break;

    node = gGeoManager->Step();
  }

  if (edep == 0)
    LOG(DEBUG) << " [FindCsIByTrack] CsI not found!" << FairLogger::endl;
  else{
    LOG(DEBUG) << " [FindCsIByTrack] Calc eloss in dead layers " << FairLogger::endl;
    Float_t T = edep;
    for (Int_t iRange = ranges.size()-1; iRange >= 0; iRange--){
      G4Material* mat = nist->FindOrBuildMaterial(materials[iRange].Data());
      Double_t rangeEdep = calc->GetDEDX(T*1e3,ion,mat)*ranges[iRange]*10*1e-3;
      edep += rangeEdep;

      LOG(DEBUG) << " [FindCsIByTrack] Ekin " << T <<  " range " << ranges[iRange]
                <<  " material " << materials[iRange] 
                <<  " edep " << rangeEdep << FairLogger::endl;

      T+=rangeEdep;
    }
    LOG(DEBUG) << " [FindCsIByTrack] Sum edep from hit to Csi " << edep << FairLogger::endl;
  }

  return edep;

}

ClassImp(ERQTelescopePID)