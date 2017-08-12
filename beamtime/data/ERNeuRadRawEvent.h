/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNeuRadRawEvent_H
#define ERNeuRadRawEvent_H

#include <iostream>

#include "TError.h"
#include "TGraph.h"
#include "TArrayD.h"
#include "TNamed.h"
#include "TArrayF.h"

using std::cout;
using std::endl;

/** @class ERNeuRadRawEvent 
 ** @brief class for raw data obtained from measurements or simulations
 ** @author V.Chudoba <chudoba@jinr.ru>
**/

class ERNeuRadRawEvent: public TNamed {

private:
	TArrayD fAmp;	//array for raw amplitudes
	TArrayD fTime;	//array for raw times

	TArrayF fPEAmps; //array for amps of single electron signals 
	TArrayF fPETimes; // array for times of single electron signals

	Double_t fStartTime;
	Double_t fFinishTime;
	const Int_t fNPoints;

public:
	ERNeuRadRawEvent();
	ERNeuRadRawEvent(const Int_t npoints);
	virtual ~ERNeuRadRawEvent();
	ClassDef(ERNeuRadRawEvent,1);

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

	TArrayF& GetPEAmps() {return fPEAmps;}
	TArrayF& GetPETimes() {return fPETimes;}
	void SetPETimes(TArrayF& fPETimesOLD) {fPETimesOLD.Copy(fPETimes);}
	void SetPEAmps(TArrayF& fPEAmpsOLD) {fPEAmpsOLD.Copy(fPEAmps);}

	void SetStartTime(Double_t t);
	Double_t GetStartTime() {return fStartTime;}
        void SetFinishTime(Double_t t);
        Double_t GetFinishTime() {return fFinishTime;}

private:
	void Init();

};

#endif /* ERNeuRadRawEvent_H */
