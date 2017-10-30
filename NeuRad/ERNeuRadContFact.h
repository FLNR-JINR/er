// -------------------------------------------------------------------------
// -----                  ERNeuRadContFact header file             	   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadContFact_H
#define ERNeuRadContFact_H

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

#endif // ERNeuRadContFact_H
