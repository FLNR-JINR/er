

#ifndef ERHe10RecoEvent_H
#define ERHe10RecoEvent_H

#include "TNamed.h"

#include "ERParticle.h"

class ERHe10RecoEvent : public TNamed {
public:
	ERParticle mis11,mis11n;
	ERParticle ej11cm0;
	ERParticle mis11cm0,mis11ncm0;
	ERParticle projcm0;
	ERParticle targetcm0;
public:
	ERHe10RecoEvent();
	virtual ~ERHe10RecoEvent();

	void Reset();

	ClassDef(ERHe10RecoEvent,1);
};

#endif
