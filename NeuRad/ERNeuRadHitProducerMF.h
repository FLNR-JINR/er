// -------------------------------------------------------------------------
// -----                        ERNeuRadHitProducerMF header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitProducerMF_H
#define ERNeuRadHitProducerMF_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitProducerMF : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitProducerMF();

  ERNeuRadHitProducerMF(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitProducerMF();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  inline void SetPixelThreshold(Float_t pixelThreshold){fPixelThreshold = pixelThreshold;}
  inline void SetModuleThreshold(Float_t moduleThreshold){fModuleThreshold = moduleThreshold;}
  /** Accessors **/
protected:
  //Input arrays
  TClonesArray *fNeuRadDigis;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

  Float_t fPixelThreshold;
  Float_t fModuleThreshold;

protected:
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  BundleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitProducerMF,1)
};

#endif
