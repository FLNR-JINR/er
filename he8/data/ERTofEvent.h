// -------------------------------------------------------------------------
// -----                      ERTofEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERTofEvent_H
#define ERTofEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERTofEvent: public ERRawEvent {

public:
	UShort_t TDC[16];
	UShort_t QDC[16];

	UShort_t ntF3l;
	UShort_t ntF3r;
	UShort_t ntF4r;
	UShort_t ntF4l;
	UShort_t naF3l;
	UShort_t naF3r;
	UShort_t naF4r;
	UShort_t naF4l;
	UShort_t ntD11;
public:
	ERTofEvent();
	virtual ~ERTofEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERTofEvent,1);
};

#endif
