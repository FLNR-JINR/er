/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecay2H_6Li_H_
#define ERDecay2H_6Li_H_

#include <vector>
#include <fstream>

#include "TGraph.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TRandom2.h"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"

#include "FairIon.h"

#include "ERDecay.h"

class ERDecay2H_6Li : public ERDecay {

public:
  ERDecay2H_6Li();
  ~ERDecay2H_6Li();

  /*Modifiers*/
  void SetMinStep(Double_t minStep) {fMinStep = minStep;}
  void SetTargetThickness(Double_t targetThickness) {fTargetThickness = targetThickness;}
  void Set4nMass(Double_t mass) {f4nMass = mass; fIs4nUserMassSet = true;}
  void Set4nExitation(Double_t excMean, Double_t fwhm, Double_t distibWeight);
  void SetDecayFile(const TString& filePath, Double_t excitationEnergyInFile /*[GeV]*/){ fDecayFilePath = filePath; }

  /** @brief Sets distribution is contained in file.
   ** @param ADfile  file with angular distribution.
  **/  
  void SetAngularDistribution(TString ADfile);   

public:
  Bool_t Init();
  Bool_t Stepping();

  void BeginEvent();
  void FinishEvent();

private:
  /** @brief Body reaction in phase space approach.
     ** @param Ecm     Total energy in CM.
    ** @oaram h7Mass  H7 ion mass.
  **/
  void ReactionPhaseGenerator(Double_t Ecm, Double_t h7Mass);

  Bool_t DecayPhaseGenerator(Double_t excitation);

  std::vector<TLorentzVector> ReadDecayEvent();

private:
  TRandom3       *fRnd;
  TRandom3       *fRnd2;
  
  TParticlePDG   *f8He;
  TParticlePDG   *f2H;
  TParticlePDG   *f6Li;
  // TParticlePDG   *f4n;
  TParticlePDG   *fn;

  TLorentzVector *fLv6Li; //!
  TLorentzVector *fLv4n; //!
  TLorentzVector *fLvn1; //!
  TLorentzVector *fLvn2; //!
  TLorentzVector *fLvn3; //!
  TLorentzVector *fLvn4; //!
  Float_t fTheta; //!

  FairIon        *fIon6Li;
  FairIon        *fUnstable4n;

  TGenPhaseSpace  *fReactionPhaseSpace;
  TGenPhaseSpace  *fDecayPhaseSpace;
  Double_t         fTargetReactZ;
  Double_t         fMinStep;
  Double_t         fTargetThickness;
  Bool_t           fDecayFinish;

  std::vector<Double_t> f4nExcitationMean;
  std::vector<Double_t> f4nExcitationSigma; 
  std::vector<Double_t> f4nExcitationWeight;

  Double_t        f4nMass;
  Bool_t          fIs4nUserMassSet;
  Bool_t          fIs4nExcitationSet;

  TString         fDecayFilePath;
  Double_t        fDecayFileExcitation = 1. /*[GeV]*/;
  Bool_t          fDecayFileFinished;
  Int_t           fDecayFileCurrentEvent;
  std::ifstream   fDecayFile;

  TGraph *fADInput = nullptr;    //!   distribution (angular distribution) graph containing AD input
  TF1    *fADFunction = nullptr; //!   function describing AD (angular distribution) of binary reaction
  Double_t fThetaMin = 0.;
  Double_t fThetaMax = 0.;

  //ADEvaluate function is necessary for TF1 constructor
  Double_t ADEvaluate(Double_t *x, Double_t *p);

  ClassDef(ERDecay2H_6Li,1)
};

#endif
