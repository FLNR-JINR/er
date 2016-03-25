// -------------------------------------------------------------------------
// -----                ERGenerator header file                   -----
// -----          Created 02/14  by V.Schetinin                        -----
// -------------------------------------------------------------------------

#ifndef ERGENERATOR_H
#define ERGENERATOR_H 1


#include "FairGenerator.h"              //for FairGenerator
#include "FairIon.h"                    //for FairIon
#include "FairPrimaryGenerator.h"       //for FairPrimaryGenerator

#include "TRandom3.h"
#include "TGenPhaseSpace.h"       //for TGenPhaseSpace 

#include <iostream>

//using namespace std;


class ERGenerator : public FairGenerator
{

 public:

  /** Default constructor without arguments should not be used. **/
  ERGenerator();

  ERGenerator(Int_t z, Int_t a, Int_t q,
              Double_t px, Double_t py, Double_t pz,
              Double_t vx, Double_t vy, Double_t vz);

  /** Destructor. **/
  virtual ~ERGenerator();

  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
  **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  /*
  void SetBeamEnergy(Double_t en){fEBeam = en;}
  void SetGammaCMEnergy(Double_t en){fEGammaCM = en;}
  void SetGammaCMPhiRange(Double_t phiMin, Double_t phiMax){fPhiMinCM = phiMin; fPhiMaxCM= phiMax;}
  void SetGammaCMThetaRange(Double_t thetaMin, Double_t thetaMax){fThetaMinCM = thetaMin; fThetaMaxCM= thetaMax; }
  void SetIon(Int_t z, Int_t a){fA = a; fZ = z;}
  */

  //Skip some events in file
  Bool_t SkipEvents(Int_t count);
   
 private:
  Double_t fEBeam;
  Double_t fEGammaCM;
  Double_t fPhiMinCM, fPhiMaxCM;
  Double_t fThetaMinCM, fThetaMaxCM;
  Double_t fPrimaryIonMass;
  Int_t fGammaPDG;

  static Int_t fgNIon;     //! Number of the instance of this class
  FairIon*   fPrimaryIon;          // Pointer to the FairIon to be generated
  FairIon*   fSecondIon;
  FairIon*   fThirdIon;
  Double_t fPx, fPy, fPz;  // Momentum components [GeV] per nucleon
  Double_t fVx, fVy, fVz;  // Vertex coordinates [cm]
  TGenPhaseSpace* fPHSpace;
  TRandom3 *fRnd;

  ERGenerator(const ERGenerator&);
  ERGenerator& operator=(const ERGenerator&);

  ClassDef(ERGenerator,1);
 
};

#endif
   
