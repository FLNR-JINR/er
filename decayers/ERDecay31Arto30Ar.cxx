#include "ERDecay31Arto30Ar.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecay31Arto30Ar::ERDecay31Arto30Ar():
  ERDecay("31Arto30Ar"),
  fTargetReactionFinish(kFALSE),
  fTargetReactZ(0.),
  fSecondaryIonPDG(-1)
{
  fRnd = new TRandom3();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* SecondIon = new FairIon("ExpertSecondIon",18,30, 18); //26O
  run->AddNewIon(SecondIon);
}

Bool_t ERDecay31Arto30Ar::Init(){

	fSecondIon = TDatabasePDG::Instance()->GetParticle("ExpertSecondIon");
    if ( ! fSecondIon ) {
        std::cerr  << "-W- ERDecay31Arto30Ar: Ion ExpertSecondIon not found in database!" << endl;
        return kFALSE;
    }
    return kTRUE;
}


ERDecay31Arto30Ar::~ERDecay31Arto30Ar(){

}

Bool_t ERDecay31Arto30Ar::Stepping(){
  //Определяемся с текущим положением.
  if (gMC->TrackPid() == 1000180310 && !fTargetReactionFinish && TString(gMC->CurrentVolName()).Contains("target")){
    gMC->SetMaxStep(0.01);
	TLorentzVector curPos;
	gMC->TrackPosition(curPos);
	if (curPos.Z() > fTargetReactZ){
		std::cout << "Start reation in target. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
		FairRunSim* run = FairRunSim::Instance();
		//Create new ion
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

	  	std::cout << "-I- ERDecay31Arto30Ar: Generating ion of type "
	       << fSecondIon->GetName() << " (PDG code " << fSecondaryIonPDG << ")" << endl;
	  	std::cout << "    Momentum (" << curMomentum.X() << ", " <<  curMomentum.Y() << ", " <<  curMomentum.Z()
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
  return kTRUE;
}

void ERDecay31Arto30Ar::BeginEvent(){
  fTargetReactionFinish = kFALSE;
  //Double_t targetThickness = ERTarget::Thickness();
  fTargetReactZ = fRnd->Uniform()*2.;
  FairRunSim* run = FairRunSim::Instance();
  ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
  header->SetTargetReactionPos(fTargetReactZ);
  fSecondaryIonPDG = -1;
}

void ERDecay31Arto30Ar::FinishEvent(){

}

ClassImp(ERDecay31Arto30Ar)
