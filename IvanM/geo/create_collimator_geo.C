#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"



void create_collimator_geo()
{
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();

  TGeoManager*   gGeoMan = NULL;

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
  TString geoFileName = geoPath + "/geometry/collimator.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

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
  // --------------------------------------------------------------------------

  //------------------ target -----------------------------------------
  //Double_t R_min = .3;   //cm
  // Double_t R_min = .0;   //cm
  // Double_t R_max = 10.;   //cm
  Double_t X_col = 4.8;
  Double_t Y_col = 4.8;
  Double_t Z = 7.;   //cm
  
  X_col /= 2.;
  Y_col /= 2.;
  Z /= 2.;
  // TGeoVolume *target = gGeoManager->MakeTube("target_vol", pFe, R_min, R_max,Z);
  TGeoVolume *target = gGeoManager->MakeBox("target_vol", pFe,X_col, Y_col, Z);
  //------------------ slice -----------------------------------------
  //Double_t R_min = .3;   //cm
  Double_t X_slice = 0.3;   //cm
  Double_t Y_slice = 0.3;   //cm
  X_slice /= 2.;
  Y_slice /= 2.;

  TGeoVolume *slice = gGeoManager->MakeBox("slice", pMed0,X_slice, Y_slice, Z);
  //------------------ STRUCTURE  -----------------------------------------
  Double_t transX = 0.;
  Double_t transY = 0.;
  Double_t transZ = 2.4 + Z + 10.;

  TGeoVolume* targetAss = new TGeoVolumeAssembly("target");
  targetAss->AddNode(target, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  targetAss->AddNode(slice, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
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