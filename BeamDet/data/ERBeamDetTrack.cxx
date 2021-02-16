#include "ERBeamDetTrack.h"

//--------------------------------------------------------------------------------------------------
ERBeamDetTrack::ERBeamDetTrack(TString name) : TNamed(name, name)
{}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrack::ERBeamDetTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v) {
	fXt = xt;
	fYt = yt;
	fZt = zt;
	fVectorOnTarget = v;
}
//--------------------------------------------------------------------------------------------------
TVector3 ERBeamDetTrack::GetDirection() const {
	TVector3 direction = fVectorOnTarget;
	direction.SetMag(1.);
	return direction;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetTrack)