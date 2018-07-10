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

  /*Tmp members for validation*/
  Double_t fTargetThickness;
  Double_t fStepIntVol;
  Double_t fDeviationFromTrajectory;
  Double_t fAngleDeviation;
  Double_t fGlobalXInTarget;
  Double_t fGlobalYInTarget;
  Double_t fGlobalZInTarget;
public:
  ERDecayMCEventHeader();
  void SetDecayPos(TVector3 pos){fDecayPos = pos;}
  void SetInputIon(Int_t iID){fInputTrackID = iID;}
  void AddOutputParticle(Int_t oID){fOutputsTracksNb++; 
                                    fOutputTrackID.Set(fOutputsTracksNb);
                                    fOutputTrackID[fOutputsTracksNb-1]=oID;}
  void Clear();

  /*Tmp method for validation*/
  void SetTrajectoryParams(Double_t thick, Double_t step) {
    fTargetThickness = thick;
    fStepIntVol = step;
  }
  void SetDeviationFromTrajectory(Double_t deviation) { fDeviationFromTrajectory = deviation;}
  
  void SetAngleDeviation(Double_t ang) {fAngleDeviation = ang;}

  void SetInteractionPos(Double_t x, Double_t y, Double_t z) {
    fGlobalXInTarget = x;
    fGlobalYInTarget = y;
    fGlobalZInTarget = z;
  }
  ClassDef(ERDecayMCEventHeader,1)
};

#endif