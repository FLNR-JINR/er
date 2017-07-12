// -------------------------------------------------------------------------
// -----                      ERTelescopeRecoEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERTelescopeRecoEvent_H
#define ERTelescopeRecoEvent_H

#include "TNamed.h"
#include "ERParticle.h"

class ERTelescopeRecoEvent : public TNamed {
public:

	double xbd1,ybd1,xbd2,ybd2,xbd3,ybd3,xbd4,ybd4,xbd5,ybd5,xbd6,ybd6;

	double x11,y11,x21,y21,x22,y22,x23,y23,x24,y24,x25,y25;
	double th1,phi1,th2,phi2,t11,t22;
	double dep11,dep12,dep21,dep22,dep23,dep24,dep25,dep26;
	double t11cmp,t22cmp,pz11cmp,pz22cmp,th11cmp,th22cmp;
	double al111, al221;
	double tmis11,tmis22,thmis11,thmis22,phimis11,phimis22,exmis11;
public:
	ERTelescopeRecoEvent();
	virtual ~ERTelescopeRecoEvent();

	void Reset();

	ClassDef(ERTelescopeRecoEvent,1);
};

#endif
	