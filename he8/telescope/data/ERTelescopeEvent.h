// -------------------------------------------------------------------------
// -----                      ERTelescopeEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERTelescopeEvent_H
#define ERTelescopeEvent_H

#include "TTree.h"
#include "TString.h"

#include "ERRawEvent.h"

class ERTelescopeEvent: public ERRawEvent {

public:
	//source data
	UShort_t Q1[16];
	UShort_t Q2[16];
	UShort_t Q3[16];
	UShort_t Q4[16];
	UShort_t Q5[16];
	UShort_t Q6[16];
	UShort_t TQ1[16];
	UShort_t TQ2[16];
	//raw data
	int mD21,mD22,mD23,mD24,mD25,mD26;
	int mDT21, mDT22;
	int nD21[32],nD22[32],nD23[32],nD24[32],nD25[32],nD26[32];
	int neD21[32],neD22[32],neD23[32],neD24[32],neD25[32],neD26[32];
	int nDT21[32],nDT22[32];
	int ntD21[32],ntD22[32];
public:
	ERTelescopeEvent();
	virtual ~ERTelescopeEvent();

	virtual Bool_t Register(TTree* tree, TString branchName);

	virtual Int_t Process();

	ClassDef(ERTelescopeEvent,1);
};

#endif
