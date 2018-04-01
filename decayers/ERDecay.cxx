#include "ERDecay.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include "FairLogger.h"
#include<iostream>

using namespace std;

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERDecay::ERDecay(TString name):
fName(name),
fRnd(new TRandom3()),
fDecayPosZ(99999999.),
fDecayFinish(kFALSE),
fUniform(kFALSE),
fExponential(kFALSE),
fVolumeName(""),
fStep(0.001),
fTargetMass(-1)
{
	fRnd = new TRandom3();
}

ERDecay::~ERDecay(){
}

void ERDecay::SetInputIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  fInputIonName = fName + TString("_InputIon");
  FairIon* ion = new FairIon(fInputIonName,A,Z,Q);
  run->AddNewIon(ion);
}

void ERDecay::SetUniformPos(Double_t a, Double_t b){
  fUniform = kTRUE;
  fUniformA = a;
  fUniformB = b;
}

void ERDecay::SetExponentialPos(Double_t start, Double_t tau){
  fExponential = kTRUE;
  fExponentialStart = start;
  fExponentialTau = tau;
}

void ERDecay::BeginEvent(){
  fDecayFinish = kFALSE;
  if (fUniform){
    fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  }
  if (fExponential){
    fDecayPosZ = fExponentialStart + fRnd->Exp(fExponentialTau);
  }
}

void ERDecay::FinishEvent(){
}

Bool_t ERDecay::Init(){
  if (fInputIonName == ""){
    LOG(FATAL) << "Input ion not defined!" << FairLogger::endl;
    return kFALSE;
  }

  fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIonName);
  if ( ! fInputIonPDG ) {
    LOG(FATAL) << "Input ion not found in pdg database!" << endl;
    return kFALSE;
  }

  return kTRUE;
}

void ERDecay::AddParticleToStack(Int_t pdg, TLorentzVector pos, TLorentzVector state){
	Int_t newTrackNb;
	gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), pdg,
	                   state.Px(),state.Py(),state.Pz(),state.E(),
	                   pos.X(), pos.Y(), pos.Z(),
	                   gMC->TrackTime(), 0., 0., 0.,
	                   kPDecay, newTrackNb, fInputIonPDG->Mass(),0);
	LOG(INFO) << "ERDecay: Added output particle with ID = " << newTrackNb << " PDG = " << pdg
		 << "; pos=(" << pos.X() << "," << pos.Y() << "," << pos.Z() 
		 << "); mom=(" << state.Px() << "," << state.Py() << "," << state.Pz() << ")" << FairLogger::endl;
}

ClassImp(ERDecay)
