#include "ERTextDecay.h"

#include <iostream>
#include <fstream>
#include <string>

#include "TVirtualMC.h"
#include "TMCProcess.h"
#include "TMath.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"

using namespace std;

#include "ERMCEventHeader.h"      //for ERMCEventHeader

ERTextDecay::ERTextDecay(TString name):
ERDecay(name),
fFileName(""),
fNOutputs(0)
{
}

ERTextDecay::~ERTextDecay(){

}

void ERTextDecay::AddOutputIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  TString outputIonName = fName + TString("_OutputIon");
  FairIon* ion = new FairIon(outputIonName,A,Z,Q);
  fOutputIons.push_back(ion);
  run->AddNewIon(ion);
  fOutputs.push_back(ion);
  fNOutputs++;
}

void ERTextDecay::AddOutputParticle(Int_t pdg){
  fNOutputs++;
  fOutputParticlesPDG.push_back(TDatabasePDG::Instance()->GetParticle(pdg));
  fOutputs.push_back(TDatabasePDG::Instance()->GetParticle(pdg));
}

Bool_t ERTextDecay::Init(){
  if (!ERDecay::Init())
    return kFALSE;

  for (Int_t iOut = 0; iOut < fOutputs.size(); iOut++){
    if (TString(fOutputs[iOut]->ClassName()).Contains("FairIon")){
      FairIon* ion = (FairIon*)fOutputs[iOut];
      TParticlePDG* ionPDG = TDatabasePDG::Instance()->GetParticle(ion->GetName());
      if ( ! ionPDG ) {
        std::cerr  << "ERTextDecay: Ion "<< ion->GetName() <<" not found in database!" << endl;
        return kFALSE;
      }
      fOutputs[iOut] = ionPDG;
    }
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
    
    gMC->SetMaxStep(fStep);
    gMC->TrackPosition(fDecayPos);

    vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];

    if (fDecayPos.Z() > fDecayPosZ){

      gMC->TrackMomentum(fInputIonV);
      if (fTargetMass){
        TLorentzVector target(0,0,0,fTargetMass);
        fInputIonV += target;
      }
      LOG(INFO) << "ERTextDecay: Decay with beta =  " << fInputIonV.Beta() << "; Gamma = " << fInputIonV.Gamma() <<endl;
      LOG(INFO) << "ERTextDecay: Primary ion with mom = (" << fInputIonV.Px() << "," << fInputIonV.Py()
                 << "," << fInputIonV.Pz() << ")" << endl;
      for (Int_t iOut = 0; iOut < fOutputs.size(); iOut++){
        
        TParticlePDG* particle = (TParticlePDG*)fOutputs[iOut];
        TLorentzVector particleV = decay[iOut];

        particleV.SetE(TMath::Sqrt(particleV.Px()*particleV.Px() + particleV.Py()*particleV.Py() + 
                       particleV.Pz()* particleV.Pz() + particle->Mass()*particle->Mass()));
        particleV.Boost(fInputIonV.BoostVector());
        
        Int_t newTrackNb;
        gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), particle->PdgCode(),
                           particleV.Px(),particleV.Py(),particleV.Pz(),
                           particleV.E(), fDecayPos.X(), fDecayPos.Y(), fDecayPos.Z(),
                           gMC->TrackTime(), 0., 0., 0.,
                           kPDecay, newTrackNb, particle->Mass(), 0);

        cout << "ERTextDecay: Added output particle with PDG = " << particle->PdgCode() << "; pos=(" 
            << fDecayPos.X() << "," << fDecayPos.Y() << "," << fDecayPos.Z() << "); mom=("
            << particleV.Px() << "," << particleV.Py() << "," << particleV.Pz() << ")" << endl;
        
        decay[iOut] = particleV;
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
  //@TODO
  /*
  FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    vector<TLorentzVector> decay = fDecays[gMC->CurrentEvent()];
    header->SetDecayPos(fDecayPos.Vect());
    header->SetInputIon(fInputIonV);
    for (Int_t iOutput = 0; iOutput < fNOutputs; iOutput++){
      header->AddOutputParticle(decay[iOutput]);
    }
  }
  */
}


ClassImp(ERTextDecay)
