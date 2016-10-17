// -------------------------------------------------------------------------
// -----                       ERNeuRadHitWBT source file                 -----
// -----           Created 03/16  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERNeuRadHitWBT.h"

ERNeuRadHitWBT::ERNeuRadHitWBT(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,
						 Int_t refIndex, Int_t  BundleIndex, Int_t FiberIndex,
						  Float_t time, Float_t qInteg):
	ERNeuRadHit(id, detID, pos, dpos, refIndex, BundleIndex, FiberIndex, time),
	fQInteg(qInteg)
{

}

ClassImp(ERNeuRadHitWBT)