#include "ERDecayer.h"

#include<iostream>

using namespace std;

ERDecayer::ERDecayer(){
}

ERDecayer::~ERDecayer(){
}

Bool_t ERDecayer::Stepping(){
  for (vector<ERDecay*>::iterator it = fDecays.begin(); it != fDecays.end(); ++it){
    ERDecay* decay = (*it);
    if (!decay->Stepping())
      return kFALSE;
  }
  return kTRUE;
}

Bool_t ERDecayer::Init(){
  for (vector<ERDecay*>::iterator it = fDecays.begin(); it != fDecays.end(); ++it){
    ERDecay* decay = (*it);
    cout << "Decay " << decay->GetName() << " initialized!" << endl;
    if (!decay->Init())
      return kFALSE;
  }
  return kTRUE;
} 

void ERDecayer::BeginEvent(){
  for (vector<ERDecay*>::iterator it = fDecays.begin(); it != fDecays.end(); ++it){
    ERDecay* decay = (*it);
    decay->BeginEvent();
  }
}

void ERDecayer::FinishEvent(){
  for (vector<ERDecay*>::iterator it = fDecays.begin(); it != fDecays.end(); ++it){
    ERDecay* decay = (*it);
    decay->FinishEvent();
  }
}

void ERDecayer::AddDecay(ERDecay* decay) {
  cout << "Decay " << decay->GetName() << " added!" << endl;
  fDecays.push_back(decay);
}

ClassImp(ERDecayer)