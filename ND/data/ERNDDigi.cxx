// -------------------------------------------------------------------------
// -----                       ERNDDigi source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERNDDigi.h"

ERNDDigi::ERNDDigi(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t stilbenNb, 
					Float_t edep, Float_t lightYoeld, Float_t time,Float_t neutronProb):
	FairHit(detID, pos, dpos, id),
	fID(id), fTime(time), fLightYield(lightYoeld),
	fNeutronProb(neutronProb),
	fEdep(edep),
	fStilbenNb(stilbenNb)
{

}

ClassImp(ERNDDigi)