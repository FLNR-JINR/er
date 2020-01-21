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
  : ERTask("ER qtelescope particle identification scheme") {
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID(Int_t verbose)
  : ERTask("ER qtelescope particle identification scheme", verbose) {
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
InitStatus ERQTelescopePID::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) 
    Fatal("Init", "No FairRootManager");
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
    LOG(DEBUG) << "[ERQTelescopePID] Work with traks in " << station << FairLogger::endl;
    for (Int_t iTrack(0); iTrack < stationTracks->GetEntriesFast(); iTrack++) { 
      const auto* track = static_cast<ERQTelescopeTrack*>(stationTracks->At(iTrack));
      for (const auto itParticesBranches : fQTelescopeParticle[station]) {
        // Get particle mass by PDG from G4IonTable
        const auto pdg = itParticesBranches.first;
        auto* ionTable = G4IonTable::GetIonTable();
        const auto* particle = ionTable->GetIon(pdg);
        const auto mass = particle->GetPDGMass() / 1000.; //GeV
        // Find geometry point to start track back propagation.
        const auto backPropagationStartPoint = FindBackPropagationStartPoint(*track);
        // Calc particle energy deposites in setup using back propagation from start point 
        // to middle of target volume. We assume that kinetic energy 
        // is fully detected by the setup, so calculate it as sum of 
        // energy deposites in digi and energy deposites in passive volumes (using G4EmCalculator).
        const auto energyDeposites = CalcEnergyDeposites(
            *track, backPropagationStartPoint, *particle);
        const auto digisDeposite = energyDeposites.first;
        const auto deadDeposite = energyDeposites.second;
        const auto kineticEnergy = digisDeposite + deadDeposite;
        // Calculate Lorentz vector of particle at the exit of the reaction in lab system.
        const Double_t momentumMag = sqrt(pow(kineticEnergy, 2) + 2 * mass * kineticEnergy);
        TVector3 direction = (track->GetTelescopeVertex()-track->GetTargetVertex());
        direction.SetMag(1.);
        const auto momentum = momentumMag * direction;
        const Double_t fullEnergy = sqrt(pow(momentumMag, 2)+pow(mass, 2));
        const TLorentzVector lvTarget (momentum, fullEnergy);
        // Add particle to collection.
        auto& particleCollection = *itParticesBranches.second;
        AddParticle(lvTarget, kineticEnergy, deadDeposite, particleCollection);
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
ERQTelescopeParticle* ERQTelescopePID::
AddParticle(const TLorentzVector& lvInteraction, const Double_t kineticEnergy, const Double_t deadEloss,
            TClonesArray& col) {
  return new(col[col.GetEntriesFast()]) ERQTelescopeParticle(lvInteraction, kineticEnergy, deadEloss);
}
//--------------------------------------------------------------------------------------------------
Double_t CalcElossIntegralVolStep (Double_t kineticEnergy, const G4ParticleDefinition& particle, 
                                   const G4Material& material, const Double_t range) { 
  //FIXME copy-past from ERBeamDetSetup
  if (range <= 0.)
    return 0;
  Double_t integralEloss = 0.;
  const Double_t intStep = range / 20;
  Double_t curStep = 0.;
  G4EmCalculator* calc = new G4EmCalculator();
  while (curStep < range) {
    Double_t eloss = calc->GetDEDX(kineticEnergy * 1e3 /* MeV */, &particle, &material) * intStep 
                     * 10 /* 1/mm */ * 1e-3 /* GeV */;
    integralEloss += eloss;
    kineticEnergy += eloss;
    curStep += intStep;
  }
  return integralEloss;
}
//--------------------------------------------------------------------------------------------------
TVector3 ERQTelescopePID::FindBackPropagationStartPoint(const ERQTelescopeTrack& track) {
  // Return geometry point at which the track exit last sensetive volume
  // on its direct propagation. If sensetive volume was not found, return 
  // track telescope vertex.
  const TVector3 telescopeVertex = track.GetTelescopeVertex();
  TVector3 backPropagationStartPoint = telescopeVertex;
  TVector3 direction = telescopeVertex - track.GetTargetVertex();
  direction.SetMag(1.);
  TGeoNode* currentNode = gGeoManager->InitTrack(telescopeVertex.X(), telescopeVertex.Y(), telescopeVertex.Z(),
                                                 direction.X(), direction.Y(), direction.Z());
  TGeoNode* lastSensetiveNode = nullptr;
  TString lastSensetivePath;
  TVector3 lastSensetivePosition;
  while(!gGeoManager->IsOutside()) {
    bool inSensetiveVolume = false;
    const TString path = gGeoManager->GetPath();
    if (path.Contains("Sensitive")) { 
      // Enter sensetive volume. Next step will be in sensetive volume.
      inSensetiveVolume = true;
      lastSensetiveNode = currentNode;
      lastSensetivePath = path; 
    }
    currentNode = gGeoManager->FindNextBoundary();
    currentNode = gGeoManager->Step();
    if (inSensetiveVolume) {
      // position, when track exit sensetve volume
      lastSensetivePosition = TVector3(gGeoManager->GetCurrentPoint());
    }
  }
  if (lastSensetiveNode) {
    backPropagationStartPoint = lastSensetivePosition;
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track "
               << lastSensetivePath << FairLogger::endl;
  } else {
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track not found. "
               << "Track telescope vertex will be used as start point for back propagation\n";
  }
  LOG(DEBUG) << "[FindBackPropagationStartPoint] Back propagation start point " 
             << "(" << backPropagationStartPoint.X() << "," << backPropagationStartPoint.Y()
             << "," << backPropagationStartPoint.Z() << ")" << FairLogger::endl;
  return backPropagationStartPoint;
}
//--------------------------------------------------------------------------------------------------
std::pair<Double_t, Double_t> ERQTelescopePID::
CalcEnergyDeposites(const ERQTelescopeTrack& track, const TVector3& startPoint, 
                    const G4ParticleDefinition& particle) {
  // Calc paritcle energy deposites in setup using back track propagation from start point.
  // Return pair: first - sum of energy deposites in digi(sensetive volumes); 
  //              second - sum of energy deposites in passive volumes (or dead energy deposite).
  Double_t digiDepositesSum = 0.;
  Double_t deadDepositesSum = 0.;
  // We start with kinetic energy equal zero, because we assume
  // that start point is an exit point of the last sensetive volume,
  // which track has passed in setup. If the track stopped earlier, 
  // it will be taken into account automatically, because particle 
  // with zero kinetic energy can not loss energy ;)
  Double_t kineticEnergy = 0.;
  // Init track in back direction.
  auto backDirection = (track.GetTargetVertex() - track.GetTelescopeVertex());
  backDirection.SetMag(1.);
  LOG(DEBUG) << "[ERQTelescopePID] [CalcEnergyDeposites] Energy deposites calculation" 
             << " for particle " << particle.GetParticleName() 
             << "; start point = (" << startPoint.X() << ","  << startPoint.Y() << "," << startPoint.Z() 
             << "; and direction = " << backDirection.X() << "," << backDirection.Y() << "," << backDirection.Z()
             << FairLogger::endl;
  TGeoNode* node = gGeoManager->InitTrack(startPoint.X(), startPoint.Y(), startPoint.Z(),
                                          backDirection.X(), backDirection.Y(), backDirection.Z());
  // While track not in target volume or outside the setup,
  // accumulate energy deposites and kinetic energy.
  Bool_t targetHasPassed = kFALSE;
  while(!gGeoManager->IsOutside()) {
    gGeoManager->FindNextBoundary();
    LOG(DEBUG) <<"[ERQTelescopePID] [CalcEnergyDeposites] path  = " 
               << gGeoManager->GetPath() << FairLogger::endl;
    const bool trackInTarget = TString(gGeoManager->GetPath()).Contains("target");
    if (targetHasPassed && !trackInTarget)
      break;
    targetHasPassed = trackInTarget;
    // If track in sensetive volume, try to find digi
    if (TString(gGeoManager->GetPath()).Contains("Sensitive")){
      LOG(DEBUG) <<"[ERQTelescopePID] [CalcEnergyDeposites]"
                 <<" Get energy deposite from digi." << FairLogger::endl;
      const auto digiDeposite = FindDigiEdepByNode(*node);
      kineticEnergy += digiDeposite;
      digiDepositesSum += digiDeposite;
    } else  { // track in passive volume
      const auto step = gGeoManager->GetStep();
      // We take into account only half of step in target.
      const auto range = trackInTarget ? step / 2 : step;
      const TString materialName = node->GetMedium()->GetMaterial()->GetName();
      const auto* material = G4NistManager::Instance()->FindOrBuildMaterial(materialName.Data());
      LOG(DEBUG) <<"[ERQTelescopePID] [CalcEnergyDeposites]"
                 <<" Calc energy deposite for range " << range << " in materail "
                 << materialName << " with kinetic energy " << kineticEnergy << FairLogger::endl;
      const auto deadDeposite = CalcElossIntegralVolStep(kineticEnergy, particle, *material, range);
      kineticEnergy += deadDeposite;
      deadDepositesSum += deadDeposite;
    }
    LOG(DEBUG) <<"[ERQTelescopePID] [CalcEnergyDeposites] Current kinetic Energy  = " 
               << kineticEnergy << FairLogger::endl;
    node = gGeoManager->Step();
  }
  LOG(DEBUG) <<"[ERQTelescopePID] [CalcEnergyDeposites] Finish deposite calculation with " 
             << "Kinetic energy = " << kineticEnergy << " = digis sum deposite  : " << digiDepositesSum
             << " + dead deposites sum : " << deadDepositesSum << FairLogger::endl;
  return {digiDepositesSum, deadDepositesSum};
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopePID::FindDigiEdepByNode(const TGeoNode& node){
  //@ TODO Now working only for SingleSi
  Double_t edep = 0.;
  TString brNamePrefix = node.GetMotherVolume()->GetName();
  // modify prefix in case of DoubleSi or pseudo volumes.
  if (brNamePrefix.Contains("pseudo") || brNamePrefix.Contains("doubleSi")) {
    // example: /cave_1/QTelescopeTmp_0/CT_3/CT_DoubleSi_DSD_XY_0/doubleSiStrip_XY_0/SensitiveDoubleSiBox_XY_16
    TString path = gGeoManager->GetPath();
    path.Remove(path.Last('/'), path.Length());
    path.Remove(path.Last('/'), path.Length());
    path.Remove(0, path.Last('/') + 1);
    if (brNamePrefix.Contains("pseudo"))
      path.Remove(path.Length()-2, path.Length());
    brNamePrefix = path;
  }
  LOG(DEBUG) <<" [ERQTelescopePID] [CalcEnergyDeposites] Branch name prefix " 
             << brNamePrefix << FairLogger::endl;
  TString brName = "";
  for (auto digiBranch : fQTelescopeDigi){
    TString currentBrNamePrefix(digiBranch.first(0,digiBranch.first.Last('_')));
    // std::cout << "currentBrNamePrefix " << currentBrNamePrefix << std::endl;
    // std::cout << "currentBrNamePrefix " << currentBrNamePrefix << std::endl;
    if (currentBrNamePrefix == brNamePrefix)
      brName = digiBranch.first;
  }
  if (brName == ""){  
    LOG(DEBUG) << " [ERQTelescopePID] [CalcEnergyDeposites]  Branch not found in telescope branches name" 
                 << FairLogger::endl;
    return 0.;
  } else {
    TString sensVolName = node.GetName();
    Int_t bLastPostfix = sensVolName.Last('_'); 
    TString stripNbStr(sensVolName(bLastPostfix + 1, sensVolName.Length()));
    Int_t stripNb = stripNbStr.Atoi();
    Bool_t found = kFALSE;
    for (Int_t iDigi = 0; iDigi < fQTelescopeDigi[brName]->GetEntriesFast(); iDigi++){
      ERQTelescopeSiDigi* digi = (ERQTelescopeSiDigi*)fQTelescopeDigi[brName]->At(iDigi);
      if (digi->GetStripNb() == stripNb){
        found = kTRUE;
        LOG(DEBUG) << " [ERQTelescopePID] [CalcEnergyDeposites] Found digi with edep " 
                   << digi->GetEdep() << FairLogger::endl;
        edep = digi->GetEdep();
        break;
      }
    }
    if (!found)
      LOG(DEBUG) << " [ERQTelescopePID] [CalcEnergyDeposites]  Digi with strip number " 
                   << stripNb << " not found in collection" << FairLogger::endl;
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
      Double_t rangeEdep = CalcElossIntegralVolStep(T, *ion, *mat, ranges[iRange]);
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
