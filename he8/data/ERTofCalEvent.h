// -------------------------------------------------------------------------
// -----                      ERTofCalEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERTofCalEvent_H
#define ERTofCalEvent_H

#include "TNamed.h"

class ERTofCalEvent : public TNamed {
public:
	double tF3l,tF3r,tF4l,tF4r,aF3l,aF3r,aF4l,aF4r,tofb,tb,tcm;
	int iFlag;
public:
	ERTofCalEvent();
	virtual ~ERTofCalEvent();

	void Reset();

	ClassDef(ERTofCalEvent,1);
};

#endif
