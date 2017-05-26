// -------------------------------------------------------------------------
// -----                      ERDsrdCalEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDsrdCalEvent_H
#define ERDsrdCalEvent_H

#include "TNamed.h"

class ERDsrdCalEvent : public TNamed {
public:
	int mC11,mC12;
	double eC11[32], eC12[32];
	int nC11[32],nC12[32];
public:
	ERDsrdCalEvent();
	virtual ~ERDsrdCalEvent();

	void Reset();

	ClassDef(ERDsrdCalEvent,1);
};

#endif
