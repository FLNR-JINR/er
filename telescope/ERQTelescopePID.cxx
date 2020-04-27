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
      if (fParticleDescriptions.find(brName) == fParticleDescriptions.end())
        continue;
      fQTelescopeTrack[brName] = (TClonesArray*) ioman->GetObject(bFullName);
      //Creating particles collections
      for (auto particleDescription : fParticleDescriptions[brName]){
        TString brParticleName;
        const auto pdg = particleDescription.fPDG;
        brParticleName.Form("%s_%d",brName.Data(), pdg);
        fQTelescopeParticle[brName][pdg] = new TClonesArray("ERQTelescopeParticle");
        ioman->Register("ERQTelescopeParticle_" + brParticleName, "QTelescope", 
                        fQTelescopeParticle[brName][pdg], kTRUE);
      }
    }
  }
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fQTelescopeSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::SetParticle(
    const TString& trackBranchName, const PDG pdg, 
    const TString& deStation /*= ""*/, const TString& eStation /*= ""*/,
    const Double_t deNormalizedThickness /*= 0.002*/) {
  if ((deStation == "" && eStation != "")
      || (deStation != "" && eStation == ""))
    LOG(FATAL) << "If one of dE or E station is defined, another should be defined." 
               << FairLogger::endl;
  fParticleDescriptions[trackBranchName].emplace_back(pdg, deStation, eStation, deNormalizedThickness);
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Exec(Option_t* opt) {
  LOG(DEBUG) << "[ERQTelescopePID]------------Started-----------------------------------------------"
             << FairLogger::endl;
  Reset();
  for (const auto& itTrackBranches : fQTelescopeTrack) {
    const auto& trackBranch = itTrackBranches.first;
    const auto& tracks = itTrackBranches.second;
    LOG(DEBUG) << "[ERQTelescopePID] Work with traks in " << trackBranch << FairLogger::endl;
    for (Int_t iTrack(0); iTrack < tracks->GetEntriesFast(); iTrack++) { 
      const auto* track = static_cast<ERQTelescopeTrack*>(tracks->At(iTrack));
      for (const auto particleDescription : fParticleDescriptions[trackBranch]) {
        // Get particle mass by PDG from G4IonTable
        const auto pdg = particleDescription.fPDG;
        auto* ionTable = G4IonTable::GetIonTable();
        const auto* particle = ionTable->GetIon(pdg);
        const auto mass = particle->GetPDGMass() / 1000.; //GeV
        // Find geometry point to start track back propagation.
        const auto backPropagationStartPoint = FindBackPropagationStartPoint(*track);
        // Calc particle energy deposites in setup using back propagation from start point 
        // to middle of target volume. We assume that kinetic energy 
        // is fully detected by the setup, so calculate it as sum of 
        // energy deposites in digi and energy deposites in passive volumes (using G4EmCalculator).
        std::list<DigiOnTrack> digisOnTrack;
        const auto energyDeposites = CalcEnergyDeposites(
            *track, backPropagationStartPoint, *particle, digisOnTrack);
        Double_t edepInThickStation = -1., edepInThinStation = -1., 
                 edepInThickStationCorrected = -1., edepInThinStationCorrected = -1.;
        FindEnergiesForDeEAnalysis(trackBranch, digisOnTrack, 
                                   particleDescription.fEStation, particleDescription.fDeStation,
                                   particleDescription.fNormalizedThickness, edepInThickStation, edepInThinStation,
                                   edepInThickStationCorrected, edepInThinStationCorrected);
        const auto digisDeposite = energyDeposites.first;
        const auto deadDeposite = energyDeposites.second;
        const auto kineticEnergy = digisDeposite + deadDeposite;
        // Calculate Lorentz vector of particle at the exit of the reaction in lab system.
        const Double_t momentumMag = sqrt(pow(kineticEnergy, 2) + 2 * mass * kineticEnergy);
        TVector3 direction = track->GetDirection();
        const auto momentum = momentumMag * direction;
        const Double_t fullEnergy = sqrt(pow(momentumMag, 2)+pow(mass, 2));
        const TLorentzVector lvTarget (momentum, fullEnergy);
        // Add particle to collection.
        auto& particleCollection = *fQTelescopeParticle[trackBranch][pdg];
        AddParticle(lvTarget, kineticEnergy, deadDeposite, edepInThickStation, edepInThinStation,
                    edepInThickStationCorrected, edepInThinStationCorrected, particleCollection);
      }
    }
  }
  LOG(DEBUG) << "[ERQTelescopePID]------------Finished----------------------------------------------"
             << FairLogger::endl;
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
            Double_t edepInThickStation, Double_t edepInThinStation,
            Double_t edepInThickStationCorrected, Double_t edepInThinStationCorrected,
            TClonesArray& col) {
  return new(col[col.GetEntriesFast()]) ERQTelescopeParticle(lvInteraction, kineticEnergy, deadEloss,
                                            edepInThickStation, edepInThinStation, 
                                            edepInThickStationCorrected, edepInThinStationCorrected);
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
  const TVector3 telescopeVertex = track.GetXStationVertex();
  TVector3 backPropagationStartPoint = telescopeVertex;
  const auto direction = track.GetDirection();
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
                    const G4ParticleDefinition& particle,
                    std::list<DigiOnTrack>& digisOnTrack ) {
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
  auto backDirection = track.GetBackDirection();
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
      const auto branchAndDigi = FindDigiEdepByNode(*node, gGeoManager->GetPath());
      const auto digiBranch = branchAndDigi.first;
      const auto* digi = branchAndDigi.second;
      if (digi) {
        digisOnTrack.emplace_back(digiBranch, digi, gGeoManager->GetStep());
      }
      const auto digiDeposite = digi ? digi->Edep() : 0.;
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
std::pair<TString, const ERDigi*> ERQTelescopePID::FindDigiEdepByNode(const TGeoNode& node, const TString& nodePath) {
  // example SSD: /cave_1/QTelescopeTmp_0/Telescope_4_5/Telescope_4_SingleSi_SSD_V_4_X_11/SensitiveSingleSiStrip_X_2
  // example DSD: /cave_1/QTelescopeTmp_0/CT_3/CT_DoubleSi_DSD_XY_0/doubleSiStrip_XY_0/SensitiveDoubleSiBox_XY_16
  // example CsI: /cave_1/QTelescopeTmp_0/Central_telescope_3/Central_telescope_CsI_0/CsIBoxShell_8/SensitiveCsIBox_1
  // example NonUniform SSD: ?
  const auto getDigiBranchSubString = [&node, &nodePath]() -> TString {
    if (nodePath.Contains("SingleSi")) {
      return node.GetMotherVolume()->GetName(); // for ex: Telescope_4_SingleSi_SSD_V_4_X_11
    }
    if (nodePath.Contains("DoubleSi") || nodePath.Contains("CsI") || nodePath.Contains("pseudo")) {
      TString digiBranchSubString = nodePath;
      digiBranchSubString.Remove(digiBranchSubString.Last('/'), digiBranchSubString.Length());
      digiBranchSubString.Remove(digiBranchSubString.Last('/'), digiBranchSubString.Length());
      digiBranchSubString.Remove(0, digiBranchSubString.Last('/') + 1);
      if (nodePath.Contains("pseudo"))
        digiBranchSubString.Remove(digiBranchSubString.Length()-2, digiBranchSubString.Length());
      return digiBranchSubString; // for ex: CT_DoubleSi_DSD_XY_0, Central_telescope_CsI_0
    }
  };
  const auto digiBranchSubstring = getDigiBranchSubString();
  LOG(DEBUG) <<"   [ERQTelescopePID][CalcEnergyDeposites] Digi branch substring " 
             << digiBranchSubstring << FairLogger::endl;
  const auto itDigiBranch = std::find_if(fQTelescopeDigi.begin(), fQTelescopeDigi.end(), 
      [&digiBranchSubstring] (const std::pair<TString, TClonesArray*> digiCollection) {
          return digiCollection.first.Contains(digiBranchSubstring);
      }
  );
  if (itDigiBranch == fQTelescopeDigi.end()){  
    LOG(ERROR) << "   [ERQTelescopePID][CalcEnergyDeposites]  Branch with substring " << digiBranchSubstring
               << " not found." << FairLogger::endl;
    return {"", nullptr};
  }
  const auto getChannel = [&node, &nodePath]() {
    TString pathWithChannelPostfix = nodePath;
    if (nodePath.Contains("CsI"))
      pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
    const TString channelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                              pathWithChannelPostfix.Length()));
    return channelStr.Atoi();
  };
  const auto channel = getChannel();
  const auto* digis = itDigiBranch->second;
  const auto digiBranchName = itDigiBranch->first;
  for (Int_t iDigi = 0; iDigi < digis->GetEntriesFast(); iDigi++){
    const auto* digi = static_cast<ERDigi*>(digis->At(iDigi));
    if (digi->Channel() != channel) 
      continue;
    LOG(DEBUG) << "   [ERQTelescopePID][CalcEnergyDeposites] Found digi with edep " 
               << digi->Edep() << FairLogger::endl;
    return {digiBranchName, digi};
  }
  LOG(ERROR) << "   [ERQTelescopePID][CalcEnergyDeposites] Digi with channel number " 
             << channel << " not found in collection " << digiBranchName << FairLogger::endl;
  return {"", nullptr};
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::FindEnergiesForDeEAnalysis(const TString& trackBranch, 
    const std::list<DigiOnTrack>& digisOnTrack, const TString& eStation, const TString& deStation,
    const Double_t normalizedThickness, Double_t& edepInThickStation, Double_t& edepInThinStation,
    Double_t& edepInThickStationCorrected, Double_t& edepInThinStationCorrected) {
  if (eStation == "" || deStation == "")
    return;
  const auto getDigiOnTrack = [&digisOnTrack](const TString& stationName) -> DigiOnTrack {
    const auto itDigiOnTrack =  std::find_if(digisOnTrack.begin(), digisOnTrack.end(),
        [&stationName](const DigiOnTrack& digiOntrack) {
      return digiOntrack.fBranch.Contains(stationName);
    });
    if (itDigiOnTrack == digisOnTrack.end()) {
      return DigiOnTrack();
    }
    return *itDigiOnTrack;
  };
  const auto deDigiOnTrack = getDigiOnTrack(deStation);
  if (!deDigiOnTrack.IsFound()) {
    LOG(WARNING) << "[ERQTelescopePID][FindEnergiesForDeEAnalysis] Digi for station " << deStation 
                 << " not found on track from " << trackBranch << " path." << FairLogger::endl;
    return;
  }
  const auto eDigiOnTrack = getDigiOnTrack(eStation);
  if (!eDigiOnTrack.IsFound()) {
    LOG(WARNING) << "[ERQTelescopePID][FindEnergiesForDeEAnalysis] Digi for station " << eStation 
                 << " not found on track from " << trackBranch << " path" << FairLogger::endl;
    return;
  }
  edepInThickStation = eDigiOnTrack.fDigi->Edep();
  edepInThinStation = deDigiOnTrack.fDigi->Edep();
  // de and E correction
  edepInThinStationCorrected = edepInThinStation * normalizedThickness / deDigiOnTrack.fSensetiveThickness;
  LOG(DEBUG) << "[ERQTelescopePID][FindEnergiesForDeEAnalysis] Digi from station " << deStation << " with edep = " << edepInThinStation 
             << " [GeV] corrected to edep = " << edepInThinStationCorrected << ". Normalized thickness = " 
             << normalizedThickness << ", step in sensetive volume = " << deDigiOnTrack.fSensetiveThickness
             << FairLogger::endl;
  edepInThickStationCorrected = edepInThickStation + edepInThinStation - edepInThinStationCorrected;
  LOG(DEBUG) << "[ERQTelescopePID][FindEnergiesForDeEAnalysis] Digi from station " << eStation << " with edep = " << edepInThickStation 
             << " corrected to " << edepInThickStationCorrected;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopePID)
