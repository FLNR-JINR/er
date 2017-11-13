// -------------------------------------------------------------------------
// -----                        ERNDDigi header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNDDigi_H
#define ERNDDigi_H

#include "FairHit.h"

class ERNDDigi : public FairHit{
private:
	Int_t fID;
	Float_t fLightYield;
	Float_t fTime;
	Float_t fNeutronProb;
public:
	ERNDDigi(){}
	ERNDDigi(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, 
				Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);

	Int_t ID() const {return fID;}

	Float_t LightYield() const {return fLightYield;}
	Float_t Time() const {return fTime;}
	Float_t NeutronProb() const {return fNeutronProb;}


	ClassDef(ERNDDigi, 1)

};

#endif

