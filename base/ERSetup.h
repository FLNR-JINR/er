/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERSetup_H
#define ERSetup_H

#include "ERGeoComponent.h"
#include "ERGeoSubAssembly.h"

class ERSetup {
public:
  ERSetup() = default;
  virtual ~ERSetup() = default;
  /* Modifiers */
  void SetXMLParametersFile(const TString& xmlFileName) {fParamsXMLFileName = xmlFileName;} 
  void SetGeoName(const TString& name) {fGeoName = name;}
  void AddSubAssembly(ERGeoSubAssembly* subAssembly);
  void AddSubAssembly(ERGeoSubAssembly* subAssembly, const TVector3& position, const TVector3& rotation);
  /* Accessors */
  TString GetGeoFileName() const {return (fGeoName + ".temp.root");}
  TString GetGeoName() const {return fGeoName;}
  TString GetXMLParametersFile(void) const {return fParamsXMLFileName;}
  std::list<ERGeoComponent*> GetAllComponents();
  ERGeoComponent* GetComponent(const TString& path);
public:
  virtual void  ReadGeoParamsFromParContainer() = 0;
  virtual Int_t SetParContainers();
  virtual void  ConstructGeometry();
protected:
  TString fParamsXMLFileName;
  std::map<TString, ERGeoSubAssembly*> fSubAssemblies;
  TString fGeoName;

  ClassDef(ERSetup,1)
};
#endif
