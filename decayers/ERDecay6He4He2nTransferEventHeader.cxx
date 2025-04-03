/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecay6He4He2nTransferEventHeader.h"

#include "FairLogger.h"

void ERDecay6He4He2nTransferEventHeader::SetData(const TVector3 &position,
                                                 const TLorentzVector &beam, const TLorentzVector &target,
                                                 const TLorentzVector &He6, const TLorentzVector &He4,
                                                 const TLorentzVector &He4Decay, const TLorentzVector &n1Decay, const TLorentzVector &n2Decay,
                                                 const TLorentzVector &He4DecayCM, const TLorentzVector &n1DecayCM, const TLorentzVector &n2DecayCM,
                                                 const TLorentzVector &NNsystem, const Float_t E_T,
                                                 const Float_t time, const Float_t thetaCM)
{
  fReactionPos = position;

  fHe6_beam = beam;
  fHe4_target = target;
  fHe6 = He6;
  fHe4 = He4;

  fHe4DecayCM= He4DecayCM;
  fn1DecayCM = n1DecayCM;
  fn2DecayCM = n2DecayCM;

  fHe4Decay = He4Decay;
  fn1Decay = n1Decay;
  fn2Decay = n2Decay;

  fNNdecayCM = NNsystem;
  fE_T = E_T;

  fTime = time;
  fThetaCM = thetaCM;
}
// -------------------------------------------------------------------------
void ERDecay6He4He2nTransferEventHeader::Clear()
{
  ERDecayMCEventHeader::Clear();

  fHe6_beam.SetXYZM(0, 0, 0, 0);
  fHe4_target.SetXYZM(0, 0, 0, 0);
  fHe6.SetXYZM(0, 0, 0, 0);
  fHe4.SetXYZM(0, 0, 0, 0);

  fHe4Decay.SetXYZM(0, 0, 0, 0);
  fn1Decay.SetXYZM(0, 0, 0, 0);
  fn2Decay.SetXYZM(0, 0, 0, 0);

  fHe4DecayCM.SetXYZM(0, 0, 0, 0);
  fn1DecayCM.SetXYZM(0, 0, 0, 0);
  fn2DecayCM.SetXYZM(0, 0, 0, 0);

  fNNdecayCM.SetXYZM(0, 0, 0, 0);
  fE_T = -1.;

  fTrigger = 0;
  fTriggerPriority = 0;
  fTime = -1.;
  fThetaCM = -1.;
}
// -------------------------------------------------------------------------

ClassImp(ERDecay6He4He2nTransferEventHeader)