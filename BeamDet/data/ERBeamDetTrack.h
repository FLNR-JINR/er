// -------------------------------------------------------------------------
// -----                        ERBeamDetTrack header file                -----
// -----                     Created   by              -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetTrack_H
#define ERBeamDetTrack_H

#include "TString.h"
#include "TNamed.h"
#include "TVector3.h"

class ERBeamDetTrack : public TNamed{
private:
  Double_t  fXt, fYt, fZt;          // point coordinates on target
  TVector3  fVectorOnTarget;
public:
  ERBeamDetTrack();
  ERBeamDetTrack(TString name);
  ERBeamDetTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v);
  void AddParameters(Double_t xt, Double_t yt, Double_t zt, TVector3 v);

  TVector3 GetVector(void) const {return fVectorOnTarget;}
  TVector3 GetTargetVertex() const {return TVector3(fXt,fYt,fZt);}

	ClassDef(ERBeamDetTrack, 1)
};

#endif
