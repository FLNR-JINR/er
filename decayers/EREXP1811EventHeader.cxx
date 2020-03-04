// -----                      EREXP1811EventHeader source file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------
#include "EREXP1811EventHeader.h"

#include "FairLogger.h"

void EREXP1811EventHeader::SetData(const TVector3& position, const TLorentzVector& He8,
                                   const TLorentzVector& H2, const TLorentzVector& He3,
                                   const TLorentzVector& H3,
                                   const TLorentzVector& H7, const TLorentzVector& n1,
                                   const TLorentzVector& n2, const TLorentzVector& n3,
                                   const TLorentzVector& n4) {
  fReactionPos = position;
  fHe8 = He8;
  fH2 = H2;
  fHe3 = He3;
  fH3 = H3;
  fH7 = H7;
  fn1 = n1;
  fn2 = n2;
  fn3 = n3;
  fn4 = n4;
}
// -------------------------------------------------------------------------
void EREXP1811EventHeader::Clear() {
  fHe8.SetXYZM(0, 0, 0, 0);
  fH2.SetXYZM(0, 0, 0, 0);
  fHe3.SetXYZM(0, 0, 0, 0);
  fH3.SetXYZM(0, 0, 0, 0);
  fH7.SetXYZM(0, 0, 0, 0);
  fn1.SetXYZM(0, 0, 0, 0);
  fn2.SetXYZM(0, 0, 0, 0);
  fn3.SetXYZM(0, 0, 0, 0);
  fn4.SetXYZM(0, 0, 0, 0);
  fTrigger = 0;
  fTriggerPriority = 0;
}
// -------------------------------------------------------------------------

ClassImp(EREXP1811EventHeader)