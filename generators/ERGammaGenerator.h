// -------------------------------------------------------------------------
// -----                ERGammaGenerator header file                   -----
// -----          Created 02/14  by V.Schetinin                        -----
// -------------------------------------------------------------------------

#ifndef ERGAMMAGENERATOR_H
#define ERGAMMAGENERATOR_H 1


#include "FairGenerator.h"
#include "FairPrimaryGenerator.h"

#include "TRandom3.h"

#include <iostream>

//using namespace std;


class ERGammaGenerator : public FairGenerator
{

 public:

  /** Default constructor without arguments should not be used. **/
  ERGammaGenerator();

  /** Destructor. **/
  virtual ~ERGammaGenerator();

  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param pStack    pointer to the stack
   ** @param ver       not used
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  void SetBeamEnergy(Double_t en){fEBeam = en;}
  void SetGammaCMEnergy(Double_t en){fEGammaCM = en;}
  void SetGammaCMPhiRange(Double_t phiMin, Double_t phiMax){fPhiMinCM = phiMin; fPhiMaxCM= phiMax;}
  void SetGammaCMThetaRange(Double_t thetaMin, Double_t thetaMax);
  void SetIon(Int_t z, Int_t a){fA = a; fZ = z;}

  //Skip some events in file
  Bool_t SkipEvents(Int_t count);
   
 private:
  Int_t fA;
  Int_t fZ;
  Double_t fEBeam;
  Double_t fEGammaCM;
  Double_t fPhiMinCM, fPhiMaxCM;
  Double_t fThetaMinCM, fThetaMaxCM;
  Double_t fIonMass;
  Int_t fGammaPDG;

  TRandom3 *fRnd;

  ERGammaGenerator(const ERGammaGenerator&);
  ERGammaGenerator& operator=(const ERGammaGenerator&);

  ClassDef(ERGammaGenerator,1);
 
};

#endif
   
