#include "ERGenerator.h"

#include <stdio.h>                      //for NULL, sprintf

#include "TDatabasePDG.h"               //for TDatabasePDG
#include "TVector3.h"
#include "TLorentzVector.h"				//for TLorentzVector
#include "TVirtualMC.h"

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERTarget.h"					//for ERTarget
#include "ERMCEventHeader.h"			//for ERMCEventHeader

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
	fRnd = new TRandom3();
	fgNIon++;
	char buffer[20];
  	//sprintf(buffer, "ExpertPrimaryIon%d", fgNIon);
  	fPrimaryIon= new FairIon("ExpertPrimaryIon", z, a, q);
  	FairRunSim* run = FairRunSim::Instance();
  	if ( ! run ) {
	    cout << "-E- ERGenerator: No FairRun instantised!" << endl;
	    Fatal("ERGenerator", "No FairRun instantised!");
	} else {
	    run->AddNewIon(fPrimaryIon);
	}
	if (z == 9 && a == 27){
		fSecondIon = new FairIon("ExpertSecondIon",8,26, 8); //26O
		run->AddNewIon(fSecondIon);
		fThirdIon = new FairIon("ExpertThirdIon",8,24, 8); //24O
		run->AddNewIon(fThirdIon);
	}
  fPHSpace = new TGenPhaseSpace();
}

ERGenerator::~ERGenerator(){

}

Bool_t ERGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
	FairRunSim* run = FairRunSim::Instance();
	ERMCEventHeader* mcheader = (ERMCEventHeader*)run->GetMCEventHeader();
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