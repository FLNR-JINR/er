// -------------------------------------------------------------------------
// -----                      ERDecayLi9DetoLi10_Li9n_p header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDecayLi9DetoLi10_Li9n_p_H
#define ERDecayLi9DetoLi10_Li9n_p_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecayLi9DetoLi10_Li9n_p : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fLi9;
	TParticlePDG*   fLi10;
	TParticlePDG*   fH2;
	TParticlePDG*   fn;
	TParticlePDG*   fp;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecayLi9DetoLi10_Li9n_p();
	~ERDecayLi9DetoLi10_Li9n_p();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	Bool_t Init();

	ClassDef(ERDecayLi9DetoLi10_Li9n_p,1)
};

#endif
