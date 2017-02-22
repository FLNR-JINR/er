// -------------------------------------------------------------------------
// -----                      ERDecay27Fto26O header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY27FTO26O_H
#define ERDECAY27FTO26O_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay27Fto26O : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fSecondIon;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay27Fto26O();
	~ERDecay27Fto26O();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecay27Fto26O,1)
};

#endif
