// -------------------------------------------------------------------------
// -----                        ERNeuRadHitProducerWBT header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitProducerWBT_H
#define ERNeuRadHitProducerWBT_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHitWBT.h"

class ERNeuRadHitProducerWBT : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitProducerWBT();

  ERNeuRadHitProducerWBT(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitProducerWBT();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  /** Accessors **/
protected:
  //Input arrays
  TClonesArray *fNeuRadPMTSignals;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

protected:
  ERNeuRadHitWBT* AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                         Int_t  BundleIndex, Int_t FiberIndex, Float_t time, Float_t qInteg);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitProducerWBT,1)
};

#endif
