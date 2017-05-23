// -------------------------------------------------------------------------
// -----                    ERFieldContFact header file                -----
// -----                   Created 05/17  by V. Schetinin              -----
// -------------------------------------------------------------------------

#ifndef ERFieldContFact_H
#define ERFieldContFact_H 1

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class ERFieldContFact : public FairContFact
{
 public:
  /** Constructor **/
  ERFieldContFact();
  /** Destructor **/
  ~ERFieldContFact();
  /** Create containers
   ** Creates the requested parameter sets (CbmFieldPar) 
   **/
  FairParSet* createContainer(FairContainer* container);
 private:
  /** Set all containers  
   ** Creates container objects with all accepted contexts and adds them
   ** to the list of containers for the field library. 
   **/
  void SetAllContainers();
  ClassDef(ERFieldContFact,1);
};
#endif
