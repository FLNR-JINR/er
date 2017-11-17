/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadContFact header file                 -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadContFact_H
#define ERNeuRadContFact_H

#include "FairContFact.h" // mother class

class FairContainer;

class ERNeuRadContFact : public FairContFact
{
private:

  void setAllContainers();

public:

  ERNeuRadContFact();
  ~ERNeuRadContFact() {}

  FairParSet* createContainer(FairContainer*);

  ClassDef(ERNeuRadContFact, 0);
};

#endif // ERNeuRadContFact_H
