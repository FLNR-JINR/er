// -------------------------------------------------------------------------
// -----                        ERDSRDHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERDSRDHitFinder_H
#define ERDSRDHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERDSRDHit.h"

class ERDSRDHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERDSRDHitFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERDSRDHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERDSRDHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetElossSigma(Float_t sigma) {fElossSigma = sigma;} 
  void SetTimeSigmaPar(Float_t sigma) {fTimeSigmaPar = sigma;}
  /** Accessors **/ 
  Float_t ElossSigma() const {return fElossSigma;}
  Float_t TimeSigmaPar() const {return fTimeSigmaPar;}
protected:
  //Input arrays
  TClonesArray *fDSRDPoints;
  //Output arrays
  TClonesArray *fDSRDHits;

  static Int_t fEvent;
  Float_t fElossSigma;
  Float_t fTimeSigmaPar;
protected:
  ERDSRDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERDSRDHitFinder,1)
};

#endif
