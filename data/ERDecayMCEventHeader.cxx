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

// -------------------------------------------------------------------------
void ERDecayMCEventHeader::SetTrigger(Int_t trigger, Int_t priority)
{
  LOG(DEBUG) << trigger << " " << priority << FairLogger::endl;
  if (priority > fTriggerPriority){
    fTrigger = trigger;
    fTriggerPriority = priority;
  }
}
// -------------------------------------------------------------------------

ClassImp(ERDecayMCEventHeader)