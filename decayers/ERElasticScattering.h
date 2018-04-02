/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERElasticScattering_H
#define ERElasticScattering_H

#include "TRandom3.h"
#include "TString.h"
#include "TParticlePDG.h"
#include "TF1.h"
#include "TGraph.h"

#include "FairIon.h"

#include "ERDecay.h" // mother class

class ERElasticScattering : public ERDecay
{
public:
  ERElasticScattering(TString name);
  ~ERElasticScattering();

  /*Modifiers*/
  void SetTargetIon(Int_t A, Int_t Z, Int_t Q);
  void SetThetaCDF(TString fileName) {fThetaFileName = fileName;}
  void SetTargetMass(Float_t mass) {fTargetMass = mass;}
  void SetThetaRange(Float_t theta1, Float_t theta2) {fTheta1 = theta1; fTheta2 = theta2;}
  void SetPhiRange(Float_t phi1, Float_t phi2) {fPhi1 = phi1; fPhi2 = phi2;}

public:
  Bool_t Init();
  Bool_t Stepping();

private:
  TString        fThetaFileName;
  Float_t        fTargetMass;
  Float_t        fTheta1,fTheta2;
  Float_t        fCDFmin,fCDFmax;
  Float_t        fPhi1, fPhi2;

  TString        fTargetIonName;
  TParticlePDG*  fTargetIonPDG;

  TF1* fThetaInvCDF;

  Float_t ThetaGen();

  ClassDef(ERElasticScattering,1);
};

#endif // ERElasticScattering_H
