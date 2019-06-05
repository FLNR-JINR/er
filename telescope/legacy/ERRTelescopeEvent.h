// -------------------------------------------------------------------------
// -----                      ERRTelescopeEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeEvent_H
#define ERRTelescopeEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERRTelescopeEvent: public ERRawEvent {

public:
	UShort_t S1[16];
	UShort_t R1[16];

	int mD11,mD12;
	int nD11[32],nD12[32],neD11[32],neD12[32];
public:
	ERRTelescopeEvent();
	virtual ~ERRTelescopeEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERRTelescopeEvent,1);
};

#endif
