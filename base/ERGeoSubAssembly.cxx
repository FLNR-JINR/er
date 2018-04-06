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
ERGeoSubAssembly::ERGeoSubAssembly() {
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name) 
: TNamed(name, name),
  fComponents(new TObjArray()),
  fComponentNames(new std::vector<TString>()),
  fPosition(new TVector3()),
  fRotation(new TGeoRotation())
{
}
//--------------------------------------------------------------------------------------------------
ERGeoSubAssembly::ERGeoSubAssembly(TString name, TVector3 position, TVector3 rotation)
: TNamed(name, name),
  fComponents(new TObjArray()),
  fComponentNames(new std::vector<TString>()),
  fPosition(new TVector3(position))
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
	fPosition = new TVector3(x, y, z);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::SetRotation(Double_t x, Double_t y, Double_t z) {
  fRotation->RotateX(x);
  fRotation->RotateY(y);
  fRotation->RotateZ(z);
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::AddComponent(TObject* component) {
  cout << "ERGeoSubAssembly::AddComponent " << ((ERGeoComponent*)component)->GetBranchName() << endl;  
  fComponents->AddLast(component);  
}
//--------------------------------------------------------------------------------------------------
void ERGeoSubAssembly::ConstructGeometryVolume() {
  fVolume = new TGeoVolumeAssembly(fName);
  TIter itComponent(fComponents);
  ERGeoComponent *component;
  int i = 0;
  while(component = (ERGeoComponent*)(itComponent.Next())){
    cout << "ERGeoSubAssembly::ConstructGeometryVolume" << endl;
    component->ConstructGeometryVolume();
    TGeoVolume*   volume = component->GetVolume(); 
    TGeoRotation* rotation = component->GetRotation();
    TVector3*     trans = component->GetPosition();
    fComponentNames->push_back(component->GetBranchName());
    fVolume->AddNode(volume, ++i, new TGeoCombiTrans(trans->X() ,trans->Y(), trans->Z(), rotation)); 
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoSubAssembly)

