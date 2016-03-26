// -------------------------------------------------------------------------
// -----                        ERmuSiTrack header file                -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSITRACK_H
#define ERMUSITRACK_H

#include "ERmuSiHit.h"

class ERmuSiTrack : public TObject{
private:
	ERmuSiHit fHits[3];
public:
	ERmuSiTrack(){}

	Int_t AddHit(Int_t station, ERmuSiHit hit);
	ERmuSiHit* Hit(Int_t iStation){return &(fHits[iStation]);}
	ClassDef(ERmuSiTrack, 1)
};

#endif

