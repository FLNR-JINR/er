/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNDPID.h"

#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Neutron.hh"
#include "G4EmCalculator.hh"
#include "G4NistManager.hh"

#include "TGeoManager.h"

#include "FairLogger.h"

#include "ERTrack.h"
//--------------------------------------------------------------------------------------------------
ERNDPID::ERNDPID()
    : ERTask("ER ND particle reconstruction.") 
{
}
//--------------------------------------------------------------------------------------------------
InitStatus ERNDPID::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fNDTracks = (TClonesArray*) ioman->GetObject("NDTrack");
  if (!fNDTracks) Fatal("Init", "Can`t find collection NDTrack!"); 
  fNDParticles = new TClonesArray("ERParticle",1000);
  ioman->Register("NDParticle", "ND particle", fNDParticles, kTRUE);
  fSetup = ERNDSetup::Instance();
  fSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
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
Double_t KineticEnergyOnTarget(const ERTrack& track, Double_t kineticEnergy) {
  // Init track in back direction.
  auto backDirection = (track.TargetVertex() - track.DetectorVertex());
  backDirection.SetMag(1.);
  TGeoNode* node = gGeoManager->InitTrack(
    track.DetectorVertex().X(), track.DetectorVertex().Y(), track.DetectorVertex().Z(),
    backDirection.X(), backDirection.Y(), backDirection.Z());
  Bool_t targetHasPassed = kFALSE;
  const auto particle = G4Neutron::Definition();
  while(!gGeoManager->IsOutside()) {
    gGeoManager->FindNextBoundary();
    const bool trackInTarget = TString(gGeoManager->GetPath()).Contains("target");
    if (targetHasPassed && !trackInTarget)
      break;
    targetHasPassed = trackInTarget;
    const auto step = gGeoManager->GetStep();
    // We take into account only half of step in target.
    const auto range = trackInTarget ? step / 2 : step;
    const TString materialName = node->GetMedium()->GetMaterial()->GetName();
    if (materialName == TString("Stilbene")) {
      node = gGeoManager->Step();
      continue;
    }
    const auto* material = G4NistManager::Instance()->FindOrBuildMaterial(materialName.Data());
    LOG(DEBUG) <<"[ERNDPID] [CalcEnergyDeposites]"
                <<" Calc energy deposite for range " << range << " in materail "
                << materialName << " with kinetic energy " << kineticEnergy << FairLogger::endl;
    const auto deadDeposite = CalcElossIntegralVolStep(kineticEnergy, *particle, *material, range);
    kineticEnergy += deadDeposite;
    node = gGeoManager->Step();
  }
  return kineticEnergy;
}
//--------------------------------------------------------------------------------------------------
void ERNDPID::Exec(Option_t* opt) {
  Reset();
  const Double_t mass = 0.93957;
  const auto calcState = [mass](const ERTrack* track, Double_t kineticEnergy) -> TLorentzVector {
    const Double_t momentumMag = sqrt(pow(kineticEnergy, 2) + 2 * mass * kineticEnergy);
    TVector3 direction = (track->DetectorVertex()-track->TargetVertex());
    direction.SetMag(1.);
    const auto momentum = momentumMag * direction;
    const Double_t fullEnergy = sqrt(pow(momentumMag, 2)+pow(mass, 2));
    return TLorentzVector(momentum, fullEnergy);
  };
  for (Int_t iTrack(0); iTrack < fNDTracks->GetEntriesFast(); iTrack++) {
    const auto* track = static_cast<ERTrack*>(fNDTracks->At(iTrack));
    auto kineticEnergy = track->Edep();
    TLorentzVector detectorState = calcState(track, kineticEnergy);
    kineticEnergy = KineticEnergyOnTarget(*track, kineticEnergy);
    TLorentzVector targetState = calcState(track, kineticEnergy);
    AddParticle(detectorState, targetState);
  }
}
//--------------------------------------------------------------------------------------------------
void ERNDPID::Reset() {
    fNDParticles->Clear();
}
//--------------------------------------------------------------------------------------------------
ERParticle* ERNDPID::AddParticle(const TLorentzVector& detectorState, 
                                 const TLorentzVector& targetState) {
  return new((*fNDParticles)[fNDParticles->GetEntriesFast()])
      ERParticle(detectorState, targetState);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERNDPID)