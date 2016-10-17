// -------------------------------------------------------------------------
// -----                        EREventHeader header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef EREventHeader_H
#define EREventHeader_H

#include "FairEventHeader.h"

class EREventHeader : public FairEventHeader {
private:
	Int_t fNeuRadPECountF;
	Int_t fNeuRadPECountB;
	Float_t fNeuRadSumAmplitudeF;
	Float_t fNeuRadSumAmplitudeB;

public:
	EREventHeader();

	void SetNeuRadPECountF(Int_t peCount) {fNeuRadPECountF = peCount;}
	void SetNeuRadPECountB(Int_t peCount) {fNeuRadPECountB = peCount;}
	void SetNeuRadSumAmplitudeF(Float_t sum) {fNeuRadSumAmplitudeF = sum;}
	void SetNeuRadSumAmplitudeB(Float_t sum) {fNeuRadSumAmplitudeB = sum;}

	Int_t NeuRadPECountF() const {return fNeuRadPECountF;}
	Int_t NeuRadPECountB() const {return fNeuRadPECountB;}
	Float_t NeuRadSumAmplitudeF() const {return fNeuRadSumAmplitudeF;}
	Float_t NeuRadSumAmplitudeB() const {return fNeuRadSumAmplitudeB;}

	ClassDef(EREventHeader, 1)

};

#endif