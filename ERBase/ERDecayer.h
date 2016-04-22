// -------------------------------------------------------------------------
// -----                      ERDecayer header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "FairIon.h"                    //for FairIon

class ERDecayer{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	FairIon*   fSecondIon;
    FairIon*   fThirdIon;

    TGenPhaseSpace* fPHSpace;
public:
	ERDecayer();
	~ERDecayer();

	void Stepping();
};

#endif
