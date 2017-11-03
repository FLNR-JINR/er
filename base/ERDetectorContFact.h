// -------------------------------------------------------------------------
// -----                  ERDetectorContFact header file             	   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERDetectorContFact_H
#define ERDetectorContFact_H

#include "FairContFact.h"

class FairContainer;

class ERDetectorContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    ERDetectorContFact();
    ~ERDetectorContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( ERDetectorContFact,0)
};

#endif  /* !ERDetectorContFact_H */
