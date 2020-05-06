/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGeoComponent.h"

#include "TGeoMedium.h"
#include "TGeoManager.h"
#include "TROOT.h"
#include "TSystem.h"

#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairLogger.h"
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(const TString& typeFromXML, const TString& id) 
: TNamed(typeFromXML + id, typeFromXML + id),
  fType(typeFromXML),
  fComponentId(id),
  fVolumeName(id)
{
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(const TString& name, const TVector3& position, 
                               const TVector3& rotation)
: TNamed(name, name), fVolumeName(name), fPosition(position)
{
  fRotation.RotateX(rotation.X());
  fRotation.RotateY(rotation.Y());
  fRotation.RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent::ERGeoComponent(const TString& typeFromXML, const TString& id, const TVector3& position,
                               const TVector3& rotation)
: TNamed(typeFromXML + id, typeFromXML + id), fType(typeFromXML),
  fComponentId(id), fVolumeName(id), fPosition(position)
{
  fRotation.RotateX(rotation.X());
  fRotation.RotateY(rotation.Y());
  fRotation.RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
void ERGeoComponent::SetRotation(const TVector3& rotation) {
  fRotation.Clear();
  fRotation.RotateX(rotation.X());
  fRotation.RotateY(rotation.Y());
  fRotation.RotateZ(rotation.Z());
}
//--------------------------------------------------------------------------------------------------
TGeoMedium* ERGeoComponent::CreateMaterial(const TString& name) {
  FairGeoLoader* geoLoader = FairGeoLoader::Instance();
  FairGeoInterface* geoFace = geoLoader->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuilder = geoLoader->getGeoBuilder();
  FairGeoMedium* fairMedia = geoMedia->getMedium(name);
  if (!fairMedia)
    LOG(FATAL) << "Media " << name << " not found in media.geo file\n";
  geoBuilder->createMedium(fairMedia);
  TGeoMedium* rootMedia = gGeoManager->GetMedium(name);
  if (!rootMedia)
    LOG(FATAL) << "Media " << name << " not found in gGeoManager\n";
  LOG(DEBUG) << "Created " << name << " media\n";
  return rootMedia;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

