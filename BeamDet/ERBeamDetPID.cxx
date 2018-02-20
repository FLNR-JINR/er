#include "ERBeamDetPID.h"

#include "TVector3.h"
#include "TMath.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "FairRootManager.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERBeamDetSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID()
  : FairTask("ER BeamDet particle finding scheme")
{
}
//--------------------------------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID(Int_t verbose)
  : FairTask("ER BeamDet particle finding scheme ", verbose)
{
}
//--------------------------------------------------------------------------------------------------
ERBeamDetPID::~ERBeamDetPID() {
}
//--------------------------------------------------------------------------------------------------
InitStatus ERBeamDetPID::Init() {
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetToFDigi1 = (TClonesArray*) ioman->GetObject("BeamDetToFDigi1");
  fBeamDetToFDigi2 = (TClonesArray*) ioman->GetObject("BeamDetToFDigi2");
  fBeamDetTrack  = (ERBeamDetTrack*) ioman->GetObject("BeamDetTrack.");

  // Register output object fProjectile
  fProjectile = (ERBeamDetParticle*)new ERBeamDetParticle();

  ioman->Register("BeamDetParticle.", "BeamDet Particle", fProjectile, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
  fBeamDetSetup->GetGeoParamsFromParContainer();
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::Exec(Option_t* opt) { 
  Reset();
  if (!fBeamDetTrack || !fBeamDetToFDigi1->At(0) || !fBeamDetToFDigi2->At(0)) {
    LOG(DEBUG)  << "ERBeamDetPID: No track" << FairLogger::endl;
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
  dE1 = digi->Edep();
  LOG(DEBUG) << "dE1 = " << dE1 << " ToF1 = " << ToF1 << FairLogger::endl;
  digi = (ERBeamDetTOFDigi*)fBeamDetToFDigi2->At(0);
  ToF2 = digi->GetTime();
  dE2 = digi->Edep();
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
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }
  LOG(DEBUG) << "Mass " << fIonMass << FairLogger::endl;
  beta = fBeamDetSetup->DistanceBetweenToF() * 1e-2 / (ToF * 1e-9) / TMath::C();
  if(beta <= 0 || beta >= 1) {
    LOG(DEBUG) << "Wrong beta " << beta << FairLogger::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  gamma = 1. / TMath::Sqrt(1.- beta*beta);
  p = beta * gamma * fIonMass;

  Double_t px, py, pz;

  px = p * TMath::Sin(fBeamDetTrack->GetVector().Theta()) * TMath::Cos(fBeamDetTrack->GetVector().Phi());
  py = p * TMath::Sin(fBeamDetTrack->GetVector().Theta()) * TMath::Sin(fBeamDetTrack->GetVector().Phi());
  pz = p * TMath::Cos(fBeamDetTrack->GetVector().Theta());

  energy = fIonMass * gamma;
  LOG(DEBUG) << "PID: " << fPID << "; px: " << px << "; py: " << py << "; pz: " << pz 
            << " energy: " << energy << "; probability " << probability << FairLogger::endl;

  AddParticle(fPID, TLorentzVector(px, py, pz, energy), probability);
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
ERBeamDetParticle* ERBeamDetPID::AddParticle(Int_t pid, TLorentzVector fourMomentum, Double_t probability){
 fProjectile->AddParameters(pid, fourMomentum, probability); 
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetPID::SetParContainers() {
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetPID)
