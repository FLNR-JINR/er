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
  virtual ~ERSetup();

  /* Modifiers */
  void SetXMLParametersFile(TString xmlFileName) {fParamsXMLFileName = xmlFileName;} 
  void SetGeoName(TString name) {fGeoName = name;}
  void AddSubAssembly(TObject* subAssembly);
  /* Accessors */
  TString               GetGeoFileName()           const {return (fGeoName + ".temp.root");}
  TString               GetGeoName()               const {return fGeoName;}
  TString               GetXMLParametersFile(void) const {return fParamsXMLFileName;} 
  std::vector<TString>* GetComponentNames()        const {return fComponentNames;}
public:
  virtual void  ReadGeoParamsFromParContainer() = 0;
  virtual Int_t SetParContainers();
  virtual void  ConstructGeometry();

protected:
  static TGeoManager    *fGeoMan;
  static FairGeoMedia   *fGeoMedia;
  static FairGeoBuilder *fGeoBuild;
  TString               fParamsXMLFileName;
  TObjArray*            fSubAssembies;
  TString               fGeoName;
  std::vector<TString>  *fComponentNames;

  ClassDef(ERSetup,1)
};
#endif
