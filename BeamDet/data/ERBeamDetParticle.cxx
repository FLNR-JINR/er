#include "ERBeamDetParticle.h"

ERBeamDetParticle::ERBeamDetParticle()
	:TNamed(),
	fPID(0),
	fTofState(),
	fTargetState(),
	fProbability(0.),
	fPx(0.),
	fPy(0.),
	fPz(0.),
	fP(0.)
{
}
ERBeamDetParticle::ERBeamDetParticle(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability)
	:TNamed(),
	fPID(pid),
	fTofState(tofState),
	fTargetState(targetState),
	fProbability(probability),
	fPx(tofState.Px()),
	fPy(tofState.Py()),
	fPz(tofState.Pz()),
	fP(tofState.P())	
{
}
void ERBeamDetParticle::AddParameters(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability)
{
	fPID = pid;
	fTofState = tofState;
	fTargetState = targetState;
	fProbability = probability;

	fPx = fTofState.Px();
	fPy = fTofState.Py();
	fPz = fTofState.Pz();
	fP  = fTofState.P();
}