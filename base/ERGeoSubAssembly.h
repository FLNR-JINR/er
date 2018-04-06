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
  void SetPosition(Double_t x, Double_t y, Double_t z);
  void SetRotation(Double_t x, Double_t y, Double_t z);
  void AddComponent(TObject* component);
  /* Accessors */
  TGeoVolume*   GetVolume()   const {return fVolume;}
  TVector3*     GetPosition() const {return fPosition;}
  TGeoRotation* GetRotation() const {return fRotation;}
  std::vector<TString>* GetComponentNames() const {return fComponentNames;}
  
public:
  void  ConstructGeometryVolume();

protected:
  TObjArray*           fComponents;
  TVector3*            fPosition;
  TGeoRotation*        fRotation;
  TGeoVolume*          fVolume;
  std::vector<TString> *fComponentNames;

  ClassDef(ERGeoSubAssembly,1)
};
#endif
