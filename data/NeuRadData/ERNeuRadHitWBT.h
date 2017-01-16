// -------------------------------------------------------------------------
// -----                        ERNeuRadHitWBT header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitWBT_H
#define ERNeuRadHitWBT_H

#include "ERNeuRadHit.h"

class ERNeuRadHitWBT : public ERNeuRadHit {
private:
	Float_t fQInteg;
public:
	ERNeuRadHitWBT(){}
	ERNeuRadHitWBT(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,  Int_t refIndex,
				 Int_t BundleIndex, Int_t FiberIndex, Float_t time, Float_t QInteg);

	Float_t QInteg() {return fQInteg;}

	ClassDef(ERNeuRadHitWBT, 1)

};

#endif

