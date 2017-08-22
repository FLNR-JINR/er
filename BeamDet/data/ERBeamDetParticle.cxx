#include "ERBeamDetParticle.h"

ERBeamDetParticle::ERBeamDetParticle()
	:TNamed(),
	fPID(0),
	fFourMomentum(),
	fProbability(0.),
	fPx(0.),
	fPy(0.),
	fPz(0.),
	fP(0.)
{
}
ERBeamDetParticle::ERBeamDetParticle(Int_t pid, TLorentzVector fourMomentum, Double_t probability)
	:TNamed(),
	fPID(pid),
	fFourMomentum(fourMomentum),
	fProbability(probability),
	fPx(fourMomentum.Px()),
	fPy(fourMomentum.Py()),
	fPz(fourMomentum.Pz()),
	fP(fourMomentum.P())	
{
}
void ERBeamDetParticle::AddParameters(Int_t pid, TLorentzVector fourMomentum, Double_t probability)
{
	fPID = pid;
	fFourMomentum = fourMomentum;
	fProbability = probability;

	fPx = fourMomentum.Px();
	fPy = fourMomentum.Py();
	fPz = fourMomentum.Pz();
	fP  = fourMomentum.P();
}