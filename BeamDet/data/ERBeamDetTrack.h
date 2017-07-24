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
//	Double_t  fX1, fY1, fZ1;          // point coordinates in first MWPC station
//  Double_t  fX2, fY2, fZ2;          // point coordinates in second MWPC station
  Double_t  fXt, fYt, fZt;          // point coordinates on target
  TVector3  fVectorOnTarget;
public:
	ERBeamDetTrack();
  ERBeamDetTrack(TString name);
  ERBeamDetTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v);
  void AddParameters(Double_t xt, Double_t yt, Double_t zt, TVector3 v);

	ClassDef(ERBeamDetTrack, 1)
};

#endif
