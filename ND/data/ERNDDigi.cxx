// -------------------------------------------------------------------------
// -----                       ERNDDigi source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERNDDigi.h"

ERNDDigi::ERNDDigi(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t stilbenNr, 
					Float_t lightYoeld, Float_t time,Float_t neutronProb):
	FairHit(detID, pos, dpos, stilbenNr),
	fID(id), fTime(time), fLightYield(lightYoeld),
	fNeutronProb(neutronProb)
{

}

ClassImp(ERNDDigi)