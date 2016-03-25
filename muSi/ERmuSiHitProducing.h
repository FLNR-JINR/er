// -------------------------------------------------------------------------
// -----                        ERmuSiHitProducing header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIHITPRODUCING_H
#define ERMUSIHITPRODUCING_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiHitProducing : public FairTask {

public:
  /** Default constructor **/
  ERmuSiHitProducing();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERmuSiHitProducing(Int_t verbose);

  /** Destructor **/
  ~ERmuSiHitProducing();

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
protected:
  ERmuSiHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index);
private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiHitProducing,1)
};

#endif
