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
	Int_t fStilbenNb;
	Float_t fLightYield;
	Float_t fEdep;
	Float_t fTime;
	Float_t fNeutronProb;
	Float_t fTAC;
public:
	ERNDDigi(){}
	ERNDDigi(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, 
			 Int_t stilbenNb, Float_t edep, Float_t lightYield, Float_t time, Float_t neutronProb,
			 Float_t tac = -1.);

	Int_t ID() const {return fID;}

	Int_t StilbenNb() const {return fStilbenNb;}
	Float_t EnergyLoss() const {return fEdep;}
	Float_t LightYield() const {return fLightYield;}
	Float_t Time() const {return fTime;}
	Float_t NeutronProb() const {return fNeutronProb;}
	Float_t TAC() const { return fTAC; }


	ClassDef(ERNDDigi, 1)

};

#endif

