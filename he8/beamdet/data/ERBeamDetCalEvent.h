// -------------------------------------------------------------------------
// -----                      ERBeamDetCalEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetCalEvent_H
#define ERBeamDetCalEvent_H

#include "TNamed.h"

class ERBeamDetCalEvent : public TNamed {
public:
	//Tof
	double tF3l,tF3r,tF4l,tF4r,aF3l,aF3r,aF4l,aF4r,tofb,tb,tcm;
	int iFlag;
	
public:
	ERBeamDetCalEvent();
	virtual ~ERBeamDetCalEvent();

	void Reset();

	ClassDef(ERBeamDetCalEvent,1);
};

#endif
