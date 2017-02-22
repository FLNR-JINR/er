// -------------------------------------------------------------------------
// -----                      ERDecay header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TString.h"
#include "TGenPhaseSpace.h"
#include "TDatabasePDG.h"               //for TDatabasePDG

class ERDecay{
protected:
	TString fName;
public:
	ERDecay(TString name);
	~ERDecay();

	virtual Bool_t Stepping() = 0;

	virtual void BeginEvent() = 0;
	virtual void FinishEvent() = 0;
	virtual Bool_t Init() = 0;

	TString GetName() {return fName;}
	ClassDef(ERDecay,1)
};

#endif
