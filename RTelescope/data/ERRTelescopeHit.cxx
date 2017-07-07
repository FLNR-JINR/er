// -------------------------------------------------------------------------
// -----                       ERRTelescopeHit source file                   -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERRTelescopeHit.h"

ERRTelescopeHit::ERRTelescopeHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time):
	FairHit(detID, pos, dpos, point_index),
	fID(id), fTime(time), fEloss(eloss)
{

}

ClassImp(ERRTelescopeHit)