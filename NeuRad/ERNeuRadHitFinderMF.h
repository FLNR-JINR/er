// -------------------------------------------------------------------------
// -----                        ERNeuRadHitFinderMF header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinderMF_H
#define ERNeuRadHitFinderMF_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitFinderMF : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitFinderMF();

  ERNeuRadHitFinderMF(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinderMF();

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
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  ModuleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinderMF,1)
};

#endif
