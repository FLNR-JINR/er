/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecay2H_3He3H_H_
#define ERDecay2H_3He3H_H_

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

class ERDecay2H_3He3H : public ERDecay {

public:
  ERDecay2H_3He3H();
  ~ERDecay2H_3He3H();

  /*Modifiers*/
  void SetMinStep(Double_t minStep) {fMinStep = minStep;}
  void SetTargetThickness(Double_t targetThickness) {fTargetThickness = targetThickness;}
  void Set4nMass(Double_t mass) {f4nMass = mass; fIs4nUserMassSet = true;}
  void Set4nExitation(Double_t excMean, Double_t fwhm, Double_t distibWeight);
  void Set6LiExitation(Double_t excMean, Double_t fwhm, Double_t distibWeight);  
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
  void ReactionPhaseGenerator(Double_t Ecm, Double_t h7Mass, Double_t exc);

  Bool_t DecayPhaseGenerator(Double_t excitation);
  Bool_t Decay6LiPhaseGenerator(Double_t excitation);

  std::vector<TLorentzVector> ReadDecayEvent();

private:
  TRandom3       *fRnd;
  TRandom3       *fRnd2;
  
  TParticlePDG   *f8He;
  TParticlePDG   *f2H;
  TParticlePDG   *f6Li;
  TParticlePDG   *f3He;
  TParticlePDG   *f3H;
  // TParticlePDG   *f4n;
  TParticlePDG   *fn;

  TLorentzVector *fLv6Li; //!
  TLorentzVector *fLv3He; //!
  TLorentzVector *fLv3H; //!
  TLorentzVector *fLv4n; //!
  TLorentzVector *fLvn1; //!
  TLorentzVector *fLvn2; //!
  TLorentzVector *fLvn3; //!
  TLorentzVector *fLvn4; //!
  Float_t fTheta; //!

  FairIon        *fUnstable6Li;
  FairIon        *fUnstable4n;
  FairIon        *fIon3He;
  FairIon        *fIon3H;

  TGenPhaseSpace  *fReactionPhaseSpace;
  TGenPhaseSpace  *fDecayPhaseSpace;
  // TGenPhaseSpace  *fReactionPhaseSpace;
  TGenPhaseSpace  *fDecay6LiPhaseSpace;  
  Double_t         fTargetReactZ;
  Double_t         fMinStep;
  Double_t         fTargetThickness;
  Bool_t           fDecayFinish;
  Bool_t           fDecay6LiFinish;

  std::vector<Double_t> f4nExcitationMean;
  std::vector<Double_t> f4nExcitationSigma; 
  std::vector<Double_t> f4nExcitationWeight;

  std::vector<Double_t> f6LiExcitationMean;
  std::vector<Double_t> f6LiExcitationSigma; 
  std::vector<Double_t> f6LiExcitationWeight;

  Double_t        f6LiMass;
  Double_t        f4nMass;
  Bool_t          fIs6LiUserMassSet;
  Bool_t          fIs6LiExcitationSet;  
  Bool_t          fIs4nUserMassSet;
  Bool_t          fIs4nExcitationSet;

  TString         fDecayFilePath;
  Double_t        fDecayFileExcitation = 1. /*[GeV]*/;
  Bool_t          fDecayFileFinished;
  Int_t           fDecayFileCurrentEvent;
  std::ifstream   fDecayFile;

  TString         fDecay6LiFilePath;
  Double_t        fDecay6LiFileExcitation = 1. /*[GeV]*/;
  Bool_t          fDecay6LiFileFinished;
  Int_t           fDecay6LiFileCurrentEvent;
  std::ifstream   fDecay6LiFile;

  TGraph *fADInput = nullptr;    //!   distribution (angular distribution) graph containing AD input
  TF1    *fADFunction = nullptr; //!   function describing AD (angular distribution) of binary reaction
  Double_t fThetaMin = 0.;
  Double_t fThetaMax = 0.;

  //ADEvaluate function is necessary for TF1 constructor
  Double_t ADEvaluate(Double_t *x, Double_t *p);

  ClassDef(ERDecay2H_3He3H,1)
};

#endif
