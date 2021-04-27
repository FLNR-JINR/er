#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"
#include "TSystem.h"

void create_target_exp1904() {

  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // create rotation for X
  TGeoRotation *fRotX = new TGeoRotation();
  fRotX->RotateX(180.);
  fRotX->RotateY(0.);
  fRotX->RotateZ(0.);
  // create rotation for Y
  TGeoRotation *fRotY = new TGeoRotation();
  fRotY->RotateX(0.);
  fRotY->RotateY(180.);
  fRotY->RotateZ(0.);
  
  Double_t zero_trans_X = 0.;
  Double_t zero_trans_Y = 0.; 
  Double_t zero_trans_Z = 0.;
  
  TGeoManager*   gGeoMan = NULL;

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media1.geo";

  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/target_exp1904.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mD2 = geoMedia->getMedium("D2");
  if ( ! mD2 ) Fatal("SOS", "FairMedium D2 not found");
  geoBuild->createMedium(mD2);
  TGeoMedium* pD2 = gGeoMan->GetMedium("D2");
  if ( ! pD2 ) Fatal("SOS", "Medium D2 not found"); 

  FairGeoMedium* mMylar = geoMedia->getMedium("mylar");
  if ( ! mMylar ) Fatal("SOS", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium* pMylar = gGeoMan->GetMedium("mylar");
  if ( ! pMylar ) Fatal("SOS", "Medium mylar not found");

  FairGeoMedium* mCopper = geoMedia->getMedium("copper");
  if ( ! mCopper ) Fatal("SOS", "FairMedium copper not found");
  geoBuild->createMedium(mCopper);
  TGeoMedium* pCopper = gGeoMan->GetMedium("copper");
  if ( ! pCopper ) Fatal("SOS", "Medium copper not found");

  FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
  if ( ! mSteel ) Fatal("SOS", "FairMedium Steel not found");
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
  if ( ! pSteel ) Fatal("SOS", "Medium vacuum not found");
  
  FairGeoMedium* vacuum = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("SOS", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("SOS", "Medium vacuum not found");
  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* target = new TGeoVolumeAssembly("target");
  // --------------------------------------------------------------------------
  // Create the box 6x6 cm of copper with a cylindrical bore 2.5 cm in diameter
  Double_t box_X,box_Y,box_Z;

  box_X = 6.; //cm
  box_Y = 6.; //cm
  box_Z = 0.4; //cm
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;

  TGeoBBox box1 = TGeoBBox("box1", box_X, box_Y, box_Z);
  TGeoTube tube1 = TGeoTube("tube1", 0., 1.25, box_Z);

  TGeoCompositeShape *box2 = new TGeoCompositeShape("box2", "(box1) - (tube1)");
  TGeoVolume* boxCopper = new TGeoVolume("boxCopper", box2, pCopper);
  
  // --------------------------------------------------------------------------
  // Create the tube of D2 with spherical lenticular 2mm edges

  Double_t R_min,R_max,theta_min,theta_max;
  theta_min = 0;
  theta_max = TMath::ATan(1.25/3.80625)*TMath::RadToDeg();
  R_max = 4.00625;   //cm
  R_min = R_max-0.2; //cm

  TGeoSphere *sectorD2_1 = new TGeoSphere("sectorD2_1",R_min,R_max,theta_min,theta_max,0,360.);
  TGeoSphere *sectorD2_2 = new TGeoSphere("sectorD2_2",R_min,R_max,theta_min,theta_max,0,360.);  

  TGeoTube tubeD2 = TGeoTube("tubeD2", 0., 1.25, box_Z);

  TGeoCombiTrans* trans1 = new TGeoCombiTrans("trans1", 0., 0., box_Z - R_min, fZeroRotation);
  trans1->RegisterYourself();

  TGeoCombiTrans* trans2 = new TGeoCombiTrans("trans2", 0., 0., -box_Z + R_min, fRotX);
  trans2->RegisterYourself();

  TGeoCompositeShape *GasShape = new TGeoCompositeShape("GasShape", "sectorD2_1:trans1 + tubeD2 + sectorD2_2:trans2");
  TGeoVolume* shapeD2 = new TGeoVolume("shapeD2", GasShape, pD2); 
  // --------------------------------------------------------------------------
  // stainless steel sector
  R_min = R_max;
  R_max = R_min + 0.0006;   //cm 

  TGeoSphere *sectorSt_1 = new TGeoSphere("sectorSt_1",R_min,R_max,theta_min,theta_max,0,360.);
  TGeoCombiTrans* trans_St1 = new TGeoCombiTrans("trans_St1", 0., 0., box_Z + 0.2 - R_min, fZeroRotation);
  trans_St1->RegisterYourself();

  TGeoSphere *sectorSt_2 = new TGeoSphere("sectorSt_2",R_min,R_max,theta_min,theta_max,0,360.);
  TGeoCombiTrans* trans_St2 = new TGeoCombiTrans("trans_St2", 0., 0., -box_Z - 0.2 + R_min, fRotX);
  trans_St2->RegisterYourself();

  TGeoCompositeShape *shapeWin1 = new TGeoCompositeShape("shapeWin1", "sectorSt_1:trans_St1 + sectorSt_2:trans_St2");
  TGeoVolume* steelWin = new TGeoVolume("steelWin", shapeWin1, pSteel);
  // --------------------------------------------------------------------------
  // mylar sector
  R_min = R_max;
  R_max = R_min + 0.00035;   //cm 

  TGeoSphere *sectorMyl_1 = new TGeoSphere("sectorMyl_1",R_min,R_max,theta_min,theta_max,0,360.);
  TGeoCombiTrans* trans_Myl1 = new TGeoCombiTrans("trans_Myl1", 0., 0., box_Z + 0.2006 - R_min, fZeroRotation);
  trans_Myl1->RegisterYourself();

  TGeoSphere *sectorMyl_2 = new TGeoSphere("sectorMyl_2",R_min,R_max,theta_min,theta_max,0,360.);
  TGeoCombiTrans* trans_Myl2 = new TGeoCombiTrans("trans_Myl2", 0., 0., -box_Z - 0.2006 + R_min, fRotX);
  trans_Myl2->RegisterYourself();

  TGeoCompositeShape *shapeWin2 = new TGeoCompositeShape("shapeWin2", "sectorMyl_1:trans_Myl1 + sectorMyl_2:trans_Myl2");
  TGeoVolume* mylarWin = new TGeoVolume("mylarWin", shapeWin2, pMylar);
  // --------------------------------------------------------------------------
  target->AddNode(boxCopper, 0, new TGeoCombiTrans(zero_trans_X, zero_trans_Y, zero_trans_Z,fZeroRotation));
  target->AddNode(shapeD2, 0, new TGeoCombiTrans(zero_trans_X, zero_trans_Y, zero_trans_Z,fZeroRotation));
  target->AddNode(steelWin, 0, new TGeoCombiTrans(zero_trans_X, zero_trans_Y, zero_trans_Z,fZeroRotation));
  target->AddNode(mylarWin, 0, new TGeoCombiTrans(zero_trans_X, zero_trans_Y, zero_trans_Z,fZeroRotation));
  // --------------------------------------------------------------------------
  top->AddNode(target, 0, new TGeoCombiTrans(zero_trans_X,zero_trans_Y, zero_trans_Z,fZeroRotation));
  // --------------------------------------------------------------------------

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps();
  // gGeoMan->CheckGeometryFull();
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  shapeD2->SetLineColor(kGray);
  shapeD2->SetTransparency(30);

  boxCopper->SetLineColor(kRed-3);
  boxCopper->SetTransparency(60);

  steelWin->SetLineColor(kBlue);
  steelWin->SetTransparency(30);

  mylarWin->SetLineColor(kGreen);
  mylarWin->SetTransparency(30);

  top->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

} 

