/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSetup.h"

TGeoManager*    ERSetup::fGeoMan   = NULL;
FairGeoMedia*   ERSetup::fGeoMedia = NULL;
FairGeoBuilder* ERSetup::fGeoBuild = NULL;
//--------------------------------------------------------------------------------------------------
ERSetup::ERSetup() 
: fSubAssembies(new TObjArray()),
  fComponentNames(new std::vector<TString>())
{
 std::cout << "ERSetup::ERSetup()" << std::endl;
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
void ERSetup::AddSubAssembly(TObject* subAssembly) {
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
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
 
  // --------------   Create geometry and top volume  -------------------------
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  TGeoVolume* geoVol = new TGeoVolumeAssembly(fGeoName);

  //------------------ STRUCTURE  ---------------------------------------------
  TIter itSubAssembly(fSubAssembies);
  ERGeoSubAssembly *subAssembly;
  int i = 0;
  while(subAssembly = (ERGeoSubAssembly*)(itSubAssembly.Next())){
    subAssembly->ConstructGeometryVolume();
    TGeoVolume*   volume = subAssembly->GetVolume(); 
    TGeoRotation* rotation = subAssembly->GetRotation();
    TVector3*     trans = subAssembly->GetPosition();
    std::vector<TString> *compNames = subAssembly->GetComponentNames();
    for (const auto itCompNames : *compNames) {
      fComponentNames->push_back(itCompNames);
    }
    // top->AddNode(volume, ++i, new TGeoCombiTrans(trans->X() ,trans->Y(), trans->Z(), rotation));
    geoVol->AddNode(volume, ++i, new TGeoCombiTrans(trans->X() ,trans->Y(), trans->Z(), rotation)); 
  }
  top->AddNode(geoVol, 1, new TGeoCombiTrans(0., 0., 0., new TGeoRotation()));  
  
  TString geoFileName = geoPath + "/geometry/" + fGeoName + ".temp.root";
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERSetup)
