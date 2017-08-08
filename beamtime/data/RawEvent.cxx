/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "RawEvent.h"

RawEvent::RawEvent() : 
//--------------------------------------------------------------------------------------------------
TNamed("RawEvent", "RawEvent"),
fNPoints(1024)
{
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
RawEvent::RawEvent(const Int_t npoints) : fNPoints(npoints) {
	// TODO Auto-generated constructor stub
	Init();
	Reset();
}
//--------------------------------------------------------------------------------------------------
RawEvent::~RawEvent() {
	// TODO Auto-generated destructor stub
}
//--------------------------------------------------------------------------------------------------
void RawEvent::Init() {
	fAmp.Set(fNPoints);
	fTime.Set(fNPoints);
}
//--------------------------------------------------------------------------------------------------
void RawEvent::Reset() {

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
void RawEvent::PrintTime(Int_t i) {
		cout << fTime[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void RawEvent::PrintAmp(Int_t i) {
		cout << fAmp[i] << endl;
}
//--------------------------------------------------------------------------------------------------
void RawEvent::SetAmp(Double_t a, Int_t i) {
//	cout << fNPoints << endl;
	if (i >= fNPoints) {
		Error("RawEvent::SetAmp", "Array with raw amplitudes is overloaded!");
		return;
	}
	fAmp[i] = a;
	return;
}
//--------------------------------------------------------------------------------------------------
void RawEvent::SetTime(Double_t t, Int_t i) {
	if (i >=fNPoints) {
		Error("RawEvent::SetTime", "Array with raw times is overloaded!");
		return;
	}
	fTime[i] = t;
	return;
}
//--------------------------------------------------------------------------------------------------
Double_t RawEvent::GetTime(Int_t i) {
		return fTime[i];
}
//--------------------------------------------------------------------------------------------------
Double_t RawEvent::GetAmp(Int_t i) {

		return fAmp[i];
}
//--------------------------------------------------------------------------------------------------
void RawEvent::SetStartTime(Double_t t) {
	fStartTime = t;
	return;
}
//--------------------------------------------------------------------------------------------------
void RawEvent::SetFinishTime(Double_t t) {
        fFinishTime = t;
        return;
}
//--------------------------------------------------------------------------------------------------
