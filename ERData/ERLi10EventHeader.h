// -------------------------------------------------------------------------
// -----                        ERLi10EventHeader header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERLi10EventHeader_H
#define ERLi10EventHeader_H

#include "FairEventHeader.h"

class ERLi10EventHeader : public FairEventHeader {
private:
	Float_t fPEnergy;
	Float_t fNEnergy;
public:
	ERLi10EventHeader();

	void SetPEnergy(Float_t e) {fPEnergy = e;}
	void SetNEnergy(Float_t e) {fNEnergy = e;}

	ClassDef(ERLi10EventHeader, 1)
};

#endif
