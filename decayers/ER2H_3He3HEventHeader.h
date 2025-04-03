#ifndef ER2H_3He3HEventHeader_H_
#define ER2H_3He3HEventHeader_H_

#include "TLorentzVector.h"
#include "TArrayI.h"

#include "FairMCEventHeader.h"

class ER2H_3He3HEventHeader : public FairMCEventHeader {
private:
  TVector3 fReactionPos;
  TLorentzVector fHe8;
  TLorentzVector fH2;
  TLorentzVector fLi6;
  TLorentzVector fHe3;
  TLorentzVector fH3;
  TLorentzVector f4n;
  TLorentzVector fn1;
  TLorentzVector fn2;
  TLorentzVector fn3;
  TLorentzVector fn4;
  Float_t fThetaCM;
  Int_t fTrigger = 0;
  Int_t fTriggerPriority = 0;
public:
  ER2H_3He3HEventHeader() : fTrigger(0), fTriggerPriority(0) {}
  void SetData(const TVector3& position, const TLorentzVector& He8,
               const TLorentzVector& H2, const TLorentzVector& Li6,
               const TLorentzVector& tetraN,
               const TLorentzVector& n1, const TLorentzVector& n2,
               const TLorentzVector& n3, const TLorentzVector& n4,
               const TLorentzVector& H3, const TLorentzVector& He3,
               const Float_t& thetaCM);
  void SetTrigger(Int_t trigger) { fTrigger = trigger; }
  
  Int_t GetTrigger() const {return fTrigger;}
  Int_t GetTriggerPriority() const {return fTriggerPriority;}
  TLorentzVector GetHe8() const { return fHe8; }
  TLorentzVector GetH2() const { return fH2; }
  TLorentzVector GetLi6() const { return fLi6; }
  TLorentzVector Get3H() const { return fH3; }
  TLorentzVector Get3He() const { return fHe3; }
  TLorentzVector Get4n() const { return f4n; }
  TLorentzVector GetN1() const { return fn1; }
  TLorentzVector GetN2() const { return fn2; }
  TLorentzVector GetN3() const { return fn3; }
  TLorentzVector GetN4() const { return fn4; }
  Float_t GetThetaCM() const {return fThetaCM;}

  void Clear();

  ClassDef(ER2H_3He3HEventHeader,1)
};

#endif
