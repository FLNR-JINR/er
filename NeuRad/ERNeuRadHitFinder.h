// -------------------------------------------------------------------------
// -----                        ERNeuRadHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinder_H
#define ERNeuRadHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitFinder();

  ERNeuRadHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  virtual void SetPixelThreshold(Float_t pixelThreshold){fPixelThreshold=pixelThreshold;}
  /** Accessors **/
protected:
  //Input arrays
  TClonesArray *fNeuRadPMTSignals;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

  Float_t fPixelThreshold;

protected:
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  ModuleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinder,1)
};

#endif
