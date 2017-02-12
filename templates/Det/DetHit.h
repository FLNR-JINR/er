
#ifndef DetHit_H
#define DetHit_H

#include "FairHit.h"

class DetHit : public FairHit{
private:
	Int_t fID;
	Float_t fEloss;
	Float_t fTime;
public:
	DetHit(){}
	DetHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);

	Int_t ID() const {return fID;}
	Float_t Eloss() const {return fEloss;}
	Float_t Time() const {return fTime;}


	ClassDef(DetHit, 1)

};

#endif

