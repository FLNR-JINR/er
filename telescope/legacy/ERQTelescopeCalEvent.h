// -------------------------------------------------------------------------
// -----                      ERQTelescopeCalEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERQTelescopeCalEvent_H
#define ERQTelescopeCalEvent_H

#include "TNamed.h"

class ERQTelescopeCalEvent : public TNamed {
public:
	int mC21,mC22,mC23,mC24,mC25,mC26;
	double eC21[32],eC22[32],eC23[32],eC24[32],eC25[32],eC26[32];
	int nC21[32],nC22[32],nC23[32],nC24[32],nC25[32],nC26[32],nCT21[32],nCT22[32];
	int mCT21,mCT22;
	double tC21[32],tC22[32];
	double eC2sum;
public:
	ERQTelescopeCalEvent();
	virtual ~ERQTelescopeCalEvent();

	void Reset();

	ClassDef(ERQTelescopeCalEvent,1);
};

#endif
