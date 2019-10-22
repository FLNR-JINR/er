#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_SiSource() {

  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  
  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/siSource.geo.root";
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mSilicon = geoMedia->getMedium("silicon");
  if ( ! mSilicon ) Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(mSilicon);
  TGeoMedium* pSilicon = gGeoMan->GetMedium("silicon");
  if ( ! pSilicon ) Fatal("Main", "Medium silicon not found");

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* target = new TGeoVolumeAssembly("target");
  // --------------------------------------------------------------------------
  Double_t radius = 6e-1;
  Double_t thickness = 600e-7;
  // --------------------------------------------------------------------------
  TGeoVolume *sourceVol = gGeoManager->MakeTube("target_siSource", pSilicon, 0, radius, thickness / 2.);

  target->AddNode(sourceVol,0,new TGeoCombiTrans(.0,.0, 0., fZeroRotation));
  top->AddNode(target,0,new TGeoCombiTrans(.0,.0, 0., fZeroRotation));
  // --------------------------------------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
} 
