// -------------------------------------------------------------------------
// -----                      ERDsrdEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDsrdEvent_H
#define ERDsrdEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERDsrdEvent: public ERRawEvent {

public:
	UShort_t S1[16];
	UShort_t R1[16];

	int mD11,mD12;
	int nD11[32],nD12[32],neD11[32],neD12[32];
public:
	ERDsrdEvent();
	virtual ~ERDsrdEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERDsrdEvent,1);
};

#endif
