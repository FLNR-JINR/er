// -------------------------------------------------------------------------
// -----                      ERBeamDetRecoEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetRecoEvent_H
#define ERBeamDetRecoEvent_H

#include "TNamed.h"

class ERBeamDetRecoEvent : public TNamed {
public:
	//Tof
	double tofb,tb,tcm;
public:
	ERBeamDetRecoEvent();
	virtual ~ERBeamDetRecoEvent();

	void Reset();

	ClassDef(ERBeamDetRecoEvent,1);
};

#endif
