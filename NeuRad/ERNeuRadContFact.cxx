/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadContFact.h"

#include "FairRuntimeDb.h"

#include "ERNeuRadGeoPar.h"
#include "ERNeuRadDigiPar.h"

static ERNeuRadContFact gERNeuRadContFact;

ERNeuRadContFact::ERNeuRadContFact()
{
  /** Constructor (called when the library is loaded) */
  fName = "ERNeuRadContFact";
  fTitle = "Factory for parameter containers in libTutorial1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void ERNeuRadContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the Tutorial1 library.
  */
  FairContainer* p1 = new FairContainer("ERNeuRadGeoPar",
                                        "ERNeuRadGeoPar",
                                        "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  containers->Add(p1);

  FairContainer* p2 = new FairContainer("ERNeuRadDigiPar",
                                        "ERNeuRadDigiPar",
                                        "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");
  containers->Add(p2);
}

FairParSet* ERNeuRadContFact::createContainer(FairContainer* c)
{
  const char* name = c->GetName();
  FairParSet* p = NULL;
  if (strcmp(name, "ERNeuRadGeoPar") == 0) {
    p = new ERNeuRadGeoPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  if (strcmp(name, "ERNeuRadDigiPar") == 0) {
    p = new ERNeuRadDigiPar(c->getConcatName().Data(), c->GetTitle(),c->getContext());
  }
  return p;
}

ClassImp(ERNeuRadContFact)
