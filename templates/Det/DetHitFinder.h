
#ifndef DetHitFinder_H
#define DetHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "DetHit.h"

class DetHitFinder : public FairTask {

public:
  DetHitFinder();

  DetHitFinder(Int_t verbose);

  ~DetHitFinder();

  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);

  virtual void Finish();

  virtual void Reset();
protected:
  //Input arrays
  TClonesArray *fPoints;
  //Output arrays
  TClonesArray *fHits;
protected:
  DetHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(DetHitFinder,1)
};

#endif
