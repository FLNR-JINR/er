#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_target_h2_geo() {

  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  
  // create rotation for sector
  TGeoRotation *fRotation = new TGeoRotation();
  fRotation->RotateX(180.);
  fRotation->RotateY(0.);
  fRotation->RotateZ(0.);
  
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
  TString geoFileName = geoPath + "/geometry/target.h2.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mH2 = geoMedia->getMedium("H2");
  if ( ! mH2 ) Fatal("Main", "FairMedium H2 not found");
  geoBuild->createMedium(mH2);
  TGeoMedium* pH2 = gGeoMan->GetMedium("H2");
  if ( ! pH2 ) Fatal("Main", "Medium H2 not found"); 

  FairGeoMedium* mMylar = geoMedia->getMedium("mylar");
  if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium* pMylar = gGeoMan->GetMedium("mylar");
  if ( ! pMylar ) Fatal("Main", "Medium mylar not found");

  FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
  if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
  if ( ! pSteel ) Fatal("Main", "Medium vacuum not found");
  
  FairGeoMedium* vacuum = geoMedia->getMedium("vacuum");
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
  TGeoVolume* target = new TGeoVolumeAssembly("target");
  // --------------------------------------------------------------------------
  // box of steel
  Double_t box_X,box_Y,box_Z; // box of steel

  box_X = 6.; //cm
  box_Y = 6.; //cm
  box_Z = 0.4; //cm
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;
  // --------------------------------------------------------------------------
  TGeoBBox box1 = TGeoBBox("box1", box_X, box_Y, box_Z);
  TGeoTube tube1 = TGeoTube("tube1", 0., 1., box_Z);

  TGeoCompositeShape *box2 = new TGeoCompositeShape("box2", "(box1) - (tube1)");
  TGeoVolume* boxST = new TGeoVolume("boxST", box2, pSteel);
  // --------------------------------------------------------------------------

  TGeoVolume *tube = gGeoManager->MakeTube("tube", pH2, 0, 1., box_Z + 0.15);

  // --------------------------------------------------------------------------
// make tubes
  Double_t tubeR,Rexit,tubeZ;
  tubeR = 1.;
  Rexit = 2.5; // cm
  tubeZ = 0.0002; 
  tubeZ /= 2.;

  TGeoVolume *entry = gGeoManager->MakeTube("entry", pMylar, 0, tubeR, tubeZ);
  TGeoVolume *exit = gGeoManager->MakeTube("exit", pMylar, 0, Rexit, tubeZ);
  // --------------------------------------------------------------------------

  Double_t R_min,R_max,thsp_min,thsp_max;
  thsp_min = 0;
  thsp_max = TMath::ATan(10./49.5)*TMath::RadToDeg();
  R_max = 5.05;   //cm
  R_min = R_max - 0.1; //cm

  TGeoSphere *sector1 = new TGeoSphere("sector1",R_min,R_max,thsp_min,thsp_max,0,360.);
  TGeoSphere *sector2 = new TGeoSphere("sector2",R_min,R_max,thsp_min,thsp_max,0,360.);  

// tube of H2
  TGeoTube tubeH = TGeoTube("tubeH", 0., 1., box_Z);

  TGeoCombiTrans* trans1 = new TGeoCombiTrans("trans1", 0., 0., - box_Z + R_max - 0.1, fRotation);
  trans1->RegisterYourself();
  TGeoCombiTrans* trans2 = new TGeoCombiTrans("trans2", 0., 0., box_Z - R_max + 0.1 ,fZeroRotation); 
  trans2->RegisterYourself();

  TGeoCompositeShape *Htube = new TGeoCompositeShape("Htube", "sector1:trans1 + tubeH + sector2:trans2");
  TGeoVolume* tubeH2 = new TGeoVolume("tubeH2", Htube, pH2);
// tube of steel

  TGeoCombiTrans* trans3 = new TGeoCombiTrans("trans3", 0., 0., - box_Z - 0.0006 + R_max - 0.1, fRotation);
  trans3->RegisterYourself();
  TGeoCombiTrans* trans4 = new TGeoCombiTrans("trans4", 0., 0.,  box_Z + 0.0006  - R_max + 0.1, fZeroRotation); 
  trans4->RegisterYourself();

  TGeoTube tubeS = TGeoTube("tubeS", 0., 1., box_Z + 0.0006);

  TGeoCompositeShape *steelTube = new TGeoCompositeShape("steelTube", "sector1:trans3 + tubeS + sector2:trans4");
  TGeoVolume* tubeSt = new TGeoVolume("tubeSt", steelTube, pSteel);

  tubeSt->AddNode(tubeH2, 0, new TGeoCombiTrans(.0, .0, .0, fZeroRotation));
  // --------------------------------------------------------------------------
  target->AddNode(entry, 0, new TGeoCombiTrans(trans_X,trans_Y, -box_Z - 0.15 - 0.5, fZeroRotation));
  target->AddNode(exit, 0, new TGeoCombiTrans(trans_X,trans_Y, box_Z + 0.15 + 0.5, fZeroRotation));
  target->AddNode(boxST,0,new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  target->AddNode(tubeSt,0,new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  top->AddNode(target,0,new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
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

