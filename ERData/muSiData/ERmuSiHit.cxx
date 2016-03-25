// -------------------------------------------------------------------------
// -----                       ERmuSiHit source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERmuSiHit.h"

ERmuSiHit::ERmuSiHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index):
	FairHit(detID, pos, dpos, index)
{

}

ClassImp(ERmuSiHit)