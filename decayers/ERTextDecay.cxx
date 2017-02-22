#include "ERTextDecay.h"

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include<iostream>

using namespace std;

#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERTextDecay::ERTextDecay(TString name):
  ERDecay(name),
  fRnd(new TRandom3()),
  fInputIon(NULL),
  fOutputIon(NULL),
  fDecayPos(99999999.),
  fDecayFinish(kFALSE)
{
}

ERTextDecay::~ERTextDecay(){

}

Bool_t ERTextDecay::Init(){

  if (fInputIon) {
    fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIonName);
    if ( ! fInputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion " << fInputIonName << " not found in database!"<< endl;
        return kFALSE;
    }
  }
  else{
    std::cerr  << "Input ion not defined"<< endl;
    return kFALSE;
  }

  if (fOutputIon){
    fOutputIonPDG = TDatabasePDG::Instance()->GetParticle(fOutputIonName);
    if ( ! fOutputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion "<< fOutputIonName <<" not found in database!" << endl;
        return kFALSE;
    }
  }
  else{
    std::cerr  << "Output ion not defined" << endl;
    return kFALSE;
  }

  return kTRUE;
}

Bool_t ERTextDecay::Stepping(){
  if(!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode()){
    gMC->SetMaxStep(0.01);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() > fDecayPos){
      TLorentzVector inputIonV;
      gMC->TrackMomentum(inputIonV);
      //Add new ion
      Int_t newTrackNb;
      gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), fOutputIonPDG->PdgCode(),
                           inputIonV.Px(),inputIonV.Py(),inputIonV.Pz(),
                           inputIonV.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fOutputIonPDG->Mass(), 0);
      //Add particles
      for (Int_t iParticle = 0; iParticle < fOutputParticlesPDG.size(); iParticle++){
        TParticlePDG* particle = fOutputParticlesPDG[iParticle];
        gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), particle->PdgCode(),
                           inputIonV.Px(),inputIonV.Py(),inputIonV.Pz(),
                           inputIonV.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, particle->Mass(), 0);
      }
      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
    }
  }
  return kTRUE;
}

void ERTextDecay::BeginEvent(){	
  fDecayFinish = kFALSE;
}

void ERTextDecay::FinishEvent(){

}

void ERTextDecay::SetInputIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  fInputIonName = fName + TString("_InputIon");
  fInputIon = new FairIon(fInputIonName,A,Z,Q);
  run->AddNewIon(fInputIon);
}

void ERTextDecay::SetOutputIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  fOutputIonName = fName + TString("_OutputIon");
  fOutputIon = new FairIon(fOutputIonName,A,Z,Q);
  run->AddNewIon(fOutputIon);
}

void ERTextDecay::AddOutputParticle(Int_t pdg){
  fOutputParticlesPDG.push_back(TDatabasePDG::Instance()->GetParticle(pdg));
}

ClassImp(ERTextDecay)
