/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERRawEvent.h"

ERRawEvent::ERRawEvent() : 
//--------------------------------------------------------------------------------------------------
TNamed("ERRawEvent", "ERRawEvent"),
fNPoints(1024)
{
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
ERRawEvent::ERRawEvent(const Int_t npoints) : fNPoints(npoints) {
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
ERRawEvent::~ERRawEvent() {
	// TODO Auto-generated destructor stub
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::Init() {
	fAmp.Set(fNPoints);
	fTime.Set(fNPoints);
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::Reset() {

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
void ERRawEvent::PrintTime(Int_t i) {
		cout << fTime[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::PrintAmp(Int_t i) {
		cout << fAmp[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::SetAmp(Double_t a, Int_t i) {
//	cout << fNPoints << endl;
	if (i >= fNPoints) {
		Error("ERRawEvent::SetAmp", "Array with raw amplitudes is overloaded!");
		return;
	}
	fAmp[i] = a;
	return;
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::SetTime(Double_t t, Int_t i) {
	if (i >=fNPoints) {
		Error("ERRawEvent::SetTime", "Array with raw times is overloaded!");
		return;
	}
	fTime[i] = t;
	return;
}
//--------------------------------------------------------------------------------------------------
Double_t ERRawEvent::GetTime(Int_t i) {
		return fTime[i];
}
//--------------------------------------------------------------------------------------------------
Double_t ERRawEvent::GetAmp(Int_t i) {
		return fAmp[i];
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::SetStartTime(Double_t t) {
	fStartTime = t;
	return;
}
//--------------------------------------------------------------------------------------------------
void ERRawEvent::SetFinishTime(Double_t t) {
        fFinishTime = t;
        return;
}
//--------------------------------------------------------------------------------------------------
