/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERGeoSubAssembly_H
#define ERGeoSubAssembly_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"

class ERGeoSubAssembly : public ERGeoComponent {
public:
  ERGeoSubAssembly() = default;
  ERGeoSubAssembly(const TString& name) : ERGeoComponent(name) {}
  ERGeoSubAssembly(const TString& name, const TVector3& position, const TVector3& rotation)
    : ERGeoComponent(name, position, rotation) {}
  /* Modifiers */
  void AddComponent(ERGeoComponent* component);
  void AddComponent(ERGeoComponent* component, const TVector3& position, const TVector3& rotation);
  /* Accessors */
  const TGeoVolume* GetVolume() const {return fVolume;}
  const std::map<TString, ERGeoComponent*>& GetComponents() const { return fComponents; }
  std::list<TString> GetComponentNames() const;
  virtual std::list<TString> GetBranchNames(ERDataObjectType object) const {
    LOG(FATAL) << "SubAssembly does not produce branches\n";
    return {};
  }
  void  ConstructGeometryVolume();
protected:
  virtual void ParseXmlParameters() {}
  std::map<TString, ERGeoComponent*> fComponents;
  ClassDef(ERGeoSubAssembly,1)
};
#endif
