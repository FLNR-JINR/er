// -------------------------------------------------------------------------
// -----           ERGeoCave header file                               -----
// -----           Created 11/12/15  by V.Schetinin                    -----
// -------------------------------------------------------------------------

#ifndef ERGEOCAVE_H
#define ERGEOCAVE_H

#include <fstream>

#include "TString.h"

#include "FairGeoSet.h"

class FairGeoMedia;

class ERGeoCave : public FairGeoSet
{
protected:

	TString name;

public:

	ERGeoCave();
	~ERGeoCave() {}

	const char* getModuleName(Int_t) {return name.Data();}
	Bool_t read(std::fstream&, FairGeoMedia*);
	void addRefNodes();
	void write(std::fstream&);
	void print();

	ClassDef(ERGeoCave,0)
};

#endif // ERGEOCAVE_H
