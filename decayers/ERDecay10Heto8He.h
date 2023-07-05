/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDecay10Heto8He_H
#define ERDecay10Heto8He_H

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

class ERDecay10Heto8He : public ERDecay {

public:
  ERDecay10Heto8He();
  ~ERDecay10Heto8He();
  
  /*Modifiers*/
  void SetMinStep(Double_t minStep) {fMinStep = minStep;}
  void SetTargetThickness(Double_t targetThickness) {fTargetThickness = targetThickness;}
  void SetHe10Mass(Double_t mass) {f10HeMass = mass; fIs10HeUserMassSet = true;}
  void SetHe10Exitation(Double_t excMean, Double_t fwhm, Double_t distibWeight);
  void SetDecayFile(const TString& filePath, Double_t excitationEnergyInFile /*[GeV]*/){ fDecayFilePath = filePath; }
   
 /** @brief Sets distribution is contained in file.
   ** @param ADfile  file with angular distribution.
  **/

void SetAngularDistribution(TString ADfile);   

public:
  Bool_t Init();
  Bool_t Stepping();
  Bool_t Stepping_main();
  // Bool_t Init() { return true; } 
  // Bool_t Stepping() { return true; } 


 void BeginEvent();
 void FinishEvent();
 // void PrintMass();

private:
  /** @brief Body reaction in phase space approach.
     ** @param Ecm     Total energy in CM.
    ** @oaram h7Mass  H7 ion mass.
  **/
   void ReactionPhaseGenerator(Double_t Ecm, Double_t he10Mass); 

   Bool_t DecayPhaseGenerator(Double_t excitation);

  std::vector<TLorentzVector> ReadDecayEvent();

private:
  TRandom3       *fRnd;
  TRandom3       *fRnd2;
  
  TParticlePDG   *f8He;
  TParticlePDG   *f10He;
  TParticlePDG   *f3H;
  TParticlePDG   *f1H;
  TParticlePDG   *fn;
     //TParticlePDG   *f6He;

  
  TLorentzVector *fLv10He; //!
  TLorentzVector *fLv1H; //!
  TLorentzVector *fLv8Hed; //!	8He decay
  TLorentzVector *fLvn1; //!
  TLorentzVector *fLvn2; //!
 
//  FairIon        *fIon1H;
  FairIon        *fUnstableIon10He;

  TGenPhaseSpace  *fReactionPhaseSpace;
  TGenPhaseSpace  *fDecayPhaseSpace;
  Double_t         fTargetReactZ;
  Double_t         fMinStep;
  Double_t         fTargetThickness;
  Bool_t           fDecayFinish;

  std::vector<Double_t> f10HeExcitationMean;
  std::vector<Double_t> f10HeExcitationSigma; 
  std::vector<Double_t> f10HeExcitationWeight;

  Double_t        f10HeMass;
  Bool_t          fIs10HeUserMassSet;
  Bool_t          fIs10HeExcitationSet;

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
  
  

  ClassDef(ERDecay10Heto8He,1);
};

#endif
