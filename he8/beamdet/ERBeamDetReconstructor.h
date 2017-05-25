// -------------------------------------------------------------------------
// -----                  ERBeamDetReconstructor header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetReconstructor_H
#define ERBeamDetReconstructor_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERBeamDetEvent.h"
#include "ERBeamDetCalEvent.h"
#include "ERBeamDetRecoEvent.h"

class ERBeamDetReconstructor : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetReconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERBeamDetReconstructor(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetReconstructor();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

protected:
  //Input objects
  ERBeamDetEvent* fInEvent;
  ERBeamDetCalEvent* fInCalEvent;

  //Output object
  ERBeamDetRecoEvent* fOutEvent;

  Int_t fEvent;
private:

  virtual void SetParContainers();
  
  ClassDef(ERBeamDetReconstructor,1)
};

#endif
