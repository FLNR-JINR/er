#include "ERBeamDetPID.h"

#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4NistManager.hh"

#include "TVector3.h"
#include "TMath.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERRunAna.h"
#include "ERDetectorList.h"
#include "ERBeamDetSetup.h"
#include "ERDigi.h"

//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID()
  : ERTask("ER BeamDet particle finding scheme")
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID(Int_t verbose)
  : ERTask("ER BeamDet particle finding scheme ", verbose)
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::SetBoxPID(Double_t tof1, Double_t tof2, Double_t dE1, Double_t dE2) {
  fToF1 = tof1;
  fToF2 = tof2;
  fdE1  = dE1;
  fdE2  = dE2;
}
//--------------------------------------------------------------------------------------------------
InitStatus ERBeamDetPID::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;

  if (fPID == -1){
    LOG(FATAL) << "PID not defined for ERBeamDetPID!" << FairLogger::endl;
  }
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetToFDigi1 = (TClonesArray*) ioman->GetObject("BeamDetToFDigi1");
  fBeamDetToFDigi2 = (TClonesArray*) ioman->GetObject("BeamDetToFDigi2");
  fBeamDetTrack    = (TClonesArray*) ioman->GetObject("BeamDetTrack");

  fProjectile = new TClonesArray("ERBeamDetParticle", consts::approx_beamdet_particle_number);

  // Register output object fProjectile

  ioman->Register("BeamDetParticle.", "BeamDet Particle", fProjectile, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
  fBeamDetSetup->GetGeoParamsFromParContainer();
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Exec(Option_t* opt) { 
  LOG(DEBUG) << "[ERBeamDetPID]---------------------Started-----------------------------------------"
             << FairLogger::endl;
  Reset();
  if (!fBeamDetTrack->At(0) || !fBeamDetToFDigi1->At(0) || !fBeamDetToFDigi2->At(0)) {
    LOG(DEBUG)  << "[ERBeamDetPID] No track" << FairLogger::endl;
    //fRun->MarkFill(kFALSE);
    return;
  }
  Double_t ToF1, ToF2, ToF;
  Double_t dE1, dE2, dE;
  Double_t probability;
  Double_t beta;
  Double_t gamma;
  Double_t p, energy;
  ERDigi* digi;
  digi = (ERDigi*)fBeamDetToFDigi1->At(0);
  ToF1 = digi->Time();
  dE1 = digi->Edep();
  LOG(DEBUG) << "[ERBeamDetPID] dE1 = " << dE1 << " ToF1 = " << ToF1 << FairLogger::endl;
  digi = (ERDigi*)fBeamDetToFDigi2->At(0);
  ToF2 = digi->Time();
  dE2 = digi->Edep();
  LOG(DEBUG) << "[ERBeamDetPID] dE2 = " << dE2 << " ToF2 = " << ToF2 << FairLogger::endl;
  dE = dE1 + dE2;
  LOG(DEBUG) << "[ERBeamDetPID] dE = " << dE << " Gev; " << " ToF1 = " << ToF1 << " ns;" 
             << " ToF2 = " << ToF2 << " ns;" << FairLogger::endl;
  ToF = TMath::Abs(ToF2 - ToF1) + fOffsetToF;
  LOG(DEBUG) << "[ERBeamDetPID] dE = " << dE << " Gev; " << " ToF = " << ToF << " ns;" 
             << FairLogger::endl;
  if(ToF <= fToF1 || ToF >= fToF2 || dE <= fdE1 || dE >= fdE2){
    probability = 0;
  }
  else {
    probability = 1;
  }
  if(probability < fProbabilityThreshold) {
    LOG(DEBUG) << "[ERBeamDetPID] Probability " << probability << " less then threshold "
               << fProbabilityThreshold << FairLogger::endl;
    //fRun->MarkFill(kFALSE);
    return ;
  }
  LOG(DEBUG) << "[ERBeamDetPID] Mass " << fIonMass << FairLogger::endl;
  Double_t distanceBetweenToF = fBeamDetSetup->GetDistanceBetweenToF(0, fBeamDetSetup->GetToFCount() - 1);
  beta = distanceBetweenToF * 1e-2 / (ToF * 1e-9) / TMath::C();
  if(beta <= 0 || beta >= 1) {
    LOG(DEBUG) << "[ERBeamDetPID] Wrong beta " << beta << FairLogger::endl;
    //fRun->MarkFill(kFALSE);
    return ;
  }
  gamma = 1. / TMath::Sqrt(1.- beta*beta);
  p = beta * gamma * fIonMass;
  Double_t px, py, pz;
  auto* track = dynamic_cast<ERBeamDetTrack*>(fBeamDetTrack->At(0));
  px = p * TMath::Sin(track->GetVector().Theta()) * TMath::Cos(track->GetVector().Phi());
  py = p * TMath::Sin(track->GetVector().Theta()) * TMath::Sin(track->GetVector().Phi());
  pz = p * TMath::Cos(track->GetVector().Theta());
  energy = fIonMass * gamma;
  LOG(DEBUG) << "[ERBeamDetPID] TOF State:: PID: " << fPID << "; px: " << px << "; py: " << py 
            << "; pz: " << pz << " energy: " << energy << "; probability " << probability 
            << FairLogger::endl;
  //eloss calculation, T-kinetic energy on target
  Double_t T = CalcEloss(*track, fPID , p, fIonMass);
  Double_t pt, ptx, pty, ptz, et;
  pt = TMath::Sqrt(T * T + 2. * T * fIonMass);
  ptx = pt * TMath::Sin(track->GetVector().Theta()) * TMath::Cos(track->GetVector().Phi());
  pty = pt * TMath::Sin(track->GetVector().Theta()) * TMath::Sin(track->GetVector().Phi());
  ptz = pt * TMath::Cos(track->GetVector().Theta());
  et = fIonMass + T;
  LOG(DEBUG) << "[ERBeamDetPID] Target State::  px: " << ptx << "; py: " << pty << "; pz: " << ptz 
            << " energy: " << et << FairLogger::endl;
  AddParticle(fPID, TLorentzVector(px, py, pz, energy), TLorentzVector(ptx,pty,ptz,et), probability);
  LOG(DEBUG) << "[ERBeamDetPID]---------------------Finished----------------------------------------"
             << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
double ERBeamDetPID::CalcEloss(ERBeamDetTrack& track, int pid, float mom, float mass){
  FairRun* run = FairRun::Instance();
  if (!TString(run->ClassName()).Contains("ERRunAna"))
    LOG(FATAL) << "[ERBeamDet] Use ERRunAna for ERBeamDetPID!!!" << FairLogger::endl;
  //calculation ion energy loss in BeamDet volumes
  const TVector3 target_vertex = track.GetTargetVertex();
  LOG(DEBUG) << "[ERBeamDet][CalcEloss] Eloss calculation with target vertex = (" << target_vertex.X() 
            << "," << target_vertex.Y() << "," << target_vertex.Z() << "), direction on target = ("
            << track.GetVector().X() << "," << track.GetVector().Y() << "," << track.GetVector().Z() 
            << ")" << FairLogger::endl;
  Float_t z_start = -3000.;//@TODO change to BeamDet start position
  Float_t x_start = target_vertex.X() 
                    + z_start*TMath::Sin(track.GetVector().Theta()) * TMath::Cos(track.GetVector().Phi());
  Float_t y_start = target_vertex.Y()
                    + z_start*TMath::Sin(track.GetVector().Theta()) * TMath::Sin(track.GetVector().Phi());
  LOG(DEBUG) << "[ERBeamDet][CalcEloss] Eloss calculation start vertex = (" << x_start << "," 
             << y_start << "," << z_start << ")" << FairLogger::FairLogger::endl; 
  G4IonTable* ion_table = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion = ion_table->GetIon(pid);
  G4NistManager* nist = G4NistManager::Instance();
  const auto track_direction = track.GetDirection();
  TGeoNode* node = gGeoManager->InitTrack(x_start, y_start, z_start, track_direction.X(),
                                          track_direction.Y(), track_direction.Z());
  Float_t E = TMath::Sqrt(mom * mom + mass * mass);
  Float_t T = E - mass;
  Float_t sumLoss = 0.;
  Bool_t firstTofAlreadySkipped = kFALSE;
  while(!gGeoManager->IsOutside()) {
    TString matName = node->GetMedium()->GetMaterial()->GetName();
    G4Material* mat = nist->FindOrBuildMaterial(matName.Data()); 
    node = gGeoManager->FindNextBoundary();
    Double_t step = gGeoManager->GetStep();
    const TVector3 current_position(gGeoManager->GetCurrentPoint());
    LOG(DEBUG) << "[ERBeamDet][CalcEloss] track position (" << current_position.X() << ", " 
               << current_position.Y() << ", " << current_position.Z() << ")" << FairLogger::endl;
    LOG(DEBUG) << "[ERBeamDet][CalcEloss] path  = " <<  gGeoManager->GetPath() 
               << FairLogger::FairLogger::endl;
    if (!firstTofAlreadySkipped && TString(gGeoManager->GetPath()).Contains("ToF")) {
      firstTofAlreadySkipped = kTRUE;
      node = gGeoManager->Step();
      continue;
    }
    if (step == 0.)
      break;
    if (!firstTofAlreadySkipped) {
      node = gGeoManager->Step();
      continue;
    }
    const double step_to_target_vertex = (target_vertex - current_position).Mag();
    const bool is_last_step = step_to_target_vertex <= step;
    step = is_last_step ? step_to_target_vertex : step;
    Double_t edep = CalcElossIntegralVolStep(T, *ion, *mat, step);
    node = gGeoManager->GetCurrentNode();
    LOG(DEBUG) <<"[ERBeamDet][CalcEloss] Kinetic Energy  = " << T << " medium " << matName 
               << " step  = " << step << " edep = " << edep << FairLogger::endl;
    T -= edep;
    sumLoss += edep;
    if (is_last_step)
      break;
    node = gGeoManager->Step();
  }
  if (!firstTofAlreadySkipped) {
    LOG(FATAL) << "[ERBeamDet][CalcEloss] ToF not found." << FairLogger::endl;
  }
  LOG(DEBUG) <<"[ERBeamDet][CalcEloss] Sum Eloss = " <<  sumLoss << FairLogger::endl;
  return T;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Reset() {
  if (fProjectile) {
    fProjectile->Clear();
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::SetIonMassNumber(Int_t a) {
  TDatabasePDG* pdgDB = TDatabasePDG::Instance();
  TParticlePDG* kProton = pdgDB->GetParticle(2212);
  Double_t kProtonMass=kProton->Mass();
  fIonMass = kProtonMass * Double_t(a);
}
//--------------------------------------------------------------------------------------------------
ERBeamDetParticle* ERBeamDetPID::AddParticle(Int_t pid, TLorentzVector tofState, 
                                             TLorentzVector targetState, Double_t probability){
 return new((*fProjectile)[fProjectile->GetEntriesFast()])
              ERBeamDetParticle(pid, tofState, targetState,  probability); 
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetPID)
