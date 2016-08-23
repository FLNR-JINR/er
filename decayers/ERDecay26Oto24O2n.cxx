#include "ERDecay26Oto24O2n.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecay26Oto24O2n::ERDecay26Oto24O2n():
  fDirectReactionFinish(kFALSE),
  fTauCM(100.) //ps
{
  fRnd = new TRandom3();
  fPHSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* ThirdIon = new FairIon("ExpertThirdIon",8,24, 8); //24O
  run->AddNewIon(ThirdIon);
}

ERDecay26Oto24O2n::~ERDecay26Oto24O2n(){

}

Bool_t ERDecay26Oto24O2n::Stepping(){
  if(!fDirectReactionFinish && gMC->TrackPid() == 1000080260){
  	fSecondIon = TDatabasePDG::Instance()->GetParticle("ExpertSecondIon");
    if ( ! fSecondIon ) {
        std::cerr  << "-W- ERDecay26Oto24O2n: Ion ExpertSecondIon not found in database!" << endl;
        return kFALSE;
    }
    fThirdIon  = TDatabasePDG::Instance()->GetParticle("ExpertThirdIon");
    if ( ! fThirdIon ) {
        std::cerr << "-W- ERDecay26Oto24O2n: Ion not ExpertThirdIon found in database!" << endl;
        return kFALSE;
    }

    gMC->SetMaxStep(0.01);
    Int_t newTrackNb;
    //Разыгрование позиции развала нестабильного иона

    Bool_t destroied = kFALSE; //развалился/не развалился

    TLorentzVector curMomentum;
    gMC->TrackMomentum(curMomentum);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    
    Double_t momentum =  TMath::Sqrt(curMomentum.X()*curMomentum.X() + curMomentum.Y()*curMomentum.Y() + curMomentum.Z()*curMomentum.Z());
    Double_t fullEnergy = TMath::Sqrt(fSecondIon->Mass()*fSecondIon->Mass() + momentum*momentum);

    //Расчет гамма фактора
    Double_t betaCM = (fullEnergy-fSecondIon->Mass())/fullEnergy;
    Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );

    Double_t tau = fTauCM*gammaCM;

    //вероятность того, что произошел развал
    if (fRnd->Uniform() > TMath::Exp(-(gMC->TrackTime())*(1.0e09)/tau)){
      FairRunSim* run = FairRunSim::Instance();
      ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
      std::cout << "Start direct reaction. Current pos: " << curPos.Z() << " [cm] " << endl;
      header->SetDirectReactionPos(curPos.Z());
      //Результирующий импульс второго иона
      TLorentzVector pSecIon(curMomentum.X(),curMomentum.Y(),curMomentum.Z(),fullEnergy);
      //Чтение из файла треков продуктов развала нестабильного иона.
      //Пока замена на ROOT класс с правильным обсчетом кинематики.
      //Массив масс продуктов распада
      Double_t masses[3];
      masses[0] = fThirdIon->Mass();
      masses[1] = 0.939; //neutron mass
      masses[2] = 0.939; //neutron mass
      if (!fPHSpace->SetDecay(pSecIon,3,masses)){
      	std::cerr << "ERDecay: the decay is forbidden by kinematics" <<endl;
        fDirectReactionFinish = kTRUE;
        return kFALSE;
      }
      fPHSpace->Generate();

      //Испускание третьего иона
      TLorentzVector* pThirdIon = fPHSpace->GetDecay(0);

    	Int_t thirdIonPDG = fThirdIon->PdgCode();

    	std::cout << "-I- ERDecay: Generating ion of type "
         << fThirdIon->GetName() << " (PDG code " << thirdIonPDG << ")" << endl;
    	std::cout << "    Momentum (" << pThirdIon->Px() << ", " << pThirdIon->Py() << ", " << pThirdIon->Pz()
         << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 1, thirdIonPDG,
                           pThirdIon->Px(),pThirdIon->Py(),pThirdIon->Pz(),
                           pThirdIon->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fThirdIon->Mass(), 0);
      //Испускание нейтронов
      TLorentzVector* pNeutron1 = fPHSpace->GetDecay(1);
      TLorentzVector* pNeutron2 = fPHSpace->GetDecay(2);

    	std::cout << "-I- ERGenerator: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron1->Px() << ", " <<  pNeutron1->Py() << ", " <<  pNeutron1->Pz() 
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;
      std::cout << "-I- ERGenerator: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron2->Px() << ", " <<  pNeutron2->Py() << ", " <<  pNeutron2->Pz()
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 1, 2112,
                           pNeutron1->Px(),pNeutron1->Py(),pNeutron1->Pz(),
                           pNeutron1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, 0.939, 0);
      gMC->GetStack()->PushTrack(1, 1, 2112,
                           pNeutron2->Px(),pNeutron2->Py(),pNeutron2->Pz(),
                           pNeutron2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb,  0.939, 0);
      //Испускание гаммы.
      fDirectReactionFinish = kTRUE;
      //Stop second ion
      gMC->StopTrack();
    }	
  }
  return kTRUE;
}

void ERDecay26Oto24O2n::BeginEvent(){	
  fDirectReactionFinish = kFALSE;
}

void ERDecay26Oto24O2n::FinishEvent(){

}

ClassImp(ERDecay26Oto24O2n)
