#include "ERDecayer.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include "FairLogger.h"

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecayer::ERDecayer():
  fTargetReactionFinish(kFALSE),
  fDirectReactionFinish(kFALSE),
  fTargetReactZ(0.),
  fSecondaryIonPDG(-1),
  fTauCM(100.) //ps
{
	fRnd = new TRandom3();
  fPHSpace = new TGenPhaseSpace();
}

ERDecayer::~ERDecayer(){

}

Bool_t ERDecayer::Stepping(){
	//Определяемся с текущим положением.
  if (gMC->TrackPid() == 1000090270 && !fTargetReactionFinish && TString(gMC->CurrentVolName()).Contains("target")){
    gMC->SetMaxStep(0.01);
		TLorentzVector curPos;
		gMC->TrackPosition(curPos);
		if (curPos.Z() > fTargetReactZ){
			LOG(INFO) << "Start reation in target. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
			FairRunSim* run = FairRunSim::Instance();
			//Create new ion
			if (gMC->TrackPid() == 1000090270){
        fSecondIon = TDatabasePDG::Instance()->GetParticle("ExpertSecondIon");
        if ( ! fSecondIon ) {
            LOG(ERROR)  << "-W- ERDecayer: Ion ExpertSecondIon not found in database!" << endl;
            return kFALSE;
        }
        fThirdIon  = TDatabasePDG::Instance()->GetParticle("ExpertThirdIon");
        if ( ! fThirdIon ) {
            LOG(ERROR) << "-W- FairIonGenerator: Ion not ExpertThirdIon found in database!" << endl;
            return kFALSE;
        }
			}
			TLorentzVector curMomentum;
			gMC->TrackMomentum(curMomentum);
      
      //Импульс первого иона в момент распада
      Double_t momentum =  TMath::Sqrt(curMomentum.X()*curMomentum.X() + curMomentum.Y()*curMomentum.Y() + curMomentum.Z()*curMomentum.Z());
      //Импульс второго иона
      momentum *= fSecondIon->Mass()/gMC->TrackMass();

			Double_t fullEnergy = TMath::Sqrt(fSecondIon->Mass()*fSecondIon->Mass() + momentum*momentum);
			//Расчет гамма фактора
			Double_t betaCM = (fullEnergy-fSecondIon->Mass())/fullEnergy;
			Double_t gammaCM = TMath::Sqrt( 1. / ( 1. - betaCM*betaCM) );

			TParticlePDG* thisPart =
					TDatabasePDG::Instance()->GetParticle(fSecondIon->GetName());
		  
      fSecondaryIonPDG = fSecondIon->PdgCode();

	  	LOG(INFO) << "-I- ERDecayer: Generating ion of type "
	       << fSecondIon->GetName() << " (PDG code " << fSecondaryIonPDG << ")" << endl;
	  	LOG(INFO) << "    Momentum (" << curMomentum.X() << ", " <<  curMomentum.Y() << ", " <<  curMomentum.Z()
	       << ") Gev from vertex (" << curPos.X() << ", " << curPos.Y()
	       << ", " << curPos.Z() << ") cm" << endl;
        
			Int_t newTrackNb;
      
			gMC->GetStack()->PushTrack(1, 0, fSecondaryIonPDG,
                       curMomentum.X(),curMomentum.Y(), curMomentum.Z(),
                       fullEnergy, curPos.X(), curPos.Y(), curPos.Z(),
                       gMC->TrackTime(), 0., 0., 0.,
                       kPDecay, newTrackNb, fSecondIon->Mass(), 0);
      //Stop first ion
      gMC->StopTrack();
      fTargetReactionFinish = kTRUE;
		}
  }
  if(!fDirectReactionFinish && fTargetReactionFinish && gMC->TrackPid() == fSecondaryIonPDG){
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
    // cerr << (Double_t) TMath::Exp(-(gMC->TrackTime()-fPreviouseStepTime)*(1.0e09)/tau)<< endl;
    if (fRnd->Uniform() > TMath::Exp(-(gMC->TrackTime())*(1.0e09)/tau)){
      FairRunSim* run = FairRunSim::Instance();
      ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
      curPos.SetZ(curPos.Z() + 1.5);
      LOG(INFO) << "Start direct reaction. Current pos: " << curPos.Z() << " [cm] " << endl;
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
      	LOG(ERROR) << "ERDecayer: the decay is forbidden by kinematics" <<endl;
        return kFALSE;
      }
      fPHSpace->Generate();

      //Испускание третьего иона
      TLorentzVector* pThirdIon = fPHSpace->GetDecay(0);

    	Int_t thirdIonPDG = fThirdIon->PdgCode();

    	LOG(INFO) << "-I- ERDecayer: Generating ion of type "
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

void ERDecayer::BeginEvent(){
  fTargetReactionFinish = kFALSE;
  fDirectReactionFinish = kFALSE;
  //Double_t targetThickness = ERTarget::Thickness();
  fTargetReactZ = fRnd->Uniform()*2.;
  FairRunSim* run = FairRunSim::Instance();
  ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
  header->SetTargetReactionPos(fTargetReactZ);
  fSecondaryIonPDG = -1;
}

void ERDecayer::FinishEvent(){

}
