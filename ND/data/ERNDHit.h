// -------------------------------------------------------------------------
// -----                        ERNDHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNDHit_H
#define ERNDHit_H

#include "FairHit.h"

class ERNDHit : public FairHit{
private:
	Int_t fID;
	Float_t fLightYield;
	Float_t fTime;
	Float_t fNeutronProb;
public:
	ERNDHit(){}
	ERNDHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, 
				Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);

	Int_t ID() const {return fID;}

	Float_t LightYield() const {return fLightYield;}
	Float_t Time() const {return fTime;}
	Float_t NeutronProb() const {return fNeutronProb;}


	ClassDef(ERNDHit, 1)

};

#endif

