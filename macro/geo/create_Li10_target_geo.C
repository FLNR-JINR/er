#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create a zero rotation
TGeoRotation *fZeroRotation = new TGeoRotation();

Double_t transX = 0.;
Double_t transY = 0.;
Double_t transZ = .056/2.;

TGeoManager*   gGeoMan = NULL;

void create_Li10_target_geo()
{
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/Li10_target.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mD      = geoMedia->getMedium("deuterium");
  if ( ! mD ) Fatal("Main", "FairMedium deuterium not found");
  geoBuild->createMedium(mD);
  TGeoMedium* pD = gGeoMan->GetMedium("deuterium");
  if ( ! pD ) Fatal("Main", "Medium deuterium not found");

  FairGeoMedium* mFe      = geoMedia->getMedium("ferum");
  if ( ! mFe ) Fatal("Main", "FairMedium ferum not found");
  geoBuild->createMedium(mFe);
  TGeoMedium* pFe = gGeoMan->GetMedium("ferum");
  if ( ! pFe ) Fatal("Main", "Medium ferum not found");
  
  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* targetAss = new TGeoVolumeAssembly("target");
  // --------------------------------------------------------------------------
  //------------------ target cover -----------------------------------------
  Double_t target_cover_R = 2.;   //cm
  Double_t target_cover_Z = 0.05+0.006;   //cm
  target_cover_Z /= 2.;
  TGeoVolume *target_cover = gGeoManager->MakeTube("target_cover", pFe, 0, target_cover_R, target_cover_Z);
  
  //------------------ target -----------------------------------------
  Double_t target_R = 2.;   //cm
  Double_t target_Z = 0.05;   //cm
  target_Z /= 2.;
  TGeoVolume *target = gGeoManager->MakeTube("target", pD, 0, target_R, target_Z);
  
  //------------------ STRUCTURE  -----------------------------------------
  target_cover->AddNode(target, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  targetAss->AddNode(target_cover, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  top->AddNode(targetAss, 1, new TGeoCombiTrans(transX, transY, transZ, fZeroRotation));
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}	