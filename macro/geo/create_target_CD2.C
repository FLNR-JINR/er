#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_target_CD2() {

  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  
  TGeoRotation *targetRotation = new TGeoRotation();
  targetRotation->RotateX(0.);
  targetRotation->RotateY(12.);
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
  TString geoFileName = geoPath + "/geometry/target_CD2_geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mAlumin = geoMedia->getMedium("aluminium");
  if ( ! mAlumin ) Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(mAlumin);
  TGeoMedium* pAlumin = gGeoMan->GetMedium("aluminium");
  if ( ! pAlumin ) Fatal("Main", "Medium aluminium not found");
  
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
  box_Z = 0.1; //cm
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;
  // --------------------------------------------------------------------------
  TGeoBBox box1 = TGeoBBox("box1", box_X, box_Y, box_Z);
  TGeoBBox box2 = TGeoBBox("box2", box_X - 1./2., box_Y - 1./2., box_Z);

  TGeoCompositeShape *frame = new TGeoCompositeShape("frame", "box1 - box2");
  TGeoVolume* frameAL = new TGeoVolume("frameAL", frame, pAlumin);
  // --------------------------------------------------------------------------

  TGeoVolume *boxCD = gGeoManager->MakeBox("boxCD", pPoli, box_X, box_Y, 1e-3);

  // --------------------------------------------------------------------------
  Double_t trans_X, trans_Y, trans_Z;
  trans_Z = 0.05 + 1e-3;
  // --------------------------------------------------------------------------
  target->AddNode(frameAL, 0, new TGeoCombiTrans(0.,0.,trans_Z, fZeroRotation));
  target->AddNode(frameAL, 0, new TGeoCombiTrans(0.,0.,-trans_Z, fZeroRotation));
  target->AddNode(boxCD, 0, new TGeoCombiTrans(0.,0.,0., fZeroRotation));
  top->AddNode(target,0,new TGeoCombiTrans(.0,.0,.0, targetRotation));
  // --------------------------------------------------------------------------
  gGeoMan->CloseGeometry();
  //gGeoMan->CheckOverlaps(0.001);
  gGeoMan->CheckGeometryFull();
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
} 

