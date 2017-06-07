// -------------------------------------------------------------------------
// -----                      ERBeamDetRecoEvent header file             -----
// -----                  Created 04/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetRecoEvent_H
#define ERBeamDetRecoEvent_H

#include "TNamed.h"
#include "ERParticle.h"

class ERBeamDetRecoEvent : public TNamed {
public:
	//Tof
	double tofb,tcm;
	float Tb;
	ERParticle projectile; 
	ERParticle CM0;
	ERParticle target;
	float xbt,ybt,zbt;

	//from Raw Track
	int mMW11;
	int mMW12;
	int mMW21;
	int mMW22;
	int mcMW11;
	int mcMW12;
	int mcMW21;
	int mcMW22;
	int nMW11[32];
	int nMW12[32];
	int nMW21[32];
	int nMW22[32];
	short mwpc[8];

	//from trackD
	double xmw1,ymw1,xmw2,ymw2,thb,phib;
public:
	ERBeamDetRecoEvent();
	virtual ~ERBeamDetRecoEvent();

	void Reset();

	ClassDef(ERBeamDetRecoEvent,1);
};

#endif
