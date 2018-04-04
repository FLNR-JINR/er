/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSetup.h"

//--------------------------------------------------------------------------------------------------
ERSetup::ERSetup() {
}
//--------------------------------------------------------------------------------------------------
ERSetup::~ERSetup() {
}
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
  fSubAssembies->AddLast(subAssembly);
}
//--------------------------------------------------------------------------------------------------
void ERSetup::ConstructGeometry(void) {
  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad = FairGeoLoader::Instance();//
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/" + fGeoName + ".temp.root";
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
 
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  TGeoVolume* geoVol = new TGeoVolumeAssembly(fGeoName);

  //------------------ STRUCTURE  ---------------------------------------------
  TIter itSubAssembly(fSubAssembies);
  ERGeoSubAssembly *subAssembly;
  while(subAssembly = (ERGeoSubAssembly*)itSubAssembly.Next()){
    TGeoVolume*   volume = subAssembly->GetVolume(); 
    TGeoRotation* rotation = subAssembly->GetRotation();
    TVector3      trans = subAssembly->GetPosition();
    std::vector<TString> *compNames = subAssembly->GetComponentNames();
    for (const auto itCompNames : *compNames) {
      fComponentNames->push_back(itCompNames);
    }
    top->AddNode(volume, 1, new TGeoCombiTrans(trans.X() ,trans.Y(), trans.Y(), rotation)); 
    // geoVol->AddNode(volume, 1, new TGeoCombiTrans(trans.X() ,trans.Y(), trans.Y(), rotation)); 
  }

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERSetup)
