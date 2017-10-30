// -------------------------------------------------------------------------
// -----                  ERNeuRadHitFinderWBT header file             -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinderWBT_H
#define ERNeuRadHitFinderWBT_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHitWBT.h"

class ERNeuRadHitFinderWBT : public FairTask {

public:

  /** Default constructor **/
  ERNeuRadHitFinderWBT();

  ERNeuRadHitFinderWBT(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinderWBT();

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
                         Int_t  ModuleIndex, Int_t FiberIndex, Float_t time, Float_t qInteg);

private:

  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinderWBT,1)
};

#endif // ERNeuRadHitFinderWBT_H
