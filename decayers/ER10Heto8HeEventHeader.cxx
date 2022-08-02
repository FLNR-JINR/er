// -----                      EREXP1811EventHeader source file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------
#include "ER10Heto8HeEventHeader.h"

#include "FairLogger.h"

void ER10Heto8HeEventHeader::SetData(const TVector3& position, const TLorentzVector& He8b,
                                    const TLorentzVector& H3, const TLorentzVector& H1,
                                    const TLorentzVector& He8d,
                                    const TLorentzVector& He10, const TLorentzVector& n1,
                                    const TLorentzVector& n2, const float time) 
{
  fReactionPos = position;
  fHe8b = He8b;
  fH3 = H3;
  fH1 = H1;
  fHe8d = He8d;
  fHe10 = He10;
  fn1 = n1;
  fn2 = n2;
  fTime = time;
}
// -------------------------------------------------------------------------
void ER10Heto8HeEventHeader::Clear() {
  ERDecayMCEventHeader::Clear();
  fHe8b.SetXYZM(0, 0, 0, 0);
  fH3.SetXYZM(0, 0, 0, 0);
  fH1.SetXYZM(0, 0, 0, 0);
  fHe8d.SetXYZM(0, 0, 0, 0);
  fHe10.SetXYZM(0, 0, 0, 0);
  fn1.SetXYZM(0, 0, 0, 0);
  fn2.SetXYZM(0, 0, 0, 0);
  fTrigger = 0;
  fTriggerPriority = 0;
  fTime = -1.;
}
// -------------------------------------------------------------------------

ClassImp(ER10Heto8HeEventHeader)