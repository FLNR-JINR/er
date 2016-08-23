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

ClassImp(ERDecayer)