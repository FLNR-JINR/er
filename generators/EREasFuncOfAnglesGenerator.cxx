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
  fX1(0.), fY1(0.), fZ1(0.), fX2(0.), fY2(0.), fZ2(0.),
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
  fX1(0.), fY1(0.), fZ1(0.), fX2(0.), fY2(0.), fZ2(0.),
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
      fZ = gRandom->Uniform(fZ1,fZ2);
    }
//______________________________________________________________________ 

    Double_t  E, Tb, TB, A13, A14, A23, A24; // condition : m3 < m4
    Double_t A;

    E = fQ + fTa;
    A = (fma+fmA)*(fmb+fmB);

    A13 = fmA*fmb*fTa/(A*E);
    A14 = fmA*fmB*fTa/(A*E);
    A23 = fma*fmb*(1+(fmA/fma)*(fQ/E))/A;
    A24 = fma*fmB*(1+(fmA/fma)*(fQ/E))/A;


    if (A13>A24){

      theta = TMath::ASin(TMath::Sqrt(A24/A13));
      Tb = A13*E*TMath::Power((TMath::Cos(theta)-TMath::Sqrt((A24/A13)-TMath::Power(TMath::Sin(theta),2))),2);

    }

    else {

      Tb = A13*E*TMath::Power((TMath::Cos(theta)+TMath::Sqrt((A24/A13)-TMath::Power(TMath::Sin(theta),2))),2);
    }

    TB = (E - Tb)/fmB; // Kinetic energy / A 
    LOG(DEBUG) << "Tb = " << Tb << FairLogger::endl;

  //__________________________________________________________________


    

//    phi = gRandom->Uniform(fPhiMin, fPhiMax) * TMath::DegToRad();
    phi = TMath::ASin(TMath::Sin(theta)*TMath::Sqrt((fmb/fmB)*(Tb/TB)));
    LOG(DEBUG) << "theta = " << theta << FairLogger::endl;
    LOG(DEBUG) << "phi = " << phi << FairLogger::endl;





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
