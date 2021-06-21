#include "ERBeamDetParticle.h"

ERBeamDetParticle::ERBeamDetParticle(Int_t pid, TLorentzVector tofState, 
									 TLorentzVector targetState, 
								     float time_on_target, float probability)
	:TNamed(),
	fPID(pid),
	fTofState(tofState),
	fTargetState(targetState),
	fTimeOnTarget(time_on_target),
	fProbability(probability)
{
}
void ERBeamDetParticle::AddParameters(Int_t pid, TLorentzVector tofState, TLorentzVector targetState,
		 							  float probability)
{
	fPID = pid;
	fTofState = tofState;
	fTargetState = targetState;
	fProbability = probability;
}