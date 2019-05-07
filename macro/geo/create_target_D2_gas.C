#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"
#include "TSystem.h"

void create_target_D2_gas() {

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
  
  Double_t trans_X = 0.;
  Double_t trans_Y = 0.; 
  Double_t trans_Z = 0.;
  
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
  TString geoFileName = geoPath + "/geometry/target.D2.gas.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mD2 = geoMedia->getMedium("deuterium");
  if ( ! mD2 ) Fatal("SOS", "FairMedium deuterium not found");
  geoBuild->createMedium(mD2);
  TGeoMedium* pD2 = gGeoMan->GetMedium("deuterium");
  if ( ! pD2 ) Fatal("SOS", "Medium deuterium not found"); 

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
  // box of stainless steel
  Double_t box_X, box_Y, box_Z; // box of stainless steel outside
  Double_t R_gas = 2.7; // [cm]

  box_X = 5.4; // [cm]
  box_Y = 7.7; // [cm]
  box_Z = 0.62; // [cm]
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;
  box_Z += 0.1; //increase the thickness of the gas is 1 mm
  R_gas /= 2.;
  // --------------------------------------------------------------------------
  TGeoBBox box1  = TGeoBBox("box1",  box_X, box_Y, box_Z - 0.1);
  TGeoTube tube1 = TGeoTube("tube1", 0., R_gas + 0.12 , box_Z - 0.1); 

  TGeoCombiTrans *trans_box1 = new TGeoCombiTrans ("trans_box1", 
                                                    trans_X,   1.15,  trans_Z, fZeroRotation); //the distance is 11.5 mm 
  trans_box1->RegisterYourself();//the hole is not in the center of the box which is made of stainless steel. 
  TGeoCombiTrans *trans_tube1 = new TGeoCombiTrans ("trans_tube1", 
                                                    trans_X, trans_Y, trans_Z, fZeroRotation);
  trans_tube1->RegisterYourself();

  TGeoCompositeShape *steel_box = new TGeoCompositeShape("steel_box", "box1:trans_box1 - tube1:trans_tube1"); 

  TGeoVolume* boxST = new TGeoVolume("boxST", steel_box, pSteel);
  boxST->SetLineColor(kBlue);
  boxST->SetTransparency(60);
  // --------------------------------------------------------------------------
  target->AddNode(boxST, 0, new TGeoCombiTrans(trans_X, trans_Y, trans_Z, fZeroRotation));
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
    // create 2 mylar windows in front and back of the gas
      // --------------------------------------------------------------------------
  Double_t tubeZ = 0.00035; // [cm] Thickness of the mylar window is 3.5 Um
  tubeZ /= 2.;

  TGeoVolume *entry = gGeoMan->MakeTube("entry", pMylar, 0., box_X, tubeZ);
  entry->SetLineColor(kGray);
  entry->SetTransparency(60);

  TGeoVolume *exit = gGeoMan->MakeTube( "exit",  pMylar, 0., box_X, tubeZ);
  exit->SetLineColor(kGray);
  exit->SetTransparency(60);
  // --------------------------------------------------------------------------
  target->AddNode(exit,  0, new TGeoCombiTrans(trans_X, trans_Y,  1.6, fZeroRotation));
  target->AddNode(entry, 0, new TGeoCombiTrans(trans_X, trans_Y, -1.6, fZeroRotation));
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
    // create a target D2
      // --------------------------------------------------------------------------
  Double_t R_min, R_max, thsp_min, thsp_max; // create 2 sphere in front and back of 

  thsp_min = 0.; 
  thsp_max = 9.105; // 16.95/2
  R_min = 9.163;   //[cm] 8.903
  R_max = R_min + 0.31; //[cm]

  TGeoSphere *sector_thick = new TGeoSphere("sector_thick", R_min, R_max, thsp_min, thsp_max, 0., 360.);

  TGeoCombiTrans* trans1 = new TGeoCombiTrans("trans1", trans_X, trans_Y, - box_Z + R_min, fRotX);
  trans1->RegisterYourself();
  TGeoCombiTrans* trans2 = new TGeoCombiTrans("trans2", trans_X, trans_Y,   box_Z - R_min, fZeroRotation); 
  trans2->RegisterYourself();

  // tube of D2 inside
  TGeoTube tubeD = TGeoTube("tubeD", 0., R_gas + 0.1, box_Z);

  TGeoCompositeShape *D2_shape = new TGeoCompositeShape("D2_shape", "tubeD - ( sector_thick : trans1 + sector_thick : trans2 )");
  TGeoVolume* tubeD2 = new TGeoVolume("tubeD2", D2_shape, pD2);
  tubeD2->SetLineColor(kRed);
  
  // 2 Stainless Steel 6 Um outside
  R_max = 9.163;
  R_min = R_max - 0.0006;

  TGeoSphere *sector_thin = new TGeoSphere("sector_thin", R_min, R_max, thsp_min, thsp_max, 0., 360.);
  TGeoVolume *SSteel = new TGeoVolume("SSteel", sector_thin, pSteel);
  SSteel->SetLineColor(kBlue);
  SSteel->SetTransparency(90);
  // --------------------------------------------------------------------------
  tubeD2->AddNode(SSteel, 0, new TGeoCombiTrans(trans_X, trans_Y, - box_Z + R_max, fRotX));
  tubeD2->AddNode(SSteel, 0, new TGeoCombiTrans(trans_X, trans_Y,   box_Z - R_max ,fZeroRotation));
  target->AddNode(tubeD2, 0, new TGeoCombiTrans(trans_X, trans_Y, trans_Z, fZeroRotation));
  // --------------------------------------------------------------------------
   
  // --------------------------------------------------------------------------
    // create 2 copper
      // --------------------------------------------------------------------------
  TGeoPcon *copper_shape = new TGeoPcon("copper_shape", 0., 360., 3);
  copper_shape->DefineSection(0, 0.,  R_gas, box_X);
  copper_shape->DefineSection(1, 0.1, R_gas, box_X);
  copper_shape->DefineSection(2, 0.7, R_gas + 0.6, box_X);

  TGeoVolume *copper = new TGeoVolume("copper", copper_shape, pCopper);
  copper->SetLineColor(kYellow);
  copper->SetLineColor(20);
  // --------------------------------------------------------------------------
  target->AddNode(copper, 0, new TGeoCombiTrans(trans_X, trans_Y,  box_Z -0.099, fZeroRotation));
  target->AddNode(copper, 0, new TGeoCombiTrans(trans_X, trans_Y, -box_Z +0.099, fRotY));
  // --------------------------------------------------------------------------
  top->AddNode(target, 0, new TGeoCombiTrans(trans_X, trans_Y, trans_Z, fZeroRotation));
  // --------------------------------------------------------------------------

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps();
  //gGeoMan->CheckGeometryFull();
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  top->Draw("ogl");

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

  // ---------------------------------THE-ENDDDDDD ^_^ -----------------------------------------
} 

