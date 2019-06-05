// -----                      ERDecayMCEventHeader source file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------
#include "ERDecayMCEventHeader.h"

#include "FairLogger.h"

ERDecayMCEventHeader::ERDecayMCEventHeader():
  fTrigger(0),
  fTriggerPriority(0)
{

}

// -------------------------------------------------------------------------
void ERDecayMCEventHeader::Clear(){
  fOutputsTracksNb = 0;
  fOutputTrackID.Set(fOutputsTracksNb);
  fOutputTrackID.Reset();
  fTrigger = 0;
  fTriggerPriority = 0;
}
// -------------------------------------------------------------------------

ClassImp(ERDecayMCEventHeader)