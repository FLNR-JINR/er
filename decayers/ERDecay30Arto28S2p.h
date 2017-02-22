// -------------------------------------------------------------------------
// -----                      ERDecay30Arto28S2p header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY30ARTO28S2P_H
#define ERDECAY30ARTO28S2P_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay30Arto28S2p : public ERDecay{
private:
	TRandom3 *fRnd;
	Float_t fTauCM; //ps
	
	TParticlePDG*   fSecondIon;
    TParticlePDG*   fThirdIon;

    TGenPhaseSpace* fPHSpace;

  	Bool_t fDirectReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay30Arto28S2p();
	~ERDecay30Arto28S2p();

	Bool_t Stepping();
	Bool_t Init();

	void BeginEvent();
	void FinishEvent();

	void SetDirectReactionTauCM(Float_t tau){fTauCM = tau;}

	ClassDef(ERDecay30Arto28S2p,1)
};

#endif
