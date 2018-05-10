/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TString.h"
#include "TGenPhaseSpace.h"
#include "TDatabasePDG.h"               //for TDatabasePDG
#include "TVector3.h"

class ERDecay{
protected:
	TString fName;
public:
	ERDecay(TString name);
	~ERDecay();
  /*Accessors*/
	TString GetName() const {return fName;}
  /*Modifiers*/
  void SetInteractionVolumeName(TString name) {fInteractionVolumeName = name;}
  void SetNuclearInteractionLenght(Double_t lambda) {fNuclearInteractionLenght = lambda;}
  
  /** @brief Defines maximum path lenght for particles in the volume that is defined 
             in SetInteractionVolumeName().
   ** @param pathLenght  maximum path lenght in target volume
  **/  
  void SetMaxPathLenght(Double_t pathLenght);

  virtual Bool_t Stepping() = 0;
  virtual void BeginEvent() = 0;
  virtual void FinishEvent() = 0;
  virtual Bool_t Init() = 0;

protected:
  Bool_t FindInteractionPoint();
  void   CalculateTargetParameters();
  
  Bool_t   fIsInterationPointFound;
  TVector3 fInteractionPoint;
  TVector3 fInputPoint;
  Double_t fStepToInteractPoint;
  Double_t fDistanceFromEntrance;

private:
  TString  fInteractionVolumeName;
  Double_t fNuclearInteractionLenght;
  Double_t fInteractionProbability;
  Bool_t   fIsInteraction;
  Double_t fTargetBoundBoxDiagonal;
  Double_t fNormalizingProbability;
  Double_t fMaxPathLenght;
  TRandom3 *fRnd1;
  TRandom3 *fRnd2;

	ClassDef(ERDecay,1)
};

#endif
