// -------------------------------------------------------------------------
// -----                       ERmuSiHit source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERmuSiHit.h"

ERmuSiHit::ERmuSiHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station):
	FairHit(detID, pos, dpos, index),
	fStation(station), fID(id)
{

}

ClassImp(ERmuSiHit)