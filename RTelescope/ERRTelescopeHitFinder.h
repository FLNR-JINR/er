// -------------------------------------------------------------------------
// -----                        ERRTelescopeHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeHitFinder_H
#define ERRTelescopeHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERRTelescopeHit.h"
#include "ERRTelescopeSetup.h"

class ERRTelescopeHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERRTelescopeHitFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERRTelescopeHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERRTelescopeHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetElossDispersion(Float_t sigma) {fElossDispersion = sigma;} 
  void SetTimeDispersionPar(Float_t sigma) {fTimeDispersionPar = sigma;}
  void SetElossThreshold(Float_t th){fElossThreshold = th;}
  /** Accessors **/ 
  Float_t ElossDispersion() const {return fElossDispersion;}
  Float_t TimeDispersionPar() const {return fTimeDispersionPar;}
protected:
  //Input arrays
  TClonesArray *fDSRDPoints;
  //Output arrays
  TClonesArray *fDSRDHits;

  static Int_t fEvent;
  Float_t fElossDispersion;
  Float_t fTimeDispersionPar;
  Float_t fElossThreshold;

  ERRTelescopeSetup* fDSRDSetup;
protected:
  ERRTelescopeHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERRTelescopeHitFinder,1)
};

#endif
