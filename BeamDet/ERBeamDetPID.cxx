#include "ERBeamDetPID.h"

#include "TVector3.h"
#include "TMath.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERRunAna.h"
#include "ERDetectorList.h"
#include "ERBeamDetSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID()
  : ERTask("ER BeamDet particle finding scheme")
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID(Int_t verbose)
  : ERTask("ER BeamDet particle finding scheme ", verbose),
  fPID(-1)
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetPID::~ERBeamDetPID() {
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

  fProjectile = new TClonesArray("ERBeamDetParticle",1000);

  // Register output object fProjectile

  ioman->Register("BeamDetParticle.", "BeamDet Particle", fProjectile, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
  fBeamDetSetup->GetGeoParamsFromParContainer();
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Exec(Option_t* opt) { 
  Reset();

  if (!fBeamDetTrack->At(0) || !fBeamDetToFDigi1->At(0) || !fBeamDetToFDigi2->At(0)) {
    LOG(DEBUG)  << "ERBeamDetPID: No track" << FairLogger::endl;
    fRun->MarkFill(kFALSE);
    return;
  }

  Double_t ToF1, ToF2, ToF;
  Double_t dE1, dE2, dE;
  Double_t probability;

  Double_t beta;
  Double_t gamma;

  Double_t p, energy;

  ERBeamDetTOFDigi* digi;

  digi = (ERBeamDetTOFDigi*)fBeamDetToFDigi1->At(0);
  ToF1 = digi->GetTime();
  dE1 = digi->GetEdep();
  LOG(DEBUG) << "dE1 = " << dE1 << " ToF1 = " << ToF1 << FairLogger::endl;
  digi = (ERBeamDetTOFDigi*)fBeamDetToFDigi2->At(0);
  ToF2 = digi->GetTime();
  dE2 = digi->GetEdep();
  LOG(DEBUG) << "dE2 = " << dE2 << " ToF2 = " << ToF2 << FairLogger::endl;

  dE = dE1 + dE2;
  LOG(DEBUG) << "dE = " << dE << " Gev; " << " ToF1 = " << ToF1 << " ns;" << " ToF2 = " << ToF2 << " ns;" << FairLogger::endl;
  ToF = TMath::Abs(ToF2 - ToF1) + fOffsetToF;
  LOG(DEBUG) << "dE = " << dE << " Gev; " << " ToF = " << ToF << " ns;" << FairLogger::endl;

  if(ToF <= fToF1 || ToF >= fToF2 || dE <= fdE1 || dE >= fdE2){
    probability = 0;
  }
  else {
    probability = 1;
  }

  if(probability < fProbabilityThreshold) {
    LOG(DEBUG) << "Probability " << probability << " less then threshold " << fProbabilityThreshold << FairLogger::endl;
    fRun->MarkFill(kFALSE);
    return ;
  }
  LOG(DEBUG) << "Mass " << fIonMass << FairLogger::endl;
  Double_t distanceBetweenToF = fBeamDetSetup->GetDistanceBetweenToF(0, fBeamDetSetup->GetToFCount() - 1);
  beta = distanceBetweenToF * 1e-2 / (ToF * 1e-9) / TMath::C();
  if(beta <= 0 || beta >= 1) {
    LOG(DEBUG) << "Wrong beta " << beta << FairLogger::endl;
    fRun->MarkFill(kFALSE);
    return ;
  }

  gamma = 1. / TMath::Sqrt(1.- beta*beta);
  p = beta * gamma * fIonMass;

  Double_t px, py, pz;

  ERBeamDetTrack* track = (ERBeamDetTrack*)fBeamDetTrack->At(0);

  px = p * TMath::Sin(track->GetVector().Theta()) * TMath::Cos(track->GetVector().Phi());
  py = p * TMath::Sin(track->GetVector().Theta()) * TMath::Sin(track->GetVector().Phi());
  pz = p * TMath::Cos(track->GetVector().Theta());

  energy = fIonMass * gamma;
  LOG(DEBUG) << "TOF State:: PID: " << fPID << "; px: " << px << "; py: " << py << "; pz: " << pz 
            << " energy: " << energy << "; probability " << probability << FairLogger::endl;

  //eloss calculation, T-kinetic energy on target
  Double_t T = fBeamDetSetup->CalcEloss(*track,fPID,p,fIonMass);
  
  Double_t pt, ptx, pty, ptz, et;

  pt = TMath::Sqrt(T*T + 2.*T*fIonMass);
  ptx = pt * TMath::Sin(track->GetVector().Theta()) * TMath::Cos(track->GetVector().Phi());
  pty = pt * TMath::Sin(track->GetVector().Theta()) * TMath::Sin(track->GetVector().Phi());
  ptz = pt * TMath::Cos(track->GetVector().Theta());

  et = fIonMass + T;

  LOG(DEBUG) << "Target State::  px: " << ptx << "; py: " << pty << "; pz: " << ptz 
            << " energy: " << et << FairLogger::endl;

  AddParticle(fPID, TLorentzVector(px, py, pz, energy), TLorentzVector(ptx,pty,ptz,et), probability);
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Reset() {
  if (fProjectile) {
    fProjectile->Clear();
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Finish(){    
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::SetIonMassNumber(Int_t a) {
  TDatabasePDG* pdgDB = TDatabasePDG::Instance();
  TParticlePDG* kProton = pdgDB->GetParticle(2212);
  Double_t kProtonMass=kProton->Mass();
  fIonMass = kProtonMass * Double_t(a);
}
//--------------------------------------------------------------------------------------------------
ERBeamDetParticle* ERBeamDetPID::AddParticle(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability){
 return new((*fProjectile)[fProjectile->GetEntriesFast()])
              ERBeamDetParticle(pid, tofState, targetState,  probability); 
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetPID)
