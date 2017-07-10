// -------------------------------------------------------------------------
// -----                    ERFieldContFact source file               -----
// -----                   Created 20/02/06  by V. Friese              -----
// -------------------------------------------------------------------------
#include "ERFieldContFact.h"

#include "FairParSet.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include <iostream>
using std::cout;
using std::endl;

#include "ERFieldPar.h"

static ERFieldContFact gERFieldContFact;
// -----   Constructor   ---------------------------------------------------
ERFieldContFact::ERFieldContFact() {
  fName = "ERFieldContFact";
  fTitle = "Factory for field parameter containers";
  SetAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}
// -------------------------------------------------------------------------
// -----   Destructor   ----------------------------------------------------
ERFieldContFact::~ERFieldContFact() { }
// -------------------------------------------------------------------------
// -----   Create containers   ---------------------------------------------
FairParSet* ERFieldContFact::createContainer(FairContainer* container) {
  const char* name = container->GetName();
  LOG(INFO) << "create ERFieldPar container " << name << FairLogger::endl;
  FairParSet* set = NULL;
  if ( strcmp(name, "ERFieldPar") == 0 ) 
    set = new ERFieldPar( container->getConcatName().Data(),
                           container->GetTitle(),
                           container->getContext() );
  return set;
}
// -------------------------------------------------------------------------
// -----   Set all containers (private)   ----------------------------------
void ERFieldContFact::SetAllContainers() {
  FairContainer* container = new FairContainer("ERFieldPar",
                                             "Field parameter container",
                                             "Default field");
  containers->Add(container);
}
// -------------------------------------------------------------------------
ClassImp(ERFieldContFact)
