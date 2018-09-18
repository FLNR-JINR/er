// -------------------------------------------------------------------------
// -----                        ERBeamTimeEventHeader header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERBeamTimeEventHeader_H
#define ERBeamTimeEventHeader_H

#include "FairEventHeader.h"

class ERBeamTimeEventHeader : public FairEventHeader {
private:
	Int_t fTrigger;
public:
	ERBeamTimeEventHeader();

	void SetTrigger(Int_t trigger) {fTrigger = trigger;}
	Int_t GetTrigger() const {return fTrigger;}

	ClassDef(ERBeamTimeEventHeader, 1)
};

#endif
