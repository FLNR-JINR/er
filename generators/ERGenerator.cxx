#include "ERGenerator.h"

#include <stdio.h>                      //for NULL, sprintf

#include "TDatabasePDG.h"               //for TDatabasePDG
#include "TVector3.h"
#include "TLorentzVector.h"				//for TLorentzVector
#include "TVirtualMC.h"

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

#include "FairRunSim.h"
#include<iostream>

#include "ERTarget.h"					//for ERTarget


using namespace std;

// -----   Initialsisation of static variables   --------------------------
Int_t ERGenerator::fgNIon = 0;
// ------------------------------------------------------------------------

ERGenerator::ERGenerator():
fPhiMinCM(0.),
fPhiMaxCM(0.),
fThetaMinCM(0.),
fThetaMaxCM(0.),
fEBeam(0.)
{
	fRnd = new TRandom3();
}

ERGenerator::ERGenerator(Int_t z, Int_t a, Int_t q,
              Double_t px, Double_t py, Double_t pz,
              Double_t vx, Double_t vy, Double_t vz):
fPx(Double_t(a)*px), fPy(Double_t(a)*py), fPz(Double_t(a)*pz),
fVx(vx), fVy(vy), fVz(vz),fPrimaryIon(NULL), fSecondIon(NULL), fThirdIon(NULL)
{
  fPrimaryIon= new FairIon("ExpertPrimaryIon", z, a, q);
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) {
	    cout << "-E- ERGenerator: No FairRun instantised!" << endl;
	    Fatal("ERGenerator", "No FairRun instantised!");
	} else {
	    run->AddNewIon(fPrimaryIon);
	}
}

ERGenerator::~ERGenerator(){

}

Bool_t ERGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
	//Добавление первичного иона в стек.
	TParticlePDG* thisPart =
    		TDatabasePDG::Instance()->GetParticle(fPrimaryIon->GetName());
  if ( ! thisPart ) {
	    cout << "-W- FairIonGenerator: Ion " << fPrimaryIon->GetName()
	         << " not found in database!" << endl;
	    return kFALSE;
	}

	int primaryIonPDG = thisPart->PdgCode();

	cout << "-I- ERGenerator: Generating ion of type "
     << fPrimaryIon->GetName() << " (PDG code " << primaryIonPDG << ")" << endl;
	cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
     << ") Gev from vertex (" << fVx << ", " << fVy
     << ", " << fVz << ") cm" << endl;

  primGen->AddTrack(primaryIonPDG, fPx, fPy, fPz, fVx, fVy, fVz);
	return kTRUE;
}


ClassImp(ERGenerator)