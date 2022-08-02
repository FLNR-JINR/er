// -------------------------------------------------------------------------
// -----                      ERDecayMCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERDecayMCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERDecayMCEventHeader_H_
#define ERDecayMCEventHeader_H_

#include "TLorentzVector.h"
#include "TArrayI.h"

#include "FairMCEventHeader.h"

class ERDecayMCEventHeader : public FairMCEventHeader {
protected:
  TVector3 fReactionPos;
  Int_t fInputTrackID;
  TArrayI fOutputTrackID;
  Int_t fOutputsTracksNb = 0;
  Int_t fTrigger;
  Int_t fTriggerPriority;
public:
  ERDecayMCEventHeader();

  void SetReactionPos(TVector3 pos){fReactionPos = pos;}
  void SetInputIon(Int_t iID){fInputTrackID = iID;}
  void SetTrigger(Int_t trigger){fTrigger = trigger;}
  void SetTriggerPriority(Int_t priority){fTriggerPriority = priority;}
  void AddOutputParticle(Int_t oID){fOutputsTracksNb++; 
                                    fOutputTrackID.Set(fOutputsTracksNb);
                                    fOutputTrackID[fOutputsTracksNb-1]=oID;}

  Int_t GetTrigger() const {return fTrigger;}
  Int_t GetTriggerPriority() const {return fTriggerPriority;}
  TArrayI GetOutputTrackID() const {return fOutputTrackID;}
  void Clear();

  ClassDef(ERDecayMCEventHeader,1)
};

#endif