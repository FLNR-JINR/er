// -------------------------------------------------------------------------
// -----                        ERNeuRadHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHit_H
#define ERNeuRadHit_H

#include "FairHit.h"

class ERNeuRadHit : public FairHit {
private:
	Int_t fFiberIndex;
	Int_t fBundleIndex;
	Int_t fID;
	Float_t fTime;
public:
	ERNeuRadHit(){}
	ERNeuRadHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,  Int_t refIndex,
				 Int_t BundleIndex, Int_t FiberIndex, Float_t time);

	Int_t Fiber()  const {return fFiberIndex;}
	Int_t Bundle() const {return fBundleIndex;}
	Int_t ID()     const {return fID;}
	Float_t Time() const {return fTime;}

	ClassDef(ERNeuRadHit, 1)

};

#endif

