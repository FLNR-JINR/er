#include "ERBeamDetParticle.h"

ERBeamDetParticle::ERBeamDetParticle()
	:TNamed(),
	fPID(0),
	fFourMomentum(),
	fProbability(0.)
{
}
ERBeamDetParticle::ERBeamDetParticle(Int_t pid, TLorentzVector fourMomentum, Double_t probability)
	:TNamed(),
	fPID(pid),
	fFourMomentum(fourMomentum),
	fProbability(probability)
{
}
void ERBeamDetParticle::AddParameters(Int_t pid, TLorentzVector fourMomentum, Double_t probability)
{
	fPID = pid;
	fFourMomentum = fourMomentum;
	fProbability = probability;
}