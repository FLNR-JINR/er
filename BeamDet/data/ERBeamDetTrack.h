// -------------------------------------------------------------------------
// -----                        ERBeamDetTrack header file                -----
// -----                     Created   by              -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetTrack_H
#define ERBeamDetTrack_H

class ERBeamDetTrack : public TObject{
private:
	Double_t fX1, fY1, fZ1;          // point coordinates in first MWPC station
  Double_t fX2, fY2, fZ2;          // point coordinates in second MWPC station
  Double_t fXt, fYt, fZt;          // point coordinates on target
public:
	ERBeamDetTrack(){}

	ClassDef(ERBeamDetTrack, 1)
};

#endif
