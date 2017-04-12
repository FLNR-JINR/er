// -------------------------------------------------------------------------
// -----                        ERHe8EventHeader header file                   -----
// -----                     Created 04/17  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERHe8EventHeader_H
#define ERHe8EventHeader_H

#include "TTree.h"
#include "TString.h"

#include "FairEventHeader.h"

class ERHe8EventHeader : public FairEventHeader {
public:
	Int_t HE8Event_nevent;
	Int_t HE8Event_trigger;
	Int_t HE8Event_subevents;
	Int_t HE8Event_evsize;
	UInt_t HE8Event_time;
public:
	ERHe8EventHeader();

	Bool_t Register(TTree* tree, TString branchName);

	ClassDef(ERHe8EventHeader, 1)

};

#endif