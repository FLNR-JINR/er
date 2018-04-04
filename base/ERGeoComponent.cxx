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
: TNamed(name, name)
{
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(TString name, TString typeFromXML, TVector3 position, TVector3 rotation)
: TNamed(name, name),
  fType(typeFromXML),
  fPosition(position)
{
  fRotation = new TGeoRotation();
  fRotation->RotateX(rotation.X());
  fRotation->RotateY(rotation.Y());
  fRotation->RotateZ(rotation.Z());}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::~ERGeoComponent() {
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

