// -------------------------------------------------------------------------
// -----           ERCave header file                                  -----
// -----           Created 11/12/15 by V.Schetinin                     -----
// -------------------------------------------------------------------------

#ifndef ERCAVE_H
#define ERCAVE_H

#include "FairModule.h"

class ERCave : public FairModule
{
public:

	ERCave();
	ERCave(const char* name, const char* title="Expert Cave");

	virtual ~ERCave();

	virtual void ConstructGeometry();

private:

	Double_t world[3]; //! TODO what is it?

	ClassDef(ERCave,1)
};

#endif // ERCAVE_H
