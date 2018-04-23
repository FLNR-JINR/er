#include "EREasFuncOfAnglesGenerator.h"

// ROOT
#include <TRandom.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <TMath.h>
#include <TH1D.h>

// FairRoot
#include <FairPrimaryGenerator.h>
#include <FairLogger.h>

EREasFuncOfAnglesGenerator::EREasFuncOfAnglesGenerator() :
	FairBoxGenerator(),
  fPDGType(2112),
  fMult(1),
  fPhiMin(0.),
  fPhiMax(360.),
  fThetaMin(0.),
  fThetaMax(180.),
  fThetaRangeIsSet(kFALSE),
  fCosThetaIsSet(kFALSE),
  fX(0.), fY(0.), fZ(0.),
  fX1(0.), fY1(0.), fX2(0.), fY2(0.),
  fPointVtxIsSet(kFALSE),
  fBoxVtxIsSet(kFALSE)
{
  LOG(DEBUG) << "Wrong constructor." << FairLogger::endl;
}

EREasFuncOfAnglesGenerator::EREasFuncOfAnglesGenerator(Int_t pdgid, Int_t mult) :
	FairBoxGenerator(pdgid, mult),
  fPDGType(pdgid),
  fMult(mult),
  fPhiMin(0.),
  fPhiMax(360.),
  fThetaMin(0.),
  fThetaMax(180.),
  fThetaRangeIsSet(kFALSE),
  fCosThetaIsSet(kFALSE),
  fX(0.), fY(0.), fZ(0.),
  fX1(0.), fY1(0.), fX2(0.), fY2(0.),
  fPointVtxIsSet(kFALSE),
  fBoxVtxIsSet(kFALSE)
{
  LOG(DEBUG) << "Correct constructor." << FairLogger::endl;
}

//TODO copy constructor

//TODO operator=

EREasFuncOfAnglesGenerator::~EREasFuncOfAnglesGenerator()
{
}

Bool_t EREasFuncOfAnglesGenerator::Init()
{
  // Initialize generator

  // Check for particle type
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPDGType);

  if (! particle) {
    Fatal("FairBoxGenerator","PDG code %d not defined.",fPDGType);
  } else {
    fPDGMass = particle->Mass();
  }

  if (fPhiMax-fPhiMin > 360.) {
    Fatal("Init()","FairBoxGenerator: phi range is too wide: %f<phi<%f",
          fPhiMin,fPhiMax);
  }

  if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","FairBoxGenerator: Cannot set point and box vertices simultaneously");
  }

  return kTRUE;
}

Bool_t EREasFuncOfAnglesGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  // Direction
  Double32_t phi=0., theta=0.;

  // Generate particles
  for (Int_t k = 0; k < fMult; k++)
  {
    phi = gRandom->Uniform(fPhiMin, fPhiMax) * TMath::DegToRad();

    if (fThetaRangeIsSet) {
      if (fCosThetaIsSet)
        theta = acos(gRandom->Uniform(TMath::Cos(fThetaMin* TMath::DegToRad()),
                                      TMath::Cos(fThetaMax* TMath::DegToRad())));
      else {
        theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();
      }
    }

    if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
    }

    Double_t A = fma*fmb*fTa / ((fmb+fmB)*(fmb+fmB));
    //LOG(DEBUG) << "A = " << A << FairLogger::endl;
    Double_t C = (fmb+fmB) * ((fmB-fma)*fTa+fmB*fQ) / (fma*fmb*fTa);
    //LOG(DEBUG) << "C = " << C << FairLogger::endl;
    Double_t B = TMath::Cos(theta) + TMath::Sqrt(TMath::Cos(theta)*TMath::Cos(theta) + C);
    //LOG(DEBUG) << "B2 = " << B2 << FairLogger::endl;
    Double_t Tb = A * B * B;
    LOG(DEBUG) << "Tb = " << Tb << FairLogger::endl;

    Double_t pabs = TMath::Sqrt(Tb*Tb + 2.*Tb*fPDGMass);

    Double_t pz = pabs*TMath::Cos(theta);
    Double_t pt = pabs*TMath::Sin(theta);
    Double_t px = pt*TMath::Cos(phi);
    Double_t py = pt*TMath::Sin(phi);

    primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);
  }
  return kTRUE;

}

ClassImp(EREasFuncOfAnglesGenerator)
