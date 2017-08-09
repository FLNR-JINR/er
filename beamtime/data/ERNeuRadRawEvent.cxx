/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadRawEvent.h"

ERNeuRadRawEvent::ERNeuRadRawEvent() : 
//--------------------------------------------------------------------------------------------------
TNamed("ERNeuRadRawEvent", "ERNeuRadRawEvent"),
fNPoints(1024)
{
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
ERNeuRadRawEvent::ERNeuRadRawEvent(const Int_t npoints) : fNPoints(npoints) {
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
ERNeuRadRawEvent::~ERNeuRadRawEvent() {
	// TODO Auto-generated destructor stub
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::Init() {
	fAmp.Set(fNPoints);
	fTime.Set(fNPoints);
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::Reset() {

  for (Int_t i = 0; i < fNPoints; i++) {
    fAmp[i] = 0;
    fTime[i] = 0;
  }
  fPEAmps.Reset();
  fPETimes.Reset();
  fStartTime = 0;
  fFinishTime = 0;
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::PrintTime(Int_t i) {
		cout << fTime[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::PrintAmp(Int_t i) {
		cout << fAmp[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::SetAmp(Double_t a, Int_t i) {
//	cout << fNPoints << endl;
	if (i >= fNPoints) {
		Error("ERNeuRadRawEvent::SetAmp", "Array with raw amplitudes is overloaded!");
		return;
	}
	fAmp[i] = a;
	return;
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::SetTime(Double_t t, Int_t i) {
	if (i >=fNPoints) {
		Error("ERNeuRadRawEvent::SetTime", "Array with raw times is overloaded!");
		return;
	}
	fTime[i] = t;
	return;
}
//--------------------------------------------------------------------------------------------------
Double_t ERNeuRadRawEvent::GetTime(Int_t i) {
		return fTime[i];
}
//--------------------------------------------------------------------------------------------------
Double_t ERNeuRadRawEvent::GetAmp(Int_t i) {
		return fAmp[i];
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::SetStartTime(Double_t t) {
	fStartTime = t;
	return;
}
//--------------------------------------------------------------------------------------------------
void ERNeuRadRawEvent::SetFinishTime(Double_t t) {
        fFinishTime = t;
        return;
}
//--------------------------------------------------------------------------------------------------
