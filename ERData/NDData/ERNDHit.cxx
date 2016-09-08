// -------------------------------------------------------------------------
// -----                       ERNDHit source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERNDHit.h"

ERNDHit::ERNDHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, 
					Float_t lightYoeld, Float_t time,Float_t neutronProb):
	FairHit(detID, pos, dpos, point_index),
	fID(id), fTime(time), fLightYield(lightYoeld),
	fNeutronProb(neutronProb)
{

}

ClassImp(ERNDHit)