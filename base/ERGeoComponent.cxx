/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGeoComponent.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent() {
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(TString name) 
: TNamed(name, name),
  fPosition(new TVector3()),
  fRotation(new TGeoRotation())
{
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(TString typeFromXML, TString id) 
: TNamed(typeFromXML + id, typeFromXML + id),
  fType(typeFromXML),
  fComponentId(id),
  fVolumeName(id)
{
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(TString typeFromXML, TString id, TVector3 position, TVector3 rotation)
: TNamed(typeFromXML + id, typeFromXML + id),
  fType(typeFromXML),
  fComponentId(id),
  fVolumeName(id),
  fPosition(new TVector3(position))
{
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::~ERGeoComponent() {
}
//--------------------------------------------------------------------------------------------------
void ERGeoComponent::SetPosition(TVector3 position) {
  fPosition = new TVector3(position);
}
//--------------------------------------------------------------------------------------------------
void ERGeoComponent::SetRotation(TVector3 rotation) {
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

