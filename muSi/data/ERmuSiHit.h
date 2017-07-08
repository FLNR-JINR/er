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
	Int_t fID;
public:
	ERmuSiHit(){}
	ERmuSiHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station);

	Int_t Station() const {return fStation;}
	Int_t ID() const {return fID;}

	ClassDef(ERmuSiHit, 1)

};

#endif

