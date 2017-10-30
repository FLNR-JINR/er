// -------------------------------------------------------------------------
// -----           ERNeuRadHit source file                             -----
// -----           Created 03/16  by V.Schetinin                       -----
// -------------------------------------------------------------------------

#include "ERNeuRadHit.h"

ERNeuRadHit::ERNeuRadHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,
						 Int_t refIndex, Int_t ModuleIndex, Int_t FiberIndex, Float_t time):
	FairHit(detID, pos, dpos, refIndex),
	fFiberIndex(FiberIndex),fModuleIndex(ModuleIndex),fID(id), fTime(time)
{
}

ClassImp(ERNeuRadHit)
