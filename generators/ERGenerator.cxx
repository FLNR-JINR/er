#include "ERGenerator.h"

#include <stdio.h>                      //for NULL, sprintf

#include "TDatabasePDG.h"               //for TDatabasePDG
#include "TVector3.h"
#include "TLorentzVector.h"				//for TLorentzVector

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

  	int pdgType = thisPart->PdgCode();

  	cout << "-I- ERGenerator: Generating ion of type "
       << fPrimaryIon->GetName() << " (PDG code " << pdgType << ")" << endl;
  	cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") Gev from vertex (" << fVx << ", " << fVy
       << ", " << fVz << ") cm" << endl;

    primGen->AddTrack(pdgType, fPx, fPy, fPz, fVx, fVy, fVz);

	//Разыгрование позиции взаимодействия в мишени
    Double_t targetThickness = ERTarget::Thickness();
    Double_t reactZ = fRnd->Uniform()*targetThickness;
    mcheader->SetTargetReactionPos(reactZ);
    //Расчет результирующей энергии
    Double_t kinEnergy = TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz) - 1. * reactZ/targetThickness;
    Double_t fullEnergy = fSecondIon->GetMass() + kinEnergy;
    //Расчет гамма фактора
    Double_t betaCM = kinEnergy/fullEnergy;
    Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );
    //Испускание второго иона
    thisPart =
    		TDatabasePDG::Instance()->GetParticle(fSecondIon->GetName());
  	if ( ! thisPart ) {
	    cout << "-W- FairIonGenerator: Ion " << fSecondIon->GetName()
	         << " not found in database!" << endl;
	    return kFALSE;
	}

  	pdgType = thisPart->PdgCode();

  	cout << "-I- ERGenerator: Generating ion of type "
       << fPrimaryIon->GetName() << " (PDG code " << pdgType << ")" << endl;
  	cout << "    Momentum (" << 0 << ", " << 0 << ", " << kinEnergy
       << ") Gev from vertex (" << 0. << ", " << 0.
       << ", " << reactZ << ") cm" << endl;
	primGen->AddTrack(pdgType, 0, 0, kinEnergy, 0, 0, reactZ,-1,true,-9e9,10. /*ps*/, 0.);
	//Разыгрование позиции развала нестабильного иона
    Double_t step = 0.05; //Шаг имитации проведения иона
    Double_t tauCM = 5.; // время распада 26O в системе ЦМ  тау= 5пс
    Bool_t destroied = kFALSE; //развалился/не развалился
    Double_t curIonPos = reactZ;

    while(1){
    	Double_t tau = tauCM*gammaCM;
    	//вероятность того, что произошел развал
    	if (fRnd->Uniform() > TMath::Exp(-step/tau)){
			destroied = kTRUE;
    		break;
    	}
    	curIonPos += step;
    	//Пересчет гамма фактора в связи с изменением кинетической энергии
    	if (curIonPos < targetThickness){
    		kinEnergy -= 1. * step/targetThickness;
    		fullEnergy = fSecondIon->GetMass() + kinEnergy;
    		betaCM = kinEnergy/fullEnergy;
    		gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );
    	}
    	if (kinEnergy <= 0.){ //Ион остановился не распавшись
    		break;
    	}
    }
    if (!destroied){
    	cerr << "ERGenerator: Второй ион остановился не распавшись"<< endl;
    	return kTRUE;
    }

    mcheader->SetDirectReactionPos(curIonPos);
    //Результирующий импульс второго иона
    TLorentzVector pSecIon(0,0,kinEnergy,fullEnergy);
	//Чтение из файла треков продуктов развала нестабильного иона.
    //Пока замена на ROOT класс с правильным обсчетом кинематики.
    //Массив масс продуктов распада
    Double_t masses[3];
    masses[0] = fThirdIon->GetMass();
    masses[1] = 0.939; //neutron mass
    masses[2] = 0.939; //neutron mass
    if (!fPHSpace->SetDecay(pSecIon,3,masses)){
    	cerr << "ERGenerator: the decay is forbidden by kinematics" <<endl;
    	return kTRUE;
    }
    fPHSpace->Generate();

    //Испускание третьего иона
    TLorentzVector* pThirdIon = fPHSpace->GetDecay(0);
    
    thisPart =
    		TDatabasePDG::Instance()->GetParticle(fThirdIon->GetName());
  	if ( ! thisPart ) {
	    cout << "-W- FairIonGenerator: Ion " << fThirdIon->GetName()
	         << " not found in database!" << endl;
	    return kFALSE;
	}

  	pdgType = thisPart->PdgCode();

  	cout << "-I- ERGenerator: Generating ion of type "
       << fPrimaryIon->GetName() << " (PDG code " << pdgType << ")" << endl;
  	cout << "    Momentum (" << pThirdIon->X() << ", " << pThirdIon->Y() << ", " << pThirdIon->Z()
       << ") Gev from vertex (" << 0. << ", " << 0.
       << ", " << curIonPos << ") cm" << endl;
	

	primGen->AddTrack(pdgType, pThirdIon->X(), pThirdIon->Y(), pThirdIon->Z(), 0., 0., curIonPos,-1,true,-9e9,20./*ps*/, 0.);
	
	//Испускание нейтронов
	TLorentzVector* pNeutron1 = fPHSpace->GetDecay(1);
	TLorentzVector* pNeutron2 = fPHSpace->GetDecay(2);

  	cout << "-I- ERGenerator: Generating neutron " << endl;
  	cout << "    Momentum (" << pNeutron1->X() << ", " <<  pNeutron1->Y() << ", " <<  pNeutron1->Z()
       << ") Gev from vertex (" << 0. << ", " << 0.
       << ", " << curIonPos << ") cm" << endl;
	cout << "-I- ERGenerator: Generating neutron " << endl;
  	cout << "    Momentum (" << pNeutron2->X() << ", " <<  pNeutron2->Y() << ", " <<  pNeutron2->Z()
       << ") Gev from vertex (" << 0. << ", " << 0.
       << ", " << curIonPos << ") cm" << endl;
	
	primGen->AddTrack(2112, pNeutron1->X(),pNeutron1->Y(),pNeutron1->Z(), 0, 0, curIonPos,-1,true,-9e9,20. /*ps*/, 0.);
	primGen->AddTrack(2112, pNeutron2->X(),pNeutron2->Y(),pNeutron2->Z(), 0, 0, curIonPos,-1,true,-9e9,20. /*ps*/, 0.);
	//Испускание гаммы.

	return kTRUE;
}


ClassImp(ERGenerator)