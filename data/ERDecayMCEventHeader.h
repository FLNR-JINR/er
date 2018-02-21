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
private:
  TVector3 fDecayPos;
  Int_t fInputTrackID;
  TArrayI fOutputTrackID;
  Int_t fOutputsTracksNb = 0;
public:
  ERDecayMCEventHeader();
  void SetDecayPos(TVector3 pos){fDecayPos = pos;}
  void SetInputIon(Int_t iID){fInputTrackID = iID;}
  void AddOutputParticle(Int_t oID){fOutputsTracksNb++; 
                                    fOutputTrackID.Set(fOutputsTracksNb);
                                    fOutputTrackID[fOutputsTracksNb-1]=oID;}

  ClassDef(ERDecayMCEventHeader,1)
};

#endif