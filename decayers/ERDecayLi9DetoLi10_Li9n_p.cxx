#include "ERDecayLi9DetoLi10_Li9n_p.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMCProcess.h"
#include "TMath.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

//#include "ERTarget.h"
#include "ERLi10MCEventHeader.h"      //for ERMCEventHeader

ERDecayLi9DetoLi10_Li9n_p::ERDecayLi9DetoLi10_Li9n_p():
  ERDecay("Li9DetoLi10_Li9n_p"),
  fTargetReactionFinish(kFALSE),
  fTargetReactZ(0.),
  fLi9(NULL),
  fLi10(NULL),
  fH2(NULL),
  fn(NULL),
  fp(NULL)
{
  fRnd = new TRandom3();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* Li10Ion = new FairIon("Li10",3,10,3);
  run->AddNewIon(Li10Ion);
  FairIon* Li9Ion = new FairIon("Li9",3,9,3);
  run->AddNewIon(Li9Ion);
}

Bool_t ERDecayLi9DetoLi10_Li9n_p::Init(){

    fLi10 = TDatabasePDG::Instance()->GetParticle("Li10");
    if ( ! fLi10 ) {
        std::cerr  << "-W- ERDecayLi9DetoLi10_Li9n_p: Ion Li10 not found in database!" << endl;
        return kFALSE;
    }

    fLi9 = TDatabasePDG::Instance()->GetParticle("Li9");
    if ( ! fLi9 ) {
        std::cerr  << "-W- ERDecayLi9DetoLi10_Li9n_p: Ion Li9 not found in database!" << endl;
        return kFALSE;
    }
    
    fH2 = TDatabasePDG::Instance()->GetParticle("Deuteron");
    if ( ! fH2 ) {
        std::cerr  << "-W- ERDecayLi9DetoLi10_Li9n_p: Ion Deuteron not found in database!" << endl;
        return kFALSE;
    }

    fn = TDatabasePDG::Instance()->GetParticle("neutron");
    if ( ! fn ) {
        std::cerr  << "-W- ERDecayLi9DetoLi10_Li9n_p: Particle neutron not found in database!" << endl;
        return kFALSE;
    }

    fp = TDatabasePDG::Instance()->GetParticle("proton");
    if ( ! fp ) {
        std::cerr  << "-W- ERDecayLi9DetoLi10_Li9n_p: Particle proton not found in database!" << endl;
        return kFALSE;
    }

  return kTRUE;
}


ERDecayLi9DetoLi10_Li9n_p::~ERDecayLi9DetoLi10_Li9n_p(){

}

