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
public:
	ERmuSiVertex();
	void AddTrack(Int_t id);
	Int_t TrackNb(){return fTracksNb;}
	Int_t Track(Int_t id);

	ClassDef(ERmuSiVertex, 1)
};

#endif

