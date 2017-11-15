/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERGEOCAVE_H
#define ERGEOCAVE_H

#include <fstream>

#include "TString.h"

#include "FairGeoSet.h"

class FairGeoMedia;

class ERGeoCave : public FairGeoSet
{
public:

  /** Default constructor **/
  ERGeoCave();

  /** Destructor **/
  ~ERGeoCave();

  const char* getModuleName(Int_t) { return name.Data(); }
  Bool_t read(std::fstream&, FairGeoMedia*);
  void addRefNodes();
  void write(std::fstream&);
  void print();

protected:

  TString name;

  ClassDef(ERGeoCave, 0);
};

#endif // ERGEOCAVE_H
