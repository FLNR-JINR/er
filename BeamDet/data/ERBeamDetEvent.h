// -------------------------------------------------------------------------
// -----                      ERBeamDetEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetEvent_H
#define ERBeamDetEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERBeamDetEvent: public ERRawEvent {

public:
	//TOF
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
	//MWPC
	UShort_t nx1;
	UShort_t ny1;
	UShort_t nx2;
	UShort_t ny2;
	UShort_t x1[32];
	UShort_t y1[32];
	UShort_t x2[32];
	UShort_t y2[32];

public:
	ERBeamDetEvent();
	virtual ~ERBeamDetEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERBeamDetEvent,1);
};

#endif
