#ifndef ExperCave_H
#define ExperCave_H

#include "FairDetector.h"
#include "FairModule.h"


class ExperCave : public FairModule {
  public:
    ExperCave(const char * name, const char *Title="Expert Cave");
    ExperCave();
    virtual ~ExperCave();
    virtual void ConstructGeometry();
    

private:
    // Double_t world[3]; //!
     ClassDef(ExperCave,1) //ExperCaveSD
};

#endif //ExperCave_H

