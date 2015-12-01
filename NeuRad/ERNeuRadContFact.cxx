#include "ERNeuRadContFact.h"

#include "ERNeuRadGeoPar.h"
#include "ERNeuRadDigiPar.h"

#include "FairRuntimeDb.h"

#include <iostream>
using namespace std;

static ERNeuRadContFact gERNeuRadContFact;

ERNeuRadContFact::ERNeuRadContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="ERNeuRadContFact";
  fTitle="Factory for parameter containers in libTutorial1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void ERNeuRadContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the Tutorial1 library.
  */

  FairContainer* p1= new FairContainer("ERNeuRadGeoPar",
                                       "ERNeuRadGeoPar",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);

  FairContainer* p2= new FairContainer("ERNeuRadDigiPar",
                                       "ERNeuRadDigiPar",
                                       "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");

  containers->Add(p2);
}

FairParSet* ERNeuRadContFact::createContainer(FairContainer* c)
{
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"ERNeuRadGeoPar")==0) {
    p=new ERNeuRadGeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"ERNeuRadDigiPar")==0) {
    p=new ERNeuRadDigiPar(c->getConcatName().Data(),
                                  c->GetTitle(),c->getContext());
  }
  return p;
}

ClassImp(ERNeuRadContFact)
