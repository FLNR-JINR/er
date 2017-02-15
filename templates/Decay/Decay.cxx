#include "Decay.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TGenPhaseSpace.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

Decay::Decay():
  fReactionFinish(kFALSE)
{
  fRnd = new TRandom3();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* PrimaryIon = new FairIon("PrimaryIon",8,26, 8); //24O
  FairIon* SecondaryIon = new FairIon("SecondaryIon",8,24, 8); //22O
  run->AddNewIon(SecondaryIon);
}

Decay::~Decay(){

}

Bool_t Decay::Stepping(){
  //cerr << gMC->TrackPid() << endl;
  if(!fReactionFinish && gMC->TrackPid() == 1000080260 && TString(gMC->CurrentVolName()).Contains("target")){
    gMC->SetMaxStep(0.01);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    //cerr << curPos.Z() << " " << fReactionPos << endl;
    if (curPos.Z() > fReactionPos){
      gMC->SetMaxStep(100.);
      TGenPhaseSpace* PHSpace = new TGenPhaseSpace();
    	fPrimaryIon = TDatabasePDG::Instance()->GetParticle("PrimaryIon");
      if ( ! fPrimaryIon ) {
          std::cerr  << "-W- Decay: Ion PrimaryIon not found in database!" << endl;
          return kFALSE;
      }
      fSecondaryIon  = TDatabasePDG::Instance()->GetParticle("SecondaryIon");
      if ( ! fSecondaryIon ) {
          std::cerr << "-W- Decay: Ion not SecondaryIon found in database!" << endl;
          return kFALSE;
      }
      Int_t newTrackNb;
      TLorentzVector curMomentum;
      gMC->TrackMomentum(curMomentum);
      
      Double_t momentum =  TMath::Sqrt(curMomentum.X()*curMomentum.X() + curMomentum.Y()*curMomentum.Y() + curMomentum.Z()*curMomentum.Z());
      Double_t fullEnergy = TMath::Sqrt(fPrimaryIon->Mass()*fPrimaryIon->Mass() + momentum*momentum);

      TLorentzVector pPrimaryIon(curMomentum.X(),curMomentum.Y(),curMomentum.Z(),fullEnergy);

      //Пока замена на ROOT класс с правильным обсчетом кинематики.
      //Массив масс продуктов распада
      Double_t masses[3];
      masses[0] = fSecondaryIon->Mass();
      masses[1] = 0.939; //neutron mass
      masses[2] = 0.939; //neutron mass

      if (!PHSpace->SetDecay(pPrimaryIon,3,masses)){
      	std::cerr << "ERDecay: the decay is forbidden by kinematics" <<endl;
        fReactionFinish = kTRUE;
        return kFALSE;
      }
      PHSpace->Generate();

      //Испускание третьего иона
      TLorentzVector* pSecondIon = PHSpace->GetDecay(0);

    	std::cout << "-I- ERDecay: Generating ion of type "
         << fSecondaryIon->GetName() << " (PDG code " <<  fSecondaryIon->PdgCode() << ")" << endl;
    	std::cout << "    Momentum (" << pSecondIon->Px() << ", " << pSecondIon->Py() << ", " << pSecondIon ->Pz()
         << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 0,  fSecondaryIon->PdgCode(),
                           pSecondIon->Px(),pSecondIon->Py(),pSecondIon->Pz(),
                           pSecondIon->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fSecondaryIon->Mass(), 0);
      //Испускание нейтронов
      TLorentzVector* pNeutron1 = PHSpace->GetDecay(1);
      TLorentzVector* pNeutron2 = PHSpace->GetDecay(2);

    	std::cout << "-I- ERDecay: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron1->Px() << ", " <<  pNeutron1->Py() << ", " <<  pNeutron1->Pz() 
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;
      std::cout << "-I- ERDecay: Generating neutron " << endl
    	          << "    Momentum (" << pNeutron2->Px() << ", " <<  pNeutron2->Py() << ", " <<  pNeutron2->Pz()
                << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y() << ", " << curPos.Z() << ") cm" << endl;

      gMC->GetStack()->PushTrack(1, 0, 2112,
                           pNeutron1->Px(),pNeutron1->Py(),pNeutron1->Pz(),
                           pNeutron1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, 0.939, 0);
      gMC->GetStack()->PushTrack(1, 0, 2112,
                           pNeutron2->Px(),pNeutron2->Py(),pNeutron2->Pz(),
                           pNeutron2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb,  0.939, 0);
      //Испускание гаммы.
      fReactionFinish = kTRUE;
      //Stop second ion
      gMC->StopTrack();
    }
  }
  return kTRUE;
}

void Decay::BeginEvent(){	
  fReactionFinish = kFALSE;
  fReactionPos = 0;
}

void Decay::FinishEvent(){

}

ClassImp(Decay)
