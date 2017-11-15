/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERCAVE_H
#define ERCAVE_H

#include "FairModule.h"

class ERCave : public FairModule
{
public:

  /** Default constructor **/
  ERCave();

  /** Standard constructor **/
  ERCave(const char* name, const char* title="ER Cave");

  /** Destructor **/
  virtual ~ERCave();

  virtual void ConstructGeometry();

private:

  Double_t world[3]; //! TODO what is it?

  ClassDef(ERCave, 1);
};

#endif // ERCAVE_H
