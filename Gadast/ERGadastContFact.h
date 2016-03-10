// -------------------------------------------------------------------------
// -----                  ERGadastContFact header file             	   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastContFact_H
#define ERGadastContFact_H

#include "FairContFact.h"

class FairContainer;

class ERGadastContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    ERGadastContFact();
    ~ERGadastContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( ERGadastContFact,0)
};

#endif  /* !ERNeuRadContFact_H */