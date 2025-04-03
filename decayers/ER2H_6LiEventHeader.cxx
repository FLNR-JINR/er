// -----                      ER2H_6LiEventHeader source file              -----
// -----                  Created 21/24  by I. Muzalevskii               -----
// -------------------------------------------------------------------------
#include "ER2H_6LiEventHeader.h"

#include "FairLogger.h"

void ER2H_6LiEventHeader::SetData(const TVector3& position, const TLorentzVector& He8,
                                   const TLorentzVector& H2, const TLorentzVector& Li6,
                                   const TLorentzVector& tetraN, const TLorentzVector& n1,
                                   const TLorentzVector& n2, const TLorentzVector& n3,
                                   const TLorentzVector& n4, const Float_t& thetaCM) {
  fReactionPos = position;
  fHe8 = He8;
  fH2 = H2;
  fLi6 = Li6;
  f4n = tetraN;
  fn1 = n1;
  fn2 = n2;
  fn3 = n3;
  fn4 = n4;
  fThetaCM = thetaCM;
}
// -------------------------------------------------------------------------
void ER2H_6LiEventHeader::Clear() {
  fHe8.SetXYZM(0, 0, 0, 0);
  fH2.SetXYZM(0, 0, 0, 0);
  fLi6.SetXYZM(0, 0, 0, 0);
  f4n.SetXYZM(0, 0, 0, 0);
  fn1.SetXYZM(0, 0, 0, 0);
  fn2.SetXYZM(0, 0, 0, 0);
  fn3.SetXYZM(0, 0, 0, 0);
  fn4.SetXYZM(0, 0, 0, 0);
  fTrigger = 0;
  fTriggerPriority = 0;
  fThetaCM = 200;
}
// -------------------------------------------------------------------------

ClassImp(ER2H_6LiEventHeader)
