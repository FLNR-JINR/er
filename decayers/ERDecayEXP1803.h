/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecayEXP1803_H
#define ERDecayEXP1803_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecayEXP1803 : public ERDecay{

public:
  ERDecayEXP1803();
  ~ERDecayEXP1803();

  /*Modifiers*/
  void SetMinStep(Double_t minStep) {fMinStep = minStep;}
  void SetTargetVolumeName(TString volumeName) {fVolumeName = volumeName;}
  void SetTargetThickness(Double_t targetThickness) {fTargetThickness = targetThickness;}

public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:
	TRandom3       *fRnd;
	
	TParticlePDG   *f6He;
	TParticlePDG   *f2H;
  TParticlePDG   *f3He;
  TParticlePDG   *f5H;
  TParticlePDG   *f3H;
  TParticlePDG   *fn;

  TGenPhaseSpace *fReactionPhaseSpace;
  TGenPhaseSpace *fDecayPhaseSpace;
  TString         fVolumeName;
  Double_t        fTargetReactZ;
  Double_t        fMinStep;
  Double_t        fTargetThickness;
	Bool_t          fDecayFinish;

	ClassDef(ERDecayEXP1803,1)
};

#endif
