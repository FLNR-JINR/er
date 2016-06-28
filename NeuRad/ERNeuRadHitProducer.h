// -------------------------------------------------------------------------
// -----                        ERNeuRadHitProducer header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitProducer_H
#define ERNeuRadHitProducer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitProducer : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitProducer();

  ERNeuRadHitProducer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitProducer();

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
  TClonesArray *fNeuRadDigis;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

  Float_t fPixelThreshold;

protected:
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  BundleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitProducer,1)
};

#endif
