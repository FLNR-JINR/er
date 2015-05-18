// -------------------------------------------------------------------------
// -----                       ERCave header file                      -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#ifndef ERCAVE_H
#define ERCAVE_H

#include "FairDetector.h"
#include "FairModule.h"


class ERCave : public FairModule {
  public:
	ERCave(const char * name, const char *Title="Expert Cave");
	ERCave();
    virtual ~ERCave();
    virtual void ConstructGeometry();
private:
     Double_t world[3]; //!
     ClassDef(ERCave,1)
};

#endif //ERCAVE_H

