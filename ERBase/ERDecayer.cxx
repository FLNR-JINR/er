#include "ERDecayer.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"               //for TDatabasePDG
#include "TMCProcess.h"

#include "FairRunSim.h"

//#include "ERTarget.h"

#include <iostream>
using namespace std;

ERDecayer::ERDecayer(){
	fTargetReactZ = 0.;
	fRnd = new TRandom3();
}

ERDecayer::~ERDecayer(){

}

void ERDecayer::Stepping(){
  Bool_t targetReactionFinish = kFALSE;
  Bool_t directReactionFinish = kFALSE;
	//Определяемся с текущим положением.
	if(TString(gMC->CurrentVolName()).Contains("target")){
		if(gMC->IsTrackEntering()){
			//Double_t targetThickness = ERTarget::Thickness();
			fTargetReactZ = fRnd->Uniform()*2.;
		}
		else{
			TLorentzVector curPos;
  		gMC->TrackPosition(curPos);
			if (curPos.Z() > fTargetReactZ){
				cout << "Start reation intarget. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
				FairRunSim* run = FairRunSim::Instance();
				//Stop first ion
				gMC->StopTrack();
				//Create new ion
        
				if (gMC->TrackPid() == 1000090270){
					fSecondIon = new FairIon("ExpertSecondIon",8,26, 8); //26O
					run->AddNewIon(fSecondIon);
					fThirdIon = new FairIon("ExpertThirdIon",8,24, 8); //24O
					run->AddNewIon(fThirdIon);
				}
				TLorentzVector curMomentum;
				gMC->TrackMomentum(curMomentum);
        
				Double_t kinEnergy = TMath::Sqrt(curMomentum.X()*curMomentum.X() + 
										curMomentum.Y()*curMomentum.Y() + curMomentum.Z()*curMomentum.Z());
    			Double_t fullEnergy = /*fSecondIon->GetMass() + */kinEnergy;
    			//Расчет гамма фактора
    			Double_t betaCM = kinEnergy/fullEnergy;
    			Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );
/*
    			TParticlePDG* thisPart =
    					TDatabasePDG::Instance()->GetParticle(fSecondIon->GetName());
			  	if ( ! thisPart ) {
				    cout << "-W- ERDecayer: Ion " << fSecondIon->GetName()
				         << " not found in database!" << endl;
				}
			  	Int_t secondaryIonPDG = thisPart->PdgCode();

			  	cout << "-I- ERDecayer: Generating ion of type "
			       << fSecondIon->GetName() << " (PDG code " << secondaryIonPDG << ")" << endl;
			  	cout << "    Momentum (" << curMomentum.X() << ", " <<  curMomentum.Y() << ", " <<  curMomentum.Z()
			       << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y()
			       << ", " << curPos.Z() << ") cm" << endl;
          */   
				Int_t newTrackNb;
        
				gMC->GetStack()->PushTrack(1, 0, 2212,
                         curMomentum.X(),curMomentum.Y(), curMomentum.Z(),
                         fullEnergy, curPos.X(), curPos.Y(), curPos.Z(),
                         gMC->TrackTime(), 0., 0., 0.,
                         kPDecay, newTrackNb,/* fSecondIon->GetMass()*/ 1., 0);
        cerr << "ERDecayer add track nb: " << newTrackNb << endl;
			}
		}
	}
if(!directReactionFinish && targetReactionFinish){
  //Разыгрование позиции развала нестабильного иона
  Double_t step = 0.05; //Шаг имитации проведения иона
  Double_t tauCM = 5.; // время распада 26O в системе ЦМ  тау= 5пс
  Bool_t destroied = kFALSE; //развалился/не развалился

  TLorentzVector curMomentum;
  gMC->TrackMomentum(curMomentum);
  TLorentzVector curPos;
  gMC->TrackPosition(curPos);

  Double_t kinEnergy = TMath::Sqrt(curMomentum.X()*curMomentum.X() + 
                    curMomentum.Y()*curMomentum.Y() + curMomentum.Z()*curMomentum.Z());
  Double_t fullEnergy = fSecondIon->GetMass() + kinEnergy;
  //Расчет гамма фактора
  Double_t betaCM = kinEnergy/fullEnergy;
  Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );

  Double_t tau = tauCM*gammaCM;

  //вероятность того, что произошел развал
  if (fRnd->Uniform() > TMath::Exp(-gMC->TrackStep()/tau)){

    //mcheader->SetDirectReactionPos(curIonPos);
    //Результирующий импульс второго иона
    TLorentzVector pSecIon(curMomentum.X(),curMomentum.Y(),curMomentum.Z(),fullEnergy);
    //Чтение из файла треков продуктов развала нестабильного иона.
    //Пока замена на ROOT класс с правильным обсчетом кинематики.
    //Массив масс продуктов распада
    Double_t masses[3];
    masses[0] = fThirdIon->GetMass();
    masses[1] = 0.939; //neutron mass
    masses[2] = 0.939; //neutron mass
    if (!fPHSpace->SetDecay(pSecIon,3,masses)){
    	cerr << "ERDecayer: the decay is forbidden by kinematics" <<endl;
    }
    fPHSpace->Generate();

    //Испускание третьего иона
    TLorentzVector* pThirdIon = fPHSpace->GetDecay(0);
    TParticlePDG* thisPart =
    		TDatabasePDG::Instance()->GetParticle(fThirdIon->GetName());
  	if ( ! thisPart ) {
      cout << "-W- FairIonGenerator: Ion " << fThirdIon->GetName()
           << " not found in database!" << endl;
    }

  	Int_t thirdIonPDG = thisPart->PdgCode();

  	cout << "-I- ERDecayer: Generating ion of type "
       << fThirdIon->GetName() << " (PDG code " << thirdIonPDG << ")" << endl;
  	cout << "    Momentum (" << pThirdIon->X() << ", " << pThirdIon->Y() << ", " << pThirdIon->Z()
       << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;


    //primGen->AddTrack(thirdIonPDG, pThirdIon->X(), pThirdIon->Y(), pThirdIon->Z(), 0., 0., curIonPos,-1,true,-9e9,20., 0.);
/*
    gMC->GetStack()->PushTrack(1, 0, thirdIonPDG,
                         pThirdIon->X(),pThirdIon->Y(),pThirdIon->Z(),
                         pThirdIon->, curPos.X(), curPos.Y(), curPos.Z(),
                         gMC->TrackTime(), 0., 0., 0.,
                         kPDecay, newTrackNb, fSecondIon->GetMass(), 0);
*/
    //Испускание нейтронов
    TLorentzVector* pNeutron1 = fPHSpace->GetDecay(1);
    TLorentzVector* pNeutron2 = fPHSpace->GetDecay(2);

    	cout << "-I- ERGenerator: Generating neutron " << endl;
    	cout << "    Momentum (" << pNeutron1->X() << ", " <<  pNeutron1->Y() << ", " <<  pNeutron1->Z()
         << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;
      cout << "-I- ERGenerator: Generating neutron " << endl;
    	cout << "    Momentum (" << pNeutron2->X() << ", " <<  pNeutron2->Y() << ", " <<  pNeutron2->Z()
         << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

    //primGen->AddTrack(2112, pNeutron1->X(),pNeutron1->Y(),pNeutron1->Z(), 0, 0, curIonPos,-1,true,-9e9,20. , 0.);
    //primGen->AddTrack(2112, pNeutron2->X(),pNeutron2->Y(),pNeutron2->Z(), 0, 0, curIonPos,-1,true,-9e9,20. , 0.);
    //Испускание гаммы.
    }
	}
}
