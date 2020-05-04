/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGeoSubAssembly.h"
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(ERGeoComponent* component) {
  component->SetVolumeNamePrefix(TString(this->GetName()) + "_");
  fComponents[component->GetVolumeName()] = component;
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(ERGeoComponent* component, const TVector3& position,
                                    const TVector3& rotation) {
  component->SetPosition(position);
  component->SetRotation(rotation);
  AddComponent(component);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::ConstructGeometryVolume() {
  fVolume = new TGeoVolumeAssembly(fName);
  for (auto componentPair : fComponents) {
    auto* component = componentPair.second;
    component->ConstructGeometryVolume();
    const auto* translation = component->GetPosition();
    auto* rotation = const_cast<TGeoRotation*>(component->GetRotation());
    fVolume->AddNode(component->GetVolume(), 0, 
                     new TGeoCombiTrans(translation->X() ,translation->Y(), translation->Z(), 
                                        rotation));
  }
}
//--------------------------------------------------------------------------------------------------
std::list<TString> ERGeoSubAssembly::GetComponentNames() const {
  std::list<TString> names;
  for (auto componentPair : fComponents) {
    names.push_back(componentPair.first);
  }
  return names;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoSubAssembly)

