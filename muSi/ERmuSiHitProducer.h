// -------------------------------------------------------------------------
// -----                        ERmuSiHitProducer header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIHITPRODUCING_H
#define ERMUSIHITPRODUCING_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiHitProducer : public FairTask {

public:
  /** Default constructor **/
  ERmuSiHitProducer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERmuSiHitProducer(Int_t verbose);

  /** Destructor **/
  ~ERmuSiHitProducer();

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
  TClonesArray *fmuSiPoints;
  //Output arrays
  TClonesArray *fmuSiHits;

  static Int_t fEvent;
protected:
  ERmuSiHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station);
private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiHitProducer,1)
};

#endif
