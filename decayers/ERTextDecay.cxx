#include "ERTextDecay.h"

#include <iostream>
#include <fstream>
#include <string>

#include "TVirtualMC.h"
#include "TMCProcess.h"

#include "FairRunSim.h"

#include "ERDecayMCEventHeader.h"

using namespace std;

#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERTextDecay::ERTextDecay(TString name):
  ERDecay(name),
  fRnd(new TRandom3()),
  fInputIon(NULL),
  fOutputIon(NULL),
  fDecayPosZ(99999999.),
  fDecayFinish(kFALSE),
  fUniform(kFALSE),
  fExponential(kFALSE),
  fFileName(""),
  fNOutputs(0),
  fVolumeName("")
{
}

ERTextDecay::~ERTextDecay(){

}

Bool_t ERTextDecay::Init(){

  if (fInputIon) {
    fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIon->GetName());
    if ( ! fInputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion " << fInputIon->GetName() << " not found in database!"<< endl;
        return kFALSE;
    }
  }
  else{
    std::cerr  << "Input ion not defined"<< endl;
    return kFALSE;
  }

  if (fOutputIon){
    fOutputIonPDG = TDatabasePDG::Instance()->GetParticle(fOutputIon->GetName());
    if ( ! fOutputIonPDG ) {
        std::cerr  << "ERTextDecay: Ion "<< fOutputIon->GetName() <<" not found in database!" << endl;
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

  if (fVolumeName == ""){
    cerr << "Volume name for decay is not defined!" << endl;
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
      decay.push_back(TLorentzVector(Px,Py,Pz,0.));
      if (iOutput == fNOutputs-1)
        f >> Angle;
    }
    fDecays.push_back(decay);
  }
  cout << fDecays.size() << " events have read from file " << path << endl;
  return kTRUE;
}

Bool_t ERTextDecay::Stepping() {
  if (gMC->CurrentEvent() > fDecays.size()-1) {
    //Fatal("ERTextDecay::BeginEvent",TString("Events count in file") 
    //                                        + fFileName + TString(" less currenet event number"));
    Fatal("ERTextDecay::BeginEvent", "Events count in file %s less currenet event number", fFileName.Data());
  }

  if(!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() &&  gMC->CurrentVolName() == fVolumeName){
    gMC->SetMaxStep(0.01);
    gMC->TrackPosition(fDecayPos);
    if (fDecayPos.Z() > fDecayPosZ){
      gMC->TrackMomentum(fInputIonV);
      //Add new ion
      Int_t newTrackNb;
      vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];
      TLorentzVector outputIonV = decay[0];
      outputIonV.SetE(outputIonV.Px()*outputIonV.Px() + outputIonV.Py()*outputIonV.Py() + 
                       outputIonV.Pz()* outputIonV.Pz() + fOutputIonPDG->Mass()*fOutputIonPDG->Mass());
      outputIonV.Boost(fInputIonV.BoostVector());
      gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), fOutputIonPDG->PdgCode(),
                           outputIonV.Px(),outputIonV.Py(),outputIonV.Pz(),
                           outputIonV.E(), fDecayPos.X(), fDecayPos.Y(), fDecayPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, fOutputIonPDG->Mass(), 0);
      cout << "ERTextDecay: Added output ion with PDG = " << fOutputIonPDG->PdgCode() << "; pos=(" 
            << fDecayPos.X() << "," << fDecayPos.Y() << "," << fDecayPos.Z() << "); mom=("
            << outputIonV.Px() << "," << outputIonV.Py() << "," << outputIonV.Pz() << ")" << endl;
      decay[0] = outputIonV;
      //Add particles
      for (Int_t iParticle = 0; iParticle < fOutputParticlesPDG.size(); iParticle++){
        TParticlePDG* particle = fOutputParticlesPDG[iParticle];
        TLorentzVector particleV = decay[iParticle+1];
        particleV.SetE(particleV.Px()*particleV.Px() + particleV.Py()*particleV.Py() + 
                       particleV.Pz()* particleV.Pz() + particle->Mass()*particle->Mass());
        particleV.Boost(fInputIonV.BoostVector());
        gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), particle->PdgCode(),
                           fInputIonV.Px(),fInputIonV.Py(),fInputIonV.Pz(),
                           fInputIonV.E(), fDecayPos.X(), fDecayPos.Y(), fDecayPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, particle->Mass(), 0);
        cout << "ERTextDecay: Added output particle with PDG = " << particle->PdgCode() << "; pos=(" 
            << fDecayPos.X() << "," << fDecayPos.Y() << "," << fDecayPos.Z() << "); mom=("
            << particleV.Px() << "," << particleV.Py() << "," << particleV.Pz() << ")" << endl;
        decay[iParticle+1] = particleV;
      }
      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
      SaveToEventHeader();
    }
  }
  return kTRUE;
}

void ERTextDecay::SaveToEventHeader(){
  FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){
    cout << "ERDecayMCEventHeader" << endl;
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];
    header->SetDecayPos(fDecayPos.Vect());
    header->SetInputIon(fInputIonV);
    header->SetOutputIon(decay[0]);
    for (Int_t iOutput = 0; iOutput < fNOutputs; iOutput++){
      header->AddOutputParticle(decay[iOutput+1]);
    }
  }
}

void ERTextDecay::BeginEvent(){
  fDecayFinish = kFALSE;
  if (fUniform){
    fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  }
  if (fExponential){
    fDecayPosZ = fExponentialStart + fRnd->Exp(fExponentialTau);
  }
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

void ERTextDecay::SetUniformPos(Double_t a, Double_t b){
  fUniform = kTRUE;
  fUniformA = a;
  fUniformB = b;
}

void ERTextDecay::SetExponentialPos(Double_t start, Double_t tau){
  fExponential = kTRUE;
  fExponentialStart = start;
  fExponentialTau = tau;
}

ClassImp(ERTextDecay)
