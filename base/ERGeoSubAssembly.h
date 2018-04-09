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

class ERGeoSubAssembly : public TNamed {
public:
  ERGeoSubAssembly();
  ERGeoSubAssembly(TString name);
  ERGeoSubAssembly(TString name, TVector3 position, TVector3 rotation);
  ~ERGeoSubAssembly();

  /* Modifiers */
  void SetPosition(TVector3 position);
  void SetRotation(TVector3 rotation);
  void AddComponent(TObject* component);
  void AddComponent(TObject* component, TVector3 position, TVector3 rotation);
  /* Accessors */
  TGeoVolume*   GetVolume()   const {return fVolume;}
  TVector3*     GetPosition() const {return fPosition;}
  TGeoRotation* GetRotation() const {return fRotation;}
  std::vector<TString>* GetComponentNames() const {return fComponentNames;}
  
public:
  void  ConstructGeometryVolume();

protected:
  TObjArray*               fComponents;
  TVector3*                fPosition;
  TGeoRotation*            fRotation;
  TGeoVolume*              fVolume;
  std::vector<TString>     *fComponentNames;
  
  static std::map<TString, Int_t> *fComponentTypes; // <"ComponentName", count of components with "ComponentName">

  ClassDef(ERGeoSubAssembly,1)
};
#endif
