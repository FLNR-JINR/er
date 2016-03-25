// -------------------------------------------------------------------------
// -----                        ERmuSiHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSIHIT_H
#define ERMUSIHIT_H

#include "FairHit.h"

class ERmuSiHit : public FairHit {
private:
	Int_t fStation;
public:
	ERmuSiHit(){}
	ERmuSiHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station);

	ClassDef(ERmuSiHit, 1)

};

#endif

