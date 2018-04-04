/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERSetup_H
#define ERSetup_H

#include "TGeoManager.h"
#include "TROOT.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"
#include "FairLogger.h"

#include "ERGeoSubAssembly.h"

class ERSetup {
public:
  ERSetup();
  ~ERSetup();

  /* Modifiers */
  void SetXMLParametersFile(TString xmlFileName) {fParamsXMLFileName = xmlFileName;} 
  void SetGeoName(TString name) {fGeoName = name;}
  void AddSubAssembly(ERGeoSubAssembly* subAssembly);
  /* Accessors */
  std::vector<TString>* GetComponentNames() const {return fComponentNames;}
  TString GetXMLParametersFile(void) {return fParamsXMLFileName;} 
public:
  virtual void  ReadGeoParamsFromParContainer() = 0;
  virtual Int_t SetParContainers();
  virtual void  ConstructGeometry();

protected:
  // static void ParseXMLParameters();

  TString              fParamsXMLFileName;
  TObjArray*           fSubAssembies;
  TString              fGeoName;
  std::vector<TString> *fComponentNames;

  ClassDef(ERSetup,1)
};
#endif
