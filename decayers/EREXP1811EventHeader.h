#ifndef EREXP1811EventHeader_H_
#define EREXP1811EventHeader_H_

#include "TLorentzVector.h"
#include "TArrayI.h"

#include "FairMCEventHeader.h"

class EREXP1811EventHeader : public FairMCEventHeader {
private:
  TVector3 fReactionPos;
  TLorentzVector fHe8;
  TLorentzVector fH2;
  TLorentzVector fH3;
  TLorentzVector fHe3;
  TLorentzVector fH7;
  TLorentzVector fn1;
  TLorentzVector fn2;
  TLorentzVector fn3;
  TLorentzVector fn4;
  Int_t fTrigger = 0;
  Int_t fTriggerPriority = 0;
public:
  EREXP1811EventHeader() : fTrigger(0), fTriggerPriority(0) {}
  void SetData(const TVector3& position, const TLorentzVector& He8,
               const TLorentzVector& H2, const TLorentzVector& He3,
               const TLorentzVector& H3, const TLorentzVector& H7,
               const TLorentzVector& n1, const TLorentzVector& n2,
               const TLorentzVector& n3, const TLorentzVector& n4);
  void SetTrigger(Int_t trigger) { fTrigger = trigger; }
  
  Int_t GetTrigger() const {return fTrigger;}
  Int_t GetTriggerPriority() const {return fTriggerPriority;}
  TLorentzVector GetHe8() const { return fHe8; }
  TLorentzVector GetH2() const { return fH2; }
  TLorentzVector GetHe3() const { return fHe3; }
  TLorentzVector GetH3() const { return fH3; }
  TLorentzVector GetH7() const { return fH7; }
  TLorentzVector GetN1() const { return fn1; }
  TLorentzVector GetN2() const { return fn2; }
  TLorentzVector GetN3() const { return fn3; }
  TLorentzVector GetN4() const { return fn4; }

  void Clear();

  ClassDef(EREXP1811EventHeader,1)
};

#endif