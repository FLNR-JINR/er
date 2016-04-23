// -------------------------------------------------------------------------
// -----                      ERDecay header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TDatabasePDG.h"               //for TDatabasePDG

class ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	Float_t fTauCM; //ps
	
	TParticlePDG*   fSecondIon;
    TParticlePDG*   fThirdIon;

    TGenPhaseSpace* fPHSpace;

    Bool_t fTargetReactionFinish;
  	Bool_t fDirectReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay();
	~ERDecay();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	void SetDirectReactionTauCM(Float_t tau){fTauCM = tau;}

	ClassDef(ERDecay,1)
};

#endif
