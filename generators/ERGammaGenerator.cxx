

#include "ERGammaGenerator.h"
#include "TMath.h"
#include "FairLogger.h"

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

using namespace std;

ERGammaGenerator::ERGammaGenerator():
fPhiMinCM(0.),
fPhiMaxCM(0.),
fThetaMinCM(0.),
fThetaMaxCM(0.),
fEBeam(0.)
{
	fRnd = new TRandom3();
}

ERGammaGenerator::~ERGammaGenerator(){

}

Bool_t ERGammaGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
	Double_t phiCM = fRnd->Uniform()*(fPhiMaxCM - fPhiMinCM) + fPhiMinCM;
	phiCM *= TMath::DegToRad();
	G4IonTable* fIonTable =  G4IonTable::GetIonTable(); 
  	G4ParticleTable* fParticleTable = G4ParticleTable::GetParticleTable();
  	fGammaPDG = fParticleTable->FindParticle("gamma")->GetPDGEncoding();
	fIonMass = fIonTable->GetIonMass(fZ,fA)/1000.;
	Double_t fullEBeam = fEBeam + fIonMass;

	Double_t cos_thetaCM = fRnd->Uniform(TMath::Cos(fThetaMaxCM), TMath::Cos(fThetaMinCM));
	Double_t sin_thetaCM = TMath::Sqrt(1-cos_thetaCM*cos_thetaCM);
	Double_t pGammaCM = fEGammaCM; 
	Double_t pxCM = pGammaCM*TMath::Cos(phiCM)*sin_thetaCM;
	Double_t pyCM = pGammaCM*TMath::Sin(phiCM)*sin_thetaCM;
	Double_t pzCM = pGammaCM*cos_thetaCM;

	Double_t pBeam = TMath::Sqrt(fullEBeam*fullEBeam - fIonMass*fIonMass);
	Double_t betaCM = pBeam/fullEBeam;
	Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );

	Double_t pxLAB = pxCM;
	Double_t pyLAB = pyCM;
	Double_t pzLAB = gammaCM * ( pzCM + betaCM * fEGammaCM);

	Double_t pLAB = TMath::Sqrt(pxLAB*pxLAB + pyLAB*pyLAB + pzLAB*pzLAB);
	primGen->AddTrack(fGammaPDG, pxLAB, pyLAB, pzLAB,0, 0, 0);

	return kTRUE;
}

void ERGammaGenerator::SetGammaCMThetaRange(Double_t thetaMin, Double_t thetaMax)
{
	fThetaMinCM = thetaMin;
	fThetaMaxCM= thetaMax;
	cerr << fThetaMinCM << " " << fThetaMaxCM << endl;
	fThetaMinCM *= TMath::DegToRad();
	fThetaMaxCM *= TMath::DegToRad();
}
