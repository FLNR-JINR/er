// -----       ERDecay6He4He2nTransferEventHeader header file        -----
// -----                  Created 08/24  by V. Chudoba               -----
// -------------------------------------------------------------------------
#ifndef ERDecay6He4He2nTransferEventHeader_H
#define ERDecay6He4He2nTransferEventHeader_H

// #include "TLorentzVector.h"
// #include "TArrayI.h"

#include "ERDecayMCEventHeader.h"

class ERDecay6He4He2nTransferEventHeader : public ERDecayMCEventHeader
{
private:
  // binary reaction:
  TLorentzVector fHe6_beam;
  TLorentzVector fHe4_target;
  TLorentzVector fHe6;
  TLorentzVector fHe4;

  // decay of excited 6He in LAB:
  TLorentzVector fHe4Decay;
  TLorentzVector fn1Decay;
  TLorentzVector fn2Decay;

  // decay of excited 6He in CM of decay:
  TLorentzVector fHe4DecayCM;
  TLorentzVector fn1DecayCM;
  TLorentzVector fn2DecayCM;

  Float_t fE_T = -1.;
  TLorentzVector fNNdecayCM;

  Int_t fTrigger = 0;
  Int_t fTriggerPriority = 0;
  Float_t fTime = -1.;
  Float_t fThetaCM = -1.;

public:
  ERDecay6He4He2nTransferEventHeader() : fE_T(0),
                                         fTrigger(0), fTriggerPriority(0),
                                         fTime(-1.), fThetaCM(-1.) {}
  void SetData(const TVector3 &position,
               const TLorentzVector &beam, const TLorentzVector &target,
               const TLorentzVector &He6, const TLorentzVector &He4,
               const TLorentzVector &He4Decay, const TLorentzVector &n1Decay, const TLorentzVector &n2Decay,
               const TLorentzVector &He4DecayCM, const TLorentzVector &n1DecayCM, const TLorentzVector &n2DecayCM,
               const TLorentzVector &NNsystem, const Float_t E_T,
               const Float_t time, const Float_t thetaCM);

  void SetTrigger(Int_t trigger) { fTrigger = trigger; }

  TLorentzVector GetBeam() const { return fHe6_beam; }
  TLorentzVector GetTarget() const { return fHe4_target; }
  TLorentzVector GetHe6() const { return fHe6; }
  TLorentzVector GetHe4() const { return fHe4; }

  TLorentzVector GetHe4Decay() const { return fHe4Decay; }
  TLorentzVector GetN1Decay() const { return fn1Decay; }
  TLorentzVector GetN2Decay() const { return fn2Decay; }

  TLorentzVector GetHe4DecayCM() const { return fHe4DecayCM; }
  TLorentzVector GetN1DecayCM() const { return fn1DecayCM; }
  TLorentzVector GetN2DecayCM() const { return fn2DecayCM; }

  Int_t GetTrigger() const { return fTrigger; }
  Int_t GetTriggerPriority() const { return fTriggerPriority; }
  Float_t GetTime() const { return fTime; }
  Float_t GetThetaCM() const { return fThetaCM; }

  void Clear();

  ClassDef(ERDecay6He4He2nTransferEventHeader, 1)
};

#endif