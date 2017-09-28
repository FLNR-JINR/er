
#ifndef ERHe10Reconstructor_H
#define ERHe10Reconstructor_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERTelescopeRecoEvent.h"
#include "ERBeamDetRecoEvent.h"
#include "ERHe10RecoEvent.h"

class ERHe10Reconstructor : public FairTask {

public:
  /** Default constructor **/
  ERHe10Reconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERHe10Reconstructor(Int_t verbose);

  /** Destructor **/
  ~ERHe10Reconstructor();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetMisMass(Float_t mass){fMisMass = mass;}
  /** Accessors **/ 
protected:
  ERTelescopeRecoEvent* fRTelescopeEvent;
  ERBeamDetRecoEvent* fBeamDetEvent;
  ERHe10RecoEvent* fOutEvent;

protected:

private:
  virtual void SetParContainers();
  Float_t fMisMass;
  
  ClassDef(ERHe10Reconstructor,1)
};

#endif
