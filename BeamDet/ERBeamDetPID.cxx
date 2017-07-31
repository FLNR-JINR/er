#include "ERBeamDetPID.h"

#include <vector>
#include <iostream>
using namespace std;

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <iostream>

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID()
  : FairTask("ER BeamDet particle finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID(Int_t verbose)
  : FairTask("ER BeamDet particle finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetPID::~ERBeamDetPID()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetPID::SetParContainers()
{
  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetPID::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetTOFDigi1 = (TClonesArray*) ioman->GetObject("BeamDetTOFDigi1");
  fBeamDetTOFDigi2 = (TClonesArray*) ioman->GetObject("BeamDetTOFDigi2");
  fBeamDetTrack  = (ERBeamDetTrack*) ioman->GetObject("BeamDetTrack.");

  // Register output object fProjectile
  fProjectile = (ERBeamDetParticle*)new ERBeamDetParticle();

  ioman->Register("BeamDetParticle.", "BeamDet Particle", fProjectile, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetPID::Exec(Option_t* opt)
{ 
  Reset();
  if (!fBeamDetTrack || !fBeamDetTOFDigi1 || !fBeamDetTOFDigi2)
  {
    cout  << "ERBeamDetPID: No track" << endl;
    return;
  }

  Double_t tof1, tof2, tof;
  Double_t dE1, dE2, dE;
  Double_t probability;

  Double_t beta;
  Double_t gamma;
  /*TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPID);
  if ( ! particle ) {
      cout << "ERIonGenerator: Ion " << fPID
      << " not found in database!" << endl;
      return ;
    }*/
  //Double_t mass = 26.2716160;//particle->Mass();
  Double_t p, energy;

  ERBeamDetTOFDigi* digi;

  digi = (ERBeamDetTOFDigi*)fBeamDetTOFDigi1->At(0);
  tof1 = digi->GetTime();
  dE1 = digi->Edep();
  cout << "dE1 = " << dE1 << " TOF1 = " << tof1 << endl;
  digi = (ERBeamDetTOFDigi*)fBeamDetTOFDigi2->At(0);
  tof2 = digi->GetTime();
  dE2 = digi->Edep();
  cout << "dE2 = " << dE2 << " TOF2 = " << tof2 << endl;

  dE = dE1 +dE2;
  cout << "dE = " << dE << " Gev; " << " TOF1 = " << tof1 << " ns;" << " TOF2 = " << tof2 << " ns;" << endl;
  tof = tof2 - tof1 + fOffsetTOF;
  cout << "dE = " << dE << " Gev; " << " TOF = " << tof << " ns;" << endl;

  if(tof <= fTOF1 || tof >= fTOF2 || dE <= fdE1 || dE >= fdE2){
    probability = 0;
  }
  else {
    probability = 1;
  }

  if(probability < fProbabilityThreshold) {
    std::cout << "Probability " << probability << " less then threshold " << fProbabilityThreshold << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  beta = fBeamDetSetup->DistanceBetweenTOF() * 1e-2 / (tof * 10e-9) / TMath::C();
  if(beta <=0 || beta >= 1) {
    std::cout << "Wrong beta " << beta << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  gamma= 1./TMath::Sqrt(1.-beta*beta);
  p = beta * gamma * fIonMass;

  Double_t px, py, pz;

  TVector3& trackVect = fBeamDetTrack->GetVector();
  Double_t theta = trackVect.Theta();
  Double_t phi = trackVect.Phi();
  px = p * TMath::Sin(theta) * TMath::Cos(phi);
  py = p * TMath::Sin(fBeamDetTrack->GetVector().Theta()) * TMath::Sin(fBeamDetTrack->GetVector().Phi());
  pz = p * TMath::Cos(fBeamDetTrack->GetVector().Theta());

  energy = fIonMass * gamma;
  std::cout << "PID: " << fPID << "; px: " << px << "; py: " << py << "; pz: " << pz 
            << " energy: " << energy << "; probability " << probability << std::endl;

  AddParticle(fPID, TLorentzVector(px, py, pz, energy), probability);

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetPID::Reset()
{
  if (fProjectile) {
    fProjectile->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetPID::Finish()
{   
  
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ERBeamDetParticle* ERBeamDetPID::AddParticle(Int_t pid, TLorentzVector fourMomentum, Double_t probability)
{
 fProjectile->AddParameters(pid, fourMomentum, probability); 
}
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetPID)