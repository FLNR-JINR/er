// -------------------------------------------------------------------------
// -----                      ERDecay26Oto24O2n header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY26OTO24O2N_H
#define ERDECAY26OTO24O2N_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay26Oto24O2n : public ERDecay{
private:
	TRandom3 *fRnd;
	Float_t fTauCM; //ps
	
	TParticlePDG*   fSecondIon;
    TParticlePDG*   fThirdIon;

    TGenPhaseSpace* fPHSpace;

  	Bool_t fDirectReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay26Oto24O2n();
	~ERDecay26Oto24O2n();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	void SetDirectReactionTauCM(Float_t tau){fTauCM = tau;}

	ClassDef(ERDecay26Oto24O2n,1)
};

#endif
