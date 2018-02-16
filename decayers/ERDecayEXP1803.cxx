/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecayEXP1803.h"

#include <iostream>

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"
#include "ERMCEventHeader.h"

using namespace std;

ERDecayEXP1803::ERDecayEXP1803():
  ERDecay("EXP1803"),
  fDecayFinish(kFALSE),
  fTargetReactZ(0.),
  fMinStep(0.01),
  f6He(NULL),
  f2H (NULL),
  f3He(NULL),
  f5H (NULL),
  f3H (NULL),
  fn  (NULL),
  fIon3He(NULL),
  f5HMass(0.),
  fIs5HUserMassSet(false)
{
  fRnd = new TRandom3();
  fReactionPhaseSpace = new TGenPhaseSpace();
  fDecayPhaseSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  FairIon* unstableIon5H = new FairIon("5H",  1, 5, 1);
  fIon3He                = new FairIon("3He", 2, 3, 2);
  run->AddNewIon(unstableIon5H);
  run->AddNewIon(fIon3He);
}
//-------------------------------------------------------------------------------------------------
ERDecayEXP1803::~ERDecayEXP1803() {
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1803::Init(){
  f6He = TDatabasePDG::Instance()->GetParticle("6He");
  if ( ! f6He ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 6He not found in database!" << endl;
    return kFALSE;
  }

  f2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! f2H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion Deuteron not found in database!" << endl;
    return kFALSE;
  }

  f5H = TDatabasePDG::Instance()->GetParticle("5H");
  if ( ! f5H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 5H not found in database!" << endl;
    return kFALSE;
  }

  f3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName());
  if ( ! f3He ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 3He not found in database!" << endl;
    return kFALSE;
  }

  f3H = TDatabasePDG::Instance()->GetParticle("Triton");
  if ( ! f3H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion Triton not found in database!" << endl;
    return kFALSE;
  }

  fn = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fn ) {
    std::cerr  << "-W- ERDecayEXP1803: Particle neutron not found in database!" << endl;
    return kFALSE;
  }

  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1803::Stepping() {
  if(!fDecayFinish && gMC->TrackPid() == 1000020060 && TString(gMC->CurrentVolName()).Contains(fVolumeName)){
    gMC->SetMaxStep(fMinStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() > fTargetReactZ){
      // std::cout << "Start reation in target. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
      // 6He + 2H → 3He + 5H
      TLorentzVector lv6He;
      gMC->TrackMomentum(lv6He);
      TLorentzVector lv2H(0., 0., 0., f2H->Mass());
      TLorentzVector lvReaction;
      lvReaction = lv6He + lv2H;

      Double_t mass5H = (fIs5HUserMassSet) ? f5HMass : f5H->Mass();
      LOG(DEBUG) << "Ion H5 mass in reaction " << mass5H << FairLogger::endl;

      Double_t reactMasses[2];
      reactMasses[0] = f3He->Mass();
      reactMasses[1] = mass5H;

      fReactionPhaseSpace->SetDecay(lvReaction, 2, reactMasses);
      fReactionPhaseSpace->Generate();

      TLorentzVector *lv3He = fReactionPhaseSpace->GetDecay(0);
      TLorentzVector *lv5H  = fReactionPhaseSpace->GetDecay(1);

      //5H → f3H + n +n.
      Double_t decayMasses[3];
      decayMasses[0] = f3H->Mass();
      decayMasses[1] = fn->Mass(); 
      decayMasses[2] = fn->Mass();

      fDecayPhaseSpace->SetDecay(*lv5H, 3, decayMasses);
      fDecayPhaseSpace->Generate();

      TLorentzVector *lv3H = fDecayPhaseSpace->GetDecay(0);
      TLorentzVector *lvn1 = fDecayPhaseSpace->GetDecay(1);
      TLorentzVector *lvn2 = fDecayPhaseSpace->GetDecay(2);

      Int_t newTrackNb;

      gMC->GetStack()->PushTrack(1, 0, f5H->PdgCode(),
                                 lv5H->Px(),lv5H->Py(),lv5H->Pz(),
                                 lv5H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, newTrackNb, mass5H, 0);
      gMC->GetStack()->PushTrack(1, 0, f3He->PdgCode(),
                                 lv3He->Px(),lv3He->Py(),lv3He->Pz(),
                                 lv3He->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, newTrackNb, f3He->Mass(), 0);
      gMC->GetStack()->PushTrack(1, 0, f3H->PdgCode(),
                                 lv3H->Px(),lv3H->Py(),lv3H->Pz(),
                                 lv3H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, newTrackNb, f3H->Mass(), 0);
      gMC->GetStack()->PushTrack(1, 0, fn->PdgCode(),
                                 lvn1->Px(),lvn1->Py(),lvn1->Pz(),
                                 lvn1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, newTrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, 0, fn->PdgCode(),
                                 lvn2->Px(),lvn2->Py(),lvn2->Pz(),
                                 lvn2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, newTrackNb, fn->Mass(), 0);
      gMC->StopTrack();
      fDecayFinish = kTRUE;
      gMC->SetMaxStep(100.);

      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetDecayPos(curPos.Vect());
        header->SetInputIon(*lv5H);
        header->AddOutputParticle(*lv3H);
        header->AddOutputParticle(*lvn1);
        header->AddOutputParticle(*lvn2);
      }
    }
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::BeginEvent() {	
  fDecayFinish = kFALSE;
  fTargetReactZ = fRnd->Uniform(-fTargetThickness / 2, fTargetThickness / 2);
  FairRunSim* run = FairRunSim::Instance();
}
//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::FinishEvent() {
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERDecayEXP1803)
