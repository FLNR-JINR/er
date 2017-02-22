// -------------------------------------------------------------------------
// -----                      ERDecay31Arto30Ar header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY31ARTO30AR_H
#define ERDECAY31ARTO30AR_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay31Arto30Ar : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fSecondIon;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay31Arto30Ar();
	~ERDecay31Arto30Ar();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecay31Arto30Ar,1)
};

#endif
