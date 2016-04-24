#include "ERDecay30Arto28S2p.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include "FairLogger.h"

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecay30Arto28S2p::ERDecay30Arto28S2p():
  fDirectReactionFinish(kFALSE),
  fTauCM(100.) //ps
{
  fRnd = new TRandom3();
  fPHSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* ThirdIon = new FairIon("ExpertThirdIon",16,28,16); //28S
  run->AddNewIon(ThirdIon);
}

ERDecay30Arto28S2p::~ERDecay30Arto28S2p(){

}

Bool_t ERDecay30Arto28S2p::Stepping(){
  if(!fDirectReactionFinish && gMC->TrackPid() == 1000180300){
  	fSecondIon = TDatabasePDG::Instance()->GetParticle("ExpertSecondIon");
    if ( ! fSecondIon ) {
        LOG(ERROR)  << "-W- ERDecay30Arto28S2p: Ion ExpertSecondIon not found in database!" << endl;
        return kFALSE;
    }
    fThirdIon  = TDatabasePDG::Instance()->GetParticle("ExpertThirdIon");
    if ( ! fThirdIon ) {
        LOG(ERROR) << "-W- ERDecay30Arto28S2p: Ion not ExpertThirdIon found in database!" << endl;
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
      LOG(INFO) << "Start direct reaction. Current pos: " << curPos.Z() << " [cm] " << endl;
      header->SetDirectReactionPos(curPos.Z());
      //Результирующий импульс второго иона
      TLorentzVector pSecIon(curMomentum.X(),curMomentum.Y(),curMomentum.Z(),fullEnergy);
      //Чтение из файла треков продуктов развала нестабильного иона.
      //Пока замена на ROOT класс с правильным обсчетом кинематики.
      //Массив масс продуктов распада
      Double_t masses[3];
      masses[0] = fThirdIon->Mass();
      masses[1] = 0.938; //proton mass
      masses[2] = 0.938; //proton mass
      if (!fPHSpace->SetDecay(pSecIon,3,masses)){
      	LOG(ERROR) << "ERDecay: the decay is forbidden by kinematics" <<endl;
        fDirectReactionFinish = kTRUE;
        return kFALSE;
      }
      fPHSpace->Generate();

      //Испускание третьего иона
      TLorentzVector* pThirdIon = fPHSpace->GetDecay(0);

    	Int_t thirdIonPDG = fThirdIon->PdgCode();

    	LOG(INFO) << "-I- ERDecay: Generating ion of type "
         << fThirdIon->GetName() << " (PDG code " << thirdIonPDG << ")" << endl;
    	LOG(INFO) << "    Momentum (" << pThirdIon->Px() << ", " << pThirdIon->Py() << ", " << pThirdIon->Pz()
         << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 1, thirdIonPDG,
                           pThirdIon->Px(),pThirdIon->Py(),pThirdIon->Pz(),
                           pThirdIon->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fThirdIon->Mass(), 0);
      //Испускание нейтронов
      TLorentzVector* pNeutron1 = fPHSpace->GetDecay(1);
      TLorentzVector* pNeutron2 = fPHSpace->GetDecay(2);

    	LOG(INFO) << "-I- ERGenerator: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron1->Px() << ", " <<  pNeutron1->Py() << ", " <<  pNeutron1->Pz() 
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;
      LOG(INFO) << "-I- ERGenerator: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron2->Px() << ", " <<  pNeutron2->Py() << ", " <<  pNeutron2->Pz()
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 1, 2212,
                           pNeutron1->Px(),pNeutron1->Py(),pNeutron1->Pz(),
                           pNeutron1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, 0.938, 0);
      gMC->GetStack()->PushTrack(1, 1, 2212,
                           pNeutron2->Px(),pNeutron2->Py(),pNeutron2->Pz(),
                           pNeutron2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb,  0.938, 0);
      //Испускание гаммы.
      fDirectReactionFinish = kTRUE;
      //Stop second ion
      gMC->StopTrack();
    }	
  }
  return kTRUE;
}

void ERDecay30Arto28S2p::BeginEvent(){	
  fDirectReactionFinish = kFALSE;
}

void ERDecay30Arto28S2p::FinishEvent(){

}

ClassImp(ERDecay30Arto28S2p)
