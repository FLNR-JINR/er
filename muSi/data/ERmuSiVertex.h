// -------------------------------------------------------------------------
// -----                        ERmuSiVertex header file                -----
// -----                     Created 04/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSIVERTEX_H
#define ERMUSIVERTEX_H

#include "TObject.h"

class ERmuSiVertex : public TObject{
private:
	Int_t fTracksID[3];
	Int_t fTracksNb;
	Float_t fX, fY, fZ;
public:
	ERmuSiVertex();
	ERmuSiVertex(Float_t x, Float_t y, Float_t z);
	
	void AddTrack(Int_t id);

	Int_t TrackNb() const {return fTracksNb;}
	Int_t Track(Int_t id) const;
	Float_t X() const {return fX;}
	Float_t Y() const {return fY;}
	Float_t Z() const {return fZ;}

	void SetX(Float_t x){fX = x;}
	void SetY(Float_t y){fY = y;}
	void SetZ(Float_t z){fZ = z;}

	ClassDef(ERmuSiVertex, 1)
};

#endif

