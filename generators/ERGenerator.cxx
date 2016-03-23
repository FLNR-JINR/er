#include "ERGenerator.h"

#include <stdio.h>                      //for NULL, sprintf

#include "TDatabasePDG.h"               //for TDatabasePDG

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERTarget.h"

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
fVx(vx), fVy(vy), fVz(vz),fIon(NULL)
{
	fRnd = new TRandom3();
	fgNIon++;
	char buffer[20];
  	sprintf(buffer, "FairIon%d", fgNIon);
  	fIon= new FairIon(buffer, z, a, q);
  	FairRunSim* run = FairRunSim::Instance();
  	if ( ! run ) {
	    cout << "-E- ERGenerator: No FairRun instantised!" << endl;
	    Fatal("ERGenerator", "No FairRun instantised!");
	} else {
	    run->AddNewIon(fIon);
	}
}

ERGenerator::~ERGenerator(){

}

Bool_t ERGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
	//Добавление первичного иона в стек.
	TParticlePDG* thisPart =
    TDatabasePDG::Instance()->GetParticle(fIon->GetName());
  	if ( ! thisPart ) {
	    cout << "-W- FairIonGenerator: Ion " << fIon->GetName()
	         << " not found in database!" << endl;
	    return kFALSE;
	}

  	int pdgType = thisPart->PdgCode();

  	cout << "-I- ERGenerator: Generating ion of type "
       << fIon->GetName() << " (PDG code " << pdgType << ")" << endl;
  	cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") Gev from vertex (" << fVx << ", " << fVy
       << ", " << fVz << ") cm" << endl;

    primGen->AddTrack(pdgType, fPx, fPy, fPz, fVx, fVy, fVz);

	//Разыгрование позиции взаимодействия в мишени

	//Разыгрование позиции развала нестабильного иона

	//Чтение из файла треков продуктов развала нестабильного иона.

	//Испускание гаммы.

	return kTRUE;
}


ClassImp(ERGenerator)