#include "ERTextDecay.h"

#include <iostream>
#include <fstream>
#include <string>

#include "TVirtualMC.h"
#include "TMCProcess.h"

#include "FairRunSim.h"

using namespace std;

#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERTextDecay::ERTextDecay(TString name):
  ERDecay(name),
  fRnd(new TRandom3()),
  fInputIon(NULL),
  fOutputIon(NULL),
  fDecayPos(99999999.),
  fDecayFinish(kFALSE),
  fFileName(""),
  fNOutputs(0)
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
  
  if (fFileName == ""){
    cerr << "File for " << fName << " decay not defined!" << endl;
    return kFALSE;
  }
  
  if (!ReadFile()){
    cerr << "Problem in "<< fName << " decay file reading!" << endl;
    return kFALSE;
  }

  return kTRUE;
}

Bool_t ERTextDecay::ReadFile(){
  TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/input/" + fFileName;
  ifstream f;
  f.open(path.Data());
  if (!f.is_open()){
    cerr << "Can`t open file " << path << endl;
    return kFALSE;  
  }
  //Пропускаем шапку файла
  string header;
  std::getline(f,header);
  
  Float_t E, Px, Py, Pz, Angle;
  while(!f.eof()){
    vector<TLorentzVector> decay;
    f >> E;
    for (Int_t iOutput = 0; iOutput < fNOutputs; iOutput++){
      f >> Px >> Py >> Pz;
      cout << Px << " " << Py << " " << Pz << endl;
      decay.push_back(TLorentzVector(Px,Py,Pz,0.));
      if (iOutput == fNOutputs-1)
        f >> Angle;
    }
    fDecays.push_back(decay);
  }
  cout << fDecays.size() << " events have read from file " << path << endl;
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
      vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];
      TLorentzVector outputIonV = decay[0];
      outputIonV.SetE(outputIonV.Px()*outputIonV.Px() + outputIonV.Py()*outputIonV.Py() + 
                       outputIonV.Pz()* outputIonV.Pz() + fOutputIonPDG->Mass()*fOutputIonPDG->Mass()); 
      gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), fOutputIonPDG->PdgCode(),
                           outputIonV.Px(),outputIonV.Py(),outputIonV.Pz(),
                           outputIonV.E(), curPos.X(), curPos.Y(), curPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fOutputIonPDG->Mass(), 0);
      //Add particles
      for (Int_t iParticle = 0; iParticle < fOutputParticlesPDG.size(); iParticle++){
        TParticlePDG* particle = fOutputParticlesPDG[iParticle];
        TLorentzVector particleV = decay[iParticle+1];
        particleV.SetE(particleV.Px()*particleV.Px() + particleV.Py()*particleV.Py() + 
                       particleV.Pz()* particleV.Pz() + particle->Mass()*particle->Mass());
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
  fNOutputs++;
}

void ERTextDecay::AddOutputParticle(Int_t pdg){
  fNOutputs++;
  fOutputParticlesPDG.push_back(TDatabasePDG::Instance()->GetParticle(pdg));
}

ClassImp(ERTextDecay)
