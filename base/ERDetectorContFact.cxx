#include "ERDetectorContFact.h"

#include "ERDetectorGeoPar.h"

#include "FairRuntimeDb.h"

#include <iostream>
using namespace std;

static ERDetectorContFact gERDetectorContFact;

ERDetectorContFact::ERDetectorContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="ERDetectorContFact";
  fTitle="Factory for parameter containers in libTutorial1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void ERDetectorContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the Tutorial1 library.
  */

  FairContainer* p1= new FairContainer("ERDetectorGeoPar",
                                       "ERDetectorGeoPar",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");

  containers->Add(p1);
}

FairParSet* ERDetectorContFact::createContainer(FairContainer* c)
{
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"ERDetectorGeoPar")==0) {
    p=new ERDetectorGeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  return p;
}

ClassImp(ERDetectorContFact)
