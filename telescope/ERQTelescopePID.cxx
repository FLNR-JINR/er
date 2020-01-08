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
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "FairEventHeader.h"

#include "ERBeamDetTrack.h"
#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"
#include "ERRunAna.h"

#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID()
  : ERTask("ER qtelescope particle identification scheme"),
  fQTelescopeSetup(NULL)
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID(Int_t verbose)
  : ERTask("ER qtelescope particle identification scheme", verbose),
  fQTelescopeSetup(NULL)
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::~ERQTelescopePID() {
}
//--------------------------------------------------------------------------------------------------
InitStatus ERQTelescopePID::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;

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
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fQTelescopeSetup->ReadGeoParamsFromParContainer();

  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Exec(Option_t* opt) { 
  Reset();
  for (const auto& itTrackBranches : fQTelescopeTrack) {
    const auto& station = itTrackBranches.first;
    const auto& stationTracks = itTrackBranches.second;
    LOG(DEBUG) << " Work with traks in " << station << FairLogger::endl;
    for (Int_t iTrack(0); iTrack < stationTracks->GetEntriesFast(); iTrack++){
      const auto* track = (ERQTelescopeTrack*)stationTracks->At(iTrack);
      for (const auto itParticesBranches : fQTelescopeParticle[station]){
        const Int_t pdg = itParticesBranches.first;
        TVector3 backPropagationStartPoint = FindBackPropagationStartPoint(track);
        // Calc eloss on path form start point to target
        // In case of sensetive volume - get eloss from digi
        // otherwise - use G4EmCalculator
        Double_t T = CalcEloss(station, track, backPropagationStartPoint, pdg);
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
        P = sqrt(pow(T,2) + 2*mass*T);
        TLorentzVector lvTarget (P*sin(direction.Theta())*cos(direction.Phi()),
                                    P*sin(direction.Theta())*sin(direction.Phi()),
                                    P*cos(direction.Theta()),
                                    sqrt(pow(P,2)+pow(mass,2)));
        AddParticle(lvTelescope, lvTarget, 0., itParticesBranches.second, T);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Reset() {
  gGeoManager->ResetState();
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
ERQTelescopeParticle* ERQTelescopePID::AddParticle(TLorentzVector lvTelescope, 
                                                   TLorentzVector lvTarget, 
						   Double_t deadEloss, 
						   TClonesArray* col) 
{
  ERQTelescopeParticle *particle = new((*col)
                                        [col->GetEntriesFast()])
                                        ERQTelescopeParticle(lvTelescope,lvTarget,deadEloss);
  return particle;
}
//------------------------------------------------------------------------------------s--------------
ERQTelescopeParticle* ERQTelescopePID::AddParticle(TLorentzVector lvTelescope, 
                                                   TLorentzVector lvTarget, 
                                                   Double_t deadEloss, TClonesArray* col,
                                                   Double_t T) 
{
  ERQTelescopeParticle *particle = new((*col)
                                        [col->GetEntriesFast()])
                                        ERQTelescopeParticle(lvTelescope,lvTarget,deadEloss,T);
  return particle;
}
Double_t CalcElossIntegralVolStep (Double_t T, const G4ParticleDefinition* ion, 
                                   const G4Material* mat, const Double_t range) { 
  //FIXME copy-past from ERBeamDetSetup
  assert(mat);
  assert(ion);
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = 1e-4*1e-2;//range / 20;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (curStep < range) {
    Double_t eloss = calc->GetDEDX(T*1e3,ion,mat)*intStep*10*1e-3;
    integralEloss += eloss;
    T += eloss;
    //std::cout << "CalcElossIntegralVolStep T" << T << "; eloss " << eloss << "; integralEloss " << integralEloss << std::endl;
    curStep += intStep;
  }
  return integralEloss;
}

//--------------------------------------------------------------------------------------------------
TVector3 ERQTelescopePID::FindBackPropagationStartPoint(const ERQTelescopeTrack* track) {
  const TVector3 telescopeVertex = track->GetTelescopeVertex();
  TVector3 backPropagationStartPoint = telescopeVertex;
  TVector3 direction = telescopeVertex - track->GetTargetVertex();
  direction.SetMag(1.);
  gGeoManager->ResetState();
  TGeoNode* currentNode = gGeoManager->InitTrack(telescopeVertex.X(), telescopeVertex.Y(), telescopeVertex.Z(),
                                                 direction.X(), direction.Y(), direction.Z());
  LOG(DEBUG) << "direction " << "(" << direction.X() << "," << direction.Y() << "," << direction.Z() << ")" << endl;
  TGeoNode* lastSensetiveNode = nullptr;
  TString lastSensetivePath;
  TVector3 lastSensetivePosition;
  while(!gGeoManager->IsOutside()) {
    bool inSensetiveVolume = false;
    const TString path = gGeoManager->GetPath();
    //gGeoManager->InspectState();
    //LOG(DEBUG) << "CheckPath " << gGeoManager->CheckPath(path) << FairLogger::endl;
    //LOG(DEBUG) << "nullStep " << gGeoManager->IsNullStep() << FairLogger::endl;
    //if (gGeoManager->IsNullStep()) {
    //  gGeoManager->CdNext();
    //}
    //LOG(DEBUG) << "path finder " << path << FairLogger::endl;
    if (path.Contains("Sensitive")) {
      inSensetiveVolume = true;
      lastSensetiveNode = currentNode;
      lastSensetivePath = path; 
    }
    currentNode = gGeoManager->FindNextBoundaryAndStep();
    if (inSensetiveVolume) {
      // position, when track exit sensetve volume
      lastSensetivePosition = TVector3(gGeoManager->GetCurrentPoint());
    }
  }
  if (lastSensetiveNode) {
    const TGeoMatrix* nodeMatrix = lastSensetiveNode->GetMatrix();
    backPropagationStartPoint = lastSensetivePosition;
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track "
               << lastSensetivePath << FairLogger::endl;
  } else {
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track not found. "
               << "Track telescope vertex will be used as start point for back propagation\n";
  }
  LOG(DEBUG) << "[FindBackPropagationStartPoint] Back propagation start point " 
             << "(" << backPropagationStartPoint.X() << "," << backPropagationStartPoint.Y()
             << "," << backPropagationStartPoint.Z() << FairLogger::endl;
  return backPropagationStartPoint;
}

//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::CalcEloss(const TString& station, const ERQTelescopeTrack* track,
                                    const TVector3& startPoint, const Int_t pdg) { 
  //calclculation ion energy loss volumes
  TVector3 telescopeVertex = track->GetTelescopeVertex();
  TVector3 direction = track->GetTargetVertex() - telescopeVertex;
  direction.SetMag(1.);
  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(pdg);
  Float_t mass = ion->GetPDGMass()/1000.; //GeV
  G4NistManager* nist = G4NistManager::Instance();
  LOG(DEBUG) << " [CalcEloss] Eloss calculation for station " << station << " for pdg " << pdg 
              << " with mass = " << mass << " start point = (" << startPoint.X() << ","
              << startPoint.Y() << "," << startPoint.Z() 
              << " with direction = " << direction.X() << "," << direction.Y() << "," << direction.Z()
              << FairLogger::endl;
  TGeoNode* node;
  node = gGeoManager->InitTrack(startPoint.X(), startPoint.Y(), startPoint.Z(),
                                direction.X(), direction.Y(), direction.Z());
  Bool_t inTarget = kFALSE;
  Float_t tarEdep = 0.;
  Double_t T = 0.;
  while(!gGeoManager->IsOutside()) {
    TString matName = node->GetMedium()->GetMaterial()->GetName();
    G4Material* mat = nist->FindOrBuildMaterial(matName.Data());
    node = gGeoManager->FindNextBoundary();
    LOG(DEBUG) <<" [CalcEloss]  path  = " <<  gGeoManager->GetPath() << FairLogger::endl;
    if (inTarget && !(TString(gGeoManager->GetPath()).Contains("target")))
      break;
    if (TString(gGeoManager->GetPath()).Contains("Sensitive")){
      LOG(DEBUG) <<" [CalcEloss]    Sensetive Volume -> get Eloss from digi" << FairLogger::endl;
      Double_t edep = FindDigiEdepByNode(node);
      T += edep;
      node = gGeoManager->Step();
      LOG(DEBUG) <<" [CalcEloss]    Kinetic Energy  = " << T << FairLogger::endl;
      continue;
    }
    Double_t range = gGeoManager->GetStep();
    if (range == 0.)
      break;
    if (TString(gGeoManager->GetPath()).Contains("target")) {
      inTarget = kTRUE;
      range /= 2.;
    }
    Double_t edep = CalcElossIntegralVolStep(T, ion, mat, range);
    node = gGeoManager->GetCurrentNode();
    LOG(DEBUG) <<" [CalcEloss]    Kinetic Energy  = " << T << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    medium " << matName << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    range  = " << range << FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss]    edep = " << edep << FairLogger::endl;
    if (inTarget)
      tarEdep+=edep;
    T += edep;
    node = gGeoManager->Step();
  }
  LOG(DEBUG) <<" [CalcEloss] Target Eloss = " <<  tarEdep << FairLogger::endl;
  LOG(DEBUG) <<" [CalcEloss] Ekin on target = " <<  T << FairLogger::endl;
  return T;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::FindDigiEdepByNode(TGeoNode* node){
  //@ TODO Now working only for SingleSi
  Double_t edep = 0.;

  TString brNamePrefix = node->GetMotherVolume()->GetName();
  LOG(DEBUG) <<" [CalcEloss]    Branch name prefix " << brNamePrefix << FairLogger::endl;
  if (brNamePrefix.Contains("pseudo")) {
    TString path = gGeoManager->GetPath();
    path.Remove(path.Last('/'), path.Length());
    path.Remove(path.Last('/'), path.Length());
    path.Remove(0, path.Last('/') + 1);
    path.Remove(path.Length()-2, path.Length());
    // std::cout << "debug path " << path << std::endl;
    brNamePrefix = path;
  }
  TString brName = "";
  for (auto digiBranch : fQTelescopeDigi){
    TString currentBrNamePrefix(digiBranch.first(0,digiBranch.first.Last('_')));
    // std::cout << "currentBrNamePrefix " << currentBrNamePrefix << std::endl;
    // std::cout << "currentBrNamePrefix " << currentBrNamePrefix << std::endl;
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
  node = gGeoManager->InitTrack(telescopeVertex.X(), telescopeVertex.Y(), telescopeVertex.Z(),
                                direction.X(), direction.Y(), direction.Z());

  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(pdg);
  Float_t mass = ion->GetPDGMass()/1000.; //GeV
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();

  std::vector<Double_t> ranges;
  std::vector<TString> materials;
  
  Bool_t finish = kFALSE;
  Bool_t first = kTRUE;
  Int_t CsIDigiCount = 0;
  while(!gGeoManager->IsOutside()) {
    TString path =  gGeoManager->GetPath();
    LOG(DEBUG) <<" [FindCsIEdepByTrack] path  = " << path  << FairLogger::endl;

    if (first && path.Contains("Sensitive")){
      LOG(DEBUG) << " [FindCsIEdepByTrack]    Skip first Sensitive " << FairLogger::endl; 
      first = kFALSE;
      node = gGeoManager->FindNextBoundary();
      node = gGeoManager->Step();
      continue;
    }

    if (CsIDigiCount > 0)
      if (!path.Contains("CsI")) //мы были в CsI и вышли из него в пещеру
        break;

    if (TString(node->GetName()).Contains("SensitiveCsIBox")){
      
      if (CsIDigiCount > 0)
        LOG(WARNING) << " [FindCsIEdepByTrack] Few CsI crystalls on one track" << FairLogger::endl;

      LOG(DEBUG) << " [FindCsIEdepByTrack]    CsI found " << FairLogger::endl;
      
      TObjArray *pathArray = path.Tokenize("/");
      TString shellName = ((TObjString *)pathArray->At(pathArray->GetEntries()-2))->String();
      Int_t bLastPostfix = shellName.Last('_');
      TString CsINbStr(shellName(bLastPostfix + 1, shellName.Length()));
      CsInb = CsINbStr.Atoi();

      LOG(DEBUG) << " [FindCsIEdepByTrack]      CsI crystall nb " << CsInb << FairLogger::endl;

      for (auto branch : fQTelescopeDigi){
        if (path.Contains(branch.first)) {
          LOG(DEBUG) << " [FindCsIEdepByTrack]      CsI Branch found " << branch.first << FairLogger::endl;
          for (Int_t iDigi = 0; iDigi < branch.second->GetEntriesFast(); iDigi++){
            ERQTelescopeCsIDigi* digi = (ERQTelescopeCsIDigi*)branch.second->At(iDigi);
            if (digi->GetBlockNb() == CsInb){
              LOG(DEBUG) << " [FindCsIEdepByTrack]      Found CsI with edep " << digi->GetEdep() << FairLogger::endl;
              edep = digi->GetEdep();
            }
          }
        }
      }

      node = gGeoManager->FindNextBoundary();
      node = gGeoManager->Step();
      CsIDigiCount++;
      continue;
    }
  

    //LOG(DEBUG) << " [FindCsIByTrack] node name " << node->GetName() << FairLogger::endl;
    LOG(DEBUG) << " [FindCsIEdepByTrack]    material  " << node->GetMedium()->GetMaterial()->GetName();

    materials.push_back(node->GetMedium()->GetMaterial()->GetName());
    node = gGeoManager->FindNextBoundary();

    ranges.push_back(gGeoManager->GetStep());
    Double_t range = gGeoManager->GetStep();
    LOG(DEBUG) << "   range " <<range << FairLogger::endl;
   
    
    if (finish)
      break;

    node = gGeoManager->Step();
  }

  if (edep == 0)
    LOG(DEBUG) << " [FindCsIEdepByTrack] CsI not found!" << FairLogger::endl;
  else{
    LOG(DEBUG) << " [FindCsIEdepByTrack] Calc eloss in dead layers " << FairLogger::endl;
    Float_t T = edep;
    //-2 Последний мертвый слой не учитываем, потому что мы до него не добежали 
    for (Int_t iRange = ranges.size()-2; iRange >= 0; iRange--){
      G4Material* mat = nist->FindOrBuildMaterial(materials[iRange].Data());
      Double_t rangeEdep = CalcElossIntegralVolStep(T, ion, mat, ranges[iRange]);
      edep += rangeEdep;

      LOG(DEBUG) << " [FindCsIEdepByTrack]    Ekin " << T <<  " range " << ranges[iRange]
                <<  " material " << materials[iRange] 
                <<  " edep " << rangeEdep << FairLogger::endl;

      T+=rangeEdep;
    }
    LOG(DEBUG) << " [FindCsIEdepByTrack] Sum edep from hit to Csi " << edep << FairLogger::endl;
  }

  return edep;

}

ClassImp(ERQTelescopePID)