Bool_t ERDecayLi9DetoLi10_Li9n_p::Stepping(){
  //Определяемся с текущим положением.
  if (gMC->TrackPid() == 1000030090 && !fTargetReactionFinish && TString(gMC->CurrentVolName()).Contains("target")){
    gMC->SetMaxStep(0.001);
	TLorentzVector curPos;
	gMC->TrackPosition(curPos);
	//std::cerr << curPos.Z() << " "<<  fTargetReactZ << std::endl;
	if (curPos.Z() > fTargetReactZ){
		std::cout << "Start reation in target. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
		FairRunSim* run = FairRunSim::Instance();
  		ERLi10MCEventHeader* header = (ERLi10MCEventHeader*)run->GetMCEventHeader();
  		header->SetReactionTime(gMC->TrackTime() * 1.0e09);

	    const double E10Li=600*10e-6; //600 KeV
	    const double mass9Li=fLi9->Mass();
	    const double mass1H=fp->Mass();
	    const double mass2H=fH2->Mass();
	    const double massn=fn->Mass();
	    const double mass10Li=fLi10->Mass();   ///mass9Li+ massn+E10Li; // ??
	    double beam_total_energy = gMC->Etot();
	    double beam_T=beam_total_energy - mass9Li;
	    const double pin9Li = sqrt(beam_T*beam_T+2*mass9Li*beam_T);
    	double amu=931.494;
    	TLorentzVector Vin9Li(0,0,pin9Li,beam_T+mass9Li);
    	TLorentzVector V2H (0,0,0,mass2H);
    	TLorentzVector Vofcm = Vin9Li+V2H;
    	TVector3 boost_vector = Vofcm.BoostVector();
    	Vin9Li.Boost(-boost_vector);
    	V2H.Boost(-boost_vector);
    	double Ecm = Vin9Li.E()+V2H.E();
	    double theta_cm = TMath::ACos(fRnd->Uniform(0.99,1.));
	    double phi_cm = fRnd->Uniform(0.0,2*TMath::Pi());

	    const double Ecm10Li=0.5*(Ecm*Ecm + mass10Li*mass10Li-mass1H*mass1H)/Ecm;
	    const double Ecmp=0.5*(Ecm*Ecm-mass10Li*mass10Li+mass1H*mass1H)/Ecm;
	    const double pcm10Li = sqrt(Ecm10Li*Ecm10Li-mass10Li*mass10Li);
	    TVector3 pcm = Vin9Li.Vect();
	    //pcm.rotate(0.,theta_cm,0);
	    //pcm.rotate(phi_cm,0,0);
	    pcm.SetTheta(theta_cm);
	    pcm.SetPhi(phi_cm);
	    pcm.SetMag(pcm10Li);
	    TLorentzVector V10Li=TLorentzVector(pcm,Ecm10Li);
	    TLorentzVector V1H=TLorentzVector(-pcm,Ecmp);
	    V10Li.Boost(boost_vector);
	    V1H.Boost(boost_vector);

	    //10Li decay
	    double theta_p1 = TMath::ACos(fRnd->Uniform(-1.00,1.00));
	    double phi_p1 = fRnd->Uniform(0.0,2*TMath::Pi());
	    
	    TVector3 boost_vector1 = V10Li.BoostVector();


	    double ecm1 = mass10Li;

	    const double Ecm9Li= 0.5*(ecm1*ecm1+mass9Li*mass9Li-massn*massn)/ecm1;

	    const double pcm9Li = sqrt(Ecm9Li*Ecm9Li-mass9Li*mass9Li);

	    TVector3 pcm1(pcm9Li*TMath::Sin(theta_p1)*TMath::Cos(phi_p1),
			       pcm9Li*TMath::Sin(theta_p1)*TMath::Sin(phi_p1),
			       pcm9Li*TMath::Cos(theta_p1));

	    TLorentzVector V9Li(pcm1,mass9Li);

	    V9Li.Boost(boost_vector1);

	    TLorentzVector Vn(-pcm1,massn);
	    Vn.Boost(boost_vector1);

	    Int_t newTrackNb;
	    gMC->GetStack()->PushTrack(1, 0, 2212,
                           V1H.Px(),V1H.Py(),V1H.Pz(),
                           V1H.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb,mass1H, 0);

	    gMC->GetStack()->PushTrack(1, 0, 2112,
                           Vn.Px(),Vn.Py(),Vn.Pz(),
                           Vn.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, massn, 0);
/*
	    gMC->GetStack()->PushTrack(1, 0, fLi9->PdgCode(),
                           V9Li.Px(),V9Li.Py(),V9Li.Pz(),
                           V9Li.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, massn, 0);
*/
	  	gMC->StopTrack();



	  	fTargetReactionFinish = kTRUE;
	  	gMC->SetMaxStep(100.);
	}
  }
  return kTRUE;
}

void ERDecayLi9DetoLi10_Li9n_p::BeginEvent(){
  fTargetReactionFinish = kFALSE;
  //Double_t targetThickness = ERTarget::Thickness();
  fTargetReactZ = fRnd->Uniform()*(.025+0.006);
  FairRunSim* run = FairRunSim::Instance();
  ERLi10MCEventHeader* header = (ERLi10MCEventHeader*)run->GetMCEventHeader();
  header->SetReactionPos(fTargetReactZ);
  fSecondaryIonPDG = -1;
}

void ERDecayLi9DetoLi10_Li9n_p::FinishEvent(){

}

ClassImp(ERDecayLi9DetoLi10_Li9n_p)
