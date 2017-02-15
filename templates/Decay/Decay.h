
#ifndef Decay_H
#define Decay_H

#include "TRandom3.h"

#include "ERDecay.h"

class Decay : public ERDecay{
private:
	TRandom3 *fRnd;
	
	TParticlePDG*   fPrimaryIon;
    TParticlePDG*   fSecondaryIon;
    Float_t fReactionPos;
  	Bool_t fReactionFinish;
public:
	Decay();
	~Decay();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	ClassDef(Decay,1)
};

#endif
