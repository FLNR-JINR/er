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

#include "ERNDTrack.h"
#include "ERBeamDetParticle.h"
#include "ERSupport.h"
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
  fNDParticles = new TClonesArray("ERNDParticle",1000);
  ioman->Register("NDParticle", "ND particle", fNDParticles, kTRUE);
  fSetup = ERNDSetup::Instance();
  fSetup->ReadGeoParamsFromParContainer();
  fBeamDetParticle = (TClonesArray*) ioman->GetObject("BeamDetParticle.");
  if (!fBeamDetParticle) {
    LOG(FATAL) << "Branch BeamDetParticle not found." << FairLogger::endl;
  }
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
Double_t KineticEnergyOnTarget(const ERNDTrack& track, Double_t kineticEnergy) {
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
  ERBeamDetParticle* beamdet_particle = (ERBeamDetParticle*)fBeamDetParticle->At(0);
  if (!beamdet_particle) {
      //fRun->MarkFill(kFALSE);
      return ;
  }
  const Double_t mass = 939.57;
  const auto calcState = [mass](const ERNDTrack* track, Double_t kineticEnergy) -> TLorentzVector {
    const Double_t momentumMag = sqrt(pow(kineticEnergy, 2) + 2 * mass * kineticEnergy);
    TVector3 direction = (track->DetectorVertex()-track->TargetVertex());
    direction.SetMag(1.);
    const auto momentum = momentumMag * direction;
    const Double_t fullEnergy = sqrt(pow(momentumMag, 2)+pow(mass, 2));
    return TLorentzVector(momentum, fullEnergy);
  };
  for (Int_t iTrack(0); iTrack < fNDTracks->GetEntriesFast(); iTrack++) {
    const auto* track = static_cast<ERNDTrack*>(fNDTracks->At(iTrack));
    auto kineticEnergy = track->Edep();
    TLorentzVector detectorState = calcState(track, kineticEnergy);
    auto time_on_target = beamdet_particle->time_on_target();
    auto time = track->Time();
    auto tof = time - time_on_target;
    auto distance_between_target_and_detector = (track->DetectorVertex() - track->TargetVertex()).Mag();
    auto beta = distance_between_target_and_detector * 1e-2 / (tof * 1e-9) / TMath::C();

    if(beta <= 0 || beta >= 1) {
      LOG(DEBUG) << "[ERNDPID] Wrong beta " << beta << FairLogger::endl;
      return ;
    }
    auto gamma = 1. / TMath::Sqrt(1.- beta * beta);
    auto p = beta * gamma * mass;
    auto E = TMath::Sqrt(p * p + mass * mass);
    TLorentzVector lv(p * TMath::Sin(track->Direction().Theta()) * TMath::Cos(track->Direction().Phi()),
                      p * TMath::Sin(track->Direction().Theta()) * TMath::Sin(track->Direction().Phi()),
                      p * TMath::Cos(track->Direction().Theta()),
                      E);
    AddParticle(lv, tof);
  }
}
//--------------------------------------------------------------------------------------------------
void ERNDPID::Reset() {
    fNDParticles->Clear();
}
//--------------------------------------------------------------------------------------------------
ERNDParticle* ERNDPID::AddParticle(const TLorentzVector& lv, float tof) {
  return new((*fNDParticles)[fNDParticles->GetEntriesFast()])
                             ERNDParticle(lv, tof);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERNDPID)