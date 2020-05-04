/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERGeoComponent_H
#define ERGeoComponent_H

#include <list>

#include "TString.h"
#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"

#include "ERSupport.h"

class ERGeoComponent : public TNamed {
public:
  ERGeoComponent() = default;
  ERGeoComponent(const TString& name) : TNamed(name, name) {}
  ERGeoComponent(const TString& name, const TVector3& position, const TVector3& rotation);
  ERGeoComponent(const TString& typeFromXML, const TString& id);
  ERGeoComponent(const TString& typeFromXML, const TString& id, 
                 const TVector3& position, const TVector3& rotation);
  /* Modifiers */
  void SetVolumeNamePrefix(const TString& prefix) {fVolumeName = prefix + fVolumeName;}
  void SetPosition(const TVector3& position) {fPosition = position; }
  void SetRotation(const TVector3& rotation);
  /* Accessors */
  TGeoVolume*  GetVolume() {return fVolume;}
  const TVector3* GetPosition() const {return &fPosition;}
  const TGeoRotation* GetRotation() const {return &fRotation;}
  const TString& GetID() const {return fComponentId;}
  const TString& GetType() const {return fType;}
  const TString& GetVolumeName() const {return fVolumeName;}
  virtual std::list<TString> GetBranchNames(ERDataObjectType object) const = 0;
  virtual void ConstructGeometryVolume(void) = 0;
protected:
  virtual void ParseXmlParameters() = 0;
  TGeoMedium* CreateMaterial(const TString& name); 
  TGeoVolume*   fVolume = nullptr;
  TVector3     fPosition;
  TGeoRotation fRotation;
  TString       fType;
  TString       fComponentId;
  TString       fVolumeName;
  ClassDef(ERGeoComponent,1)
};
#endif
