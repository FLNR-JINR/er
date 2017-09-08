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
	Double_t x11,y11;
	Double_t dep11,dep12,dep21,dep22,dep23,dep24,dep25,dep26,dep2sum;
	Double_t t11, t11cm0, tmis11, tmis11cm0;
	Double_t th1,phi1;
	Double_t xbd1,ybd1,xbd2,ybd2;

	ERParticle ej11;
	
public:
	ERTelescopeRecoEvent();
	virtual ~ERTelescopeRecoEvent();

	void Reset();

	ClassDef(ERTelescopeRecoEvent,1);
};

#endif
	