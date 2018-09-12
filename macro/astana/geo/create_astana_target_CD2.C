#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_astana_target_CD2() {

  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  
  TGeoRotation *targetRotation = new TGeoRotation();
  targetRotation->RotateX(0.);
  targetRotation->RotateY(0.);
  targetRotation->RotateZ(0.);

  TGeoManager*   gGeoMan = NULL;

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
  TString geoFileName = geoPath + "/geometry/astana_target_CD2_geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* poli = geoMedia->getMedium("CD2_CH2");
  if ( ! poli ) Fatal("Main", "FairMedium CD2_CH2 not found");
  geoBuild->createMedium(poli);
  TGeoMedium* pPoli = gGeoMan->GetMedium("CD2_CH2");
  if ( ! pPoli ) Fatal("Main", "Medium CD2_CH2 not found");
  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* target = new TGeoVolumeAssembly("target");
  // --------------------------------------------------------------------------
  // frame of aluminium
  Double_t box_X,box_Y,box_Z; // box of steel

  box_X = 6.; //cm
  box_Y = 6.; //cm
  box_Z = 0.004; //cm
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;
  // --------------------------------------------------------------------------
  TGeoVolume *boxCD = gGeoManager->MakeBox("boxCD", pPoli, box_X, box_Y, box_Z);
  // --------------------------------------------------------------------------
  target->AddNode(boxCD, 0, new TGeoCombiTrans(0.,0.,0., fZeroRotation));
  top->AddNode(target,0,new TGeoCombiTrans(.0,.0,.0, targetRotation));
  // --------------------------------------------------------------------------
  gGeoMan->CloseGeometry();
  //gGeoMan->CheckOverlaps(0.001);
  //gGeoMan->CheckGeometryFull();
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
} 

