#ifndef FAIRTUTORIALDET2CONTFACT_H
#define FAIRTUTORIALDET2CONTFACT_H

#include "FairContFact.h"

class FairContainer;

class ERNeuRadContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    ERNeuRadContFact();
    ~ERNeuRadContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( ERNeuRadContFact,0)
};

#endif  /* !FAIRTUTORIALDETCONTFACT_H */
