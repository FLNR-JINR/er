// -------------------------------------------------------------------------
// -----                      ERRTelescopeCalEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeCalEvent_H
#define ERRTelescopeCalEvent_H

#include "TNamed.h"

class ERRTelescopeCalEvent : public TNamed {
public:
	int mC11,mC12;
	double eC11[32], eC12[32];
	int nC11[32],nC12[32];
public:
	ERRTelescopeCalEvent();
	virtual ~ERRTelescopeCalEvent();

	void Reset();

	ClassDef(ERRTelescopeCalEvent,1);
};

#endif
