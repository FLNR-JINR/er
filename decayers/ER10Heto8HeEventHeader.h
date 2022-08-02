#ifndef ER10Heto8HeEventHeader_H_
#define ER10Heto8HeEventHeader_H_

#include "TLorentzVector.h"
#include "TArrayI.h"

#include "ERDecayMCEventHeader.h"

class ER10Heto8HeEventHeader : public ERDecayMCEventHeader {
private:
  TLorentzVector fHe8b;
  TLorentzVector fHe8d;
  TLorentzVector fH3;
  TLorentzVector fH1;
  TLorentzVector fHe10;
  TLorentzVector fn1;
  TLorentzVector fn2;
  
  Int_t fTrigger = 0;
  Int_t fTriggerPriority = 0;
  float fTime = -1.;
public:
  ER10Heto8HeEventHeader() : fTrigger(0), fTriggerPriority(0) {}
  void SetData(const TVector3& position, const TLorentzVector& He8b,
               const TLorentzVector& H3, const TLorentzVector& H1,
               const TLorentzVector& He8d, const TLorentzVector& He10,
               const TLorentzVector& n1, const TLorentzVector& n2, float time);
  
  void SetTrigger(Int_t trigger) { fTrigger = trigger; }
  
  Int_t GetTrigger() const {return fTrigger;}
  Int_t GetTriggerPriority() const {return fTriggerPriority;}
  TLorentzVector GetHe8b() const { return fHe8b; }
  TLorentzVector GetH3() const { return fH3; }
  TLorentzVector GetH1() const { return fH1; }
  TLorentzVector GetHe8d() const { return fHe8d; }
  TLorentzVector GetHe10() const { return fHe10; }
  TLorentzVector GetN1() const { return fn1; }
  TLorentzVector GetN2() const { return fn2; }

  void Clear();

  ClassDef(ER10Heto8HeEventHeader,1)
};

#endif