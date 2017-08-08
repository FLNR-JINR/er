// -------------------------------------------------------------------------
// -----                  ERTelescopeReconstructorNew header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTelescopeReconstructorNew_H
#define ERTelescopeReconstructorNew_H

#include "TRandom.h"
#include "TVector3.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoCompositeShape.h"

#include "FairTask.h"

#include "ERQTelescopeCalEvent.h"
#include "ERRTelescopeCalEvent.h"
#include "ERHe8EventHeader.h"
#include "ERParticle.h"
#include "ERBeamDetRecoEvent.h"
#include "ERTelescopeRecoEvent.h"

class ERTelescopeReconstructorNew : public FairTask {

public:
  /** Default constructor **/
  ERTelescopeReconstructorNew();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERTelescopeReconstructorNew(Int_t verbose);

  /** Destructor **/
  ~ERTelescopeReconstructorNew();

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
  ERQTelescopeCalEvent* fQTelescopeEvent;
  ERRTelescopeCalEvent* fRTelescopeEvent;
  ERBeamDetRecoEvent* fBeamDetEvent;

  //Output objects
  ERTelescopeRecoEvent* fOutEvent;

  Int_t fEvent;
  //particles
  ERParticle* fProjectile;
  ERParticle* fTarget;
  ERParticle* fCM0;

private:

  virtual void SetParContainers();
  
  ClassDef(ERTelescopeReconstructorNew,1)
};

#endif
