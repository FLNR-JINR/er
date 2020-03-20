#include "ERParticle.h"

ERParticle::ERParticle(const TLorentVector& detectorState, const TLorentVector& targetState) 
    :fDetectorState(detectorState), fTargetState(targetState) 
{
}