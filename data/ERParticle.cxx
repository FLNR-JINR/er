#include "ERParticle.h"

ERParticle::ERParticle(const TLorentzVector& detectorState, const TLorentzVector& targetState) 
    :fDetectorState(detectorState), fTargetState(targetState) 
{
}