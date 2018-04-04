/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGeoSubAssembly.h"

#include "ERSetup.h"
#include "ERGeoComponent.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name) 
: TNamed(name, name)
{
  fRotation = new TGeoRotation();
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name, TVector3 position, TVector3 rotation)
: TNamed(name, name)
{
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::~ERGeoSubAssembly() {
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::SetPosition(Double_t x, Double_t y, Double_t z) {
	fPosition = TVector3(x, y, z);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::SetRotation(Double_t x, Double_t y, Double_t z) {
  fRotation->RotateX(x);
  fRotation->RotateY(y);
  fRotation->RotateZ(z);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(ERGeoComponent* component) {
  fComponents->AddLast(component);  
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::ConstructGeometryVolume() {
  fVolume = new TGeoVolumeAssembly(fName);
  TIter itComponent(fComponents);
  ERGeoComponent *component;
  while(component = (ERGeoComponent*)itComponent.Next()){
    component->ConstructGeometryVolume();
    TGeoVolume*   volume = component->GetVolume(); 
    TGeoRotation* rotation = component->GetRotation();
    TVector3      trans = component->GetPosition();
    fComponentNames->push_back(component->GetName());
    fVolume->AddNode(volume, 1, new TGeoCombiTrans(trans.X() ,trans.Y(), trans.Y(), rotation)); 
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoSubAssembly)

