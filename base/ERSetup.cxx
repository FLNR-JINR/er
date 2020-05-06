/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSetup.h"

#include "TGeoManager.h"
#include "TROOT.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"
#include "FairLogger.h"
//--------------------------------------------------------------------------------------------------
Int_t ERSetup::SetParContainers(){
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
//--------------------------------------------------------------------------------------------------
void ERSetup::AddSubAssembly(ERGeoSubAssembly* subAssembly) {
  fSubAssemblies[subAssembly->GetName()] = subAssembly;
}
//--------------------------------------------------------------------------------------------------
void ERSetup::AddSubAssembly(ERGeoSubAssembly* subAssembly, const TVector3& position,
                             const TVector3& rotation) {
  subAssembly->SetPosition(position);
  subAssembly->SetRotation(rotation);  
  AddSubAssembly(subAssembly);
}
//--------------------------------------------------------------------------------------------------
void ERSetup::ConstructGeometry(void) {
  if (!gGeoManager)
    LOG(FATAL) << "[ERSetup] gGeoManager has not inited.\n";
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  TGeoVolume* geoVol = new TGeoVolumeAssembly(fGeoName);
  int i = 0;
  for (auto subAssemblyPair : fSubAssemblies) {
    auto* subAssembly = subAssemblyPair.second;
    subAssembly->ConstructGeometryVolume();
    const TVector3* translation = subAssembly->GetPosition();
    auto* rotation = const_cast<TGeoRotation*>(subAssembly->GetRotation());
    auto* volume = const_cast<TGeoVolume*>(subAssembly->GetVolume());
    geoVol->AddNode(volume, ++i, 
                    new TGeoCombiTrans(translation->X() ,translation->Y(), translation->Z(),
                    rotation));
  }
  top->AddNode(geoVol, 1, new TGeoCombiTrans(0., 0., 0., new TGeoRotation()));  
  const TString geoPath = gSystem->Getenv("VMCWORKDIR");
  const TString geoFileName = geoPath + "/geometry/" + fGeoName + ".temp.root";
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
}
//--------------------------------------------------------------------------------------------------
std::list<ERGeoComponent*> ERSetup::GetAllComponents() {
  std::list<ERGeoComponent*> components;
  for (auto subAssemblyPair : fSubAssemblies) {
    auto* subAssembly = subAssemblyPair.second;
    auto& subComponents = subAssembly->GetComponents();
    for (auto subComponentPair : subComponents) {
      components.push_back(subComponentPair.second);
    }
  }
  return components;
}
//--------------------------------------------------------------------------------------------------
ERGeoComponent* ERSetup::GetComponent(const TString& path){
  const auto components = GetAllComponents();
  const auto componentIt = std::find_if(
    components.begin(), components.end(), [&path](ERGeoComponent* component) {
      return path.Contains(component->GetVolumeName());
  });
  return componentIt != components.end() ? *componentIt : nullptr;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERSetup)
