// -------------------------------------------------------------------------
// -----                        ERNDHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNDHitFinder_H
#define ERNDHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNDHit.h"
#include "ERNDSetup.h"

class ERNDHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERNDHitFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNDHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERNDHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetLYDispersionA(Float_t sigma) {fLYDispersionA = sigma;} 
  void SetLYDispersionB(Float_t sigma) {fLYDispersionB = sigma;} 
  void SetTimeDispersionPar(Float_t sigma) {fTimeDispersionPar = sigma;}
  void SetQuenchThreshold(Float_t th) {fQuenchThreshold = th;}
  void SetLYThreshold(Float_t th) {fLYThreshold = th;}
  void SetProbabilityB(Float_t b) {fProbabilityB = b;}
  void SetProbabilityC(Float_t c) {fProbabilityC = c;}
  /** Accessors **/ 
  Float_t LYDispersionA() const {return fLYDispersionA;}
  Float_t LYDispersionB() const {return fLYDispersionB;}
  Float_t TimeDispersionPar() const {return fTimeDispersionPar;}
  Float_t ElossThreshold() const {return fQuenchThreshold;}
  Float_t LYThreshold() const {return fLYThreshold;}
  Float_t ProbabilityB() const {return fProbabilityB;}
  Float_t ProbabilityC() const {return fProbabilityC;}
protected:
  //Input arrays
  TClonesArray *fNDPoints;
  //Output arrays
  TClonesArray *fNDHits;

  static Int_t fEvent;
  Float_t fLYDispersionA;
  Float_t fLYDispersionB;
  Float_t fTimeDispersionPar;
  Float_t fQuenchThreshold;
  Float_t fLYThreshold;
  Float_t fProbabilityB;
  Float_t fProbabilityC;

  ERNDSetup* fSetup;
protected:
  ERNDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, 
          Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNDHitFinder,1)
};

#endif
