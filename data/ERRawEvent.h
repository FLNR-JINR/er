// -------------------------------------------------------------------------
// -----                      ERRawEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRawEvent_H
#define ERRawEvent_H

#include "TNamed.h"
#include "TTree.h"
#include "TString.h"

class ERRawEvent: public TNamed {

public:
	ERRawEvent();
	virtual ~ERRawEvent();

	virtual Bool_t Register(TTree* tree, TString branchName) = 0;

	virtual Int_t Process() = 0;

	ClassDef(ERRawEvent,1);
};

#endif
