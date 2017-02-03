/*
 * RawEvent.h
 *
 *  Created on: Dec 27, 2016
 *      Author: vratik
 */

#ifndef DATACLASSES_RAWEVENT_H_
#define DATACLASSES_RAWEVENT_H_

#include <iostream>

//#include "Rtypes.h"
#include "TError.h"
#include "TGraph.h"
#include "TArrayD.h"
#include "TNamed.h"

//#define NCELLS 1024

using std::cout;
using std::endl;

class RawEvent: public TNamed {

private:
//	Double_t fAmp[NCELLS]; //array for raw amplitudes
//	Double_t fTime[NCELLS]; //array for raw times

	TArrayD fAmp;	//array for raw amplitudes
	TArrayD fTime;	//array for raw times

	const Int_t fNPoints;

//	TGraph *gAmp;

//public:
//	TArrayD fAmpA;

public:
	RawEvent();
	RawEvent(const Int_t npoints);
	virtual ~RawEvent();
	ClassDef(RawEvent,1);

	void Reset();
	//Resets arrays to zeros

	const Double_t* GetAmp() const { return fAmp.GetArray(); }
	
	const Double_t* GetTime() const { return fTime.GetArray(); }

	void SetAmp(Double_t a, Int_t i);
	//Takes amplitude (raw data, voltage from binary file) 
	//and places it in the array fAmp

	void SetTime(Double_t t, Int_t i);
	//Takes time (raw data, times from binary file) 
	//and places it in the array fTime

	void PrintAmp(Int_t i);
	//Prints i amplitudes (to make sense i shold be NCELLS)

	void PrintTime(Int_t i);

	Double_t GetTime(Int_t i);

	Double_t GetAmp(Int_t i);

	Double_t& Amp(Int_t i){return fAmp[i];}

//	void InvertAmp(Double_t a, Int_t i);
	//Inverts the amplitudes i.e. makes from negative singals 
	//posititve signals and vise versa.

//	void SetGraphs();
//	TGraph* GetGraph() {
//		return gAmp;
//	}

private:
	void Init();

};

#endif /* DATACLASSES_RAWEVENT_H_ */
