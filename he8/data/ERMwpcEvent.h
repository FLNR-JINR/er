// -------------------------------------------------------------------------
// -----                      ERMwpcEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERMwpcEvent_H
#define ERMwpcEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERMwpcEvent: public ERRawEvent {

public:
	UShort_t nx1;
	UShort_t ny1;
	UShort_t nx2;
	UShort_t ny2;
	UShort_t x1[32];
	UShort_t y1[32];
	UShort_t x2[32];
	UShort_t y2[32];
public:
	ERMwpcEvent();
	virtual ~ERMwpcEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERMwpcEvent,1);
};

#endif
