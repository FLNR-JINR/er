#include "ERGadastContFact.h"

#include "ERGadastDigiPar.h"
#include "ERGadastGeoPar.h"

#include "FairRuntimeDb.h"

#include <iostream>
using namespace std;

static ERGadastContFact gERGadastContFact;

ERGadastContFact::ERGadastContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="ERGadastContFact";
  fTitle="Factory for parameter containers in libTutorial1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void ERGadastContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the Tutorial1 library.
  */

  FairContainer* p1= new FairContainer("ERGadastGeoPar",
                                       "ERGadastGeoPar",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);

  FairContainer* p2= new FairContainer("ERGadastDigiPar",
                                       "ERGadastDigiPar",
                                       "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");

  containers->Add(p2);
}

FairParSet* ERGadastContFact::createContainer(FairContainer* c)
{
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"ERGadastGeoPar")==0) {
    p=new ERGadastGeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"ERGadastDigiPar")==0) {
    p=new ERGadastDigiPar(c->getConcatName().Data(),
                                  c->GetTitle(),c->getContext());
  }
  return p;
}

ClassImp(ERGadastContFact)
