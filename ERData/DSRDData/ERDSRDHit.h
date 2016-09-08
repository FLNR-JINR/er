// -------------------------------------------------------------------------
// -----                        ERDSRDHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERDSRDHit_H
#define ERDSRDHit_H

#include "FairHit.h"

class ERDSRDHit : public FairHit{
private:
	Int_t fID;
	Float_t fEloss;
	Float_t fTime;
public:
	ERDSRDHit(){}
	ERDSRDHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);

	Int_t ID() const {return fID;}


	ClassDef(ERDSRDHit, 1)

};

#endif

