#include "ERBeamDetTrack.h"

ERBeamDetTrack::ERBeamDetTrack()
{
}

ERBeamDetTrack::ERBeamDetTrack(TString name) : TNamed(name, name)
{
}

ERBeamDetTrack::ERBeamDetTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v)
{
	fXt = xt;
	fYt = yt;
	fZt = zt;
	fVectorOnTarget = v;
}

void ERBeamDetTrack::AddParameters(Double_t xt, Double_t yt, Double_t zt, TVector3 v)
{
	fXt = xt;
	fYt = yt;
	fZt = zt;
	fVectorOnTarget = v;
}
