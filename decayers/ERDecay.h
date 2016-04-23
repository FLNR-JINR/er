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
public:
	ERDecay();
	~ERDecay();

	virtual Bool_t Stepping() = 0;

	virtual void BeginEvent() = 0;
	virtual void FinishEvent() = 0;

	ClassDef(ERDecay,1)
};

#endif
