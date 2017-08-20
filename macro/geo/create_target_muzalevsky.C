#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_target_muzalevsky() {

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
  TString geoFileName = geoPath + "/geometry/target.h.IM.root";
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
  // --------------------------------------------------------------------------
  // box of steel
  Double_t box_X,box_Y,box_Z; // box of steel

  box_X = 6.; //cm
  box_Y = 6.; //cm
  box_Z = 0.4; //cm
  
  box_X /= 2.;
  box_Y /= 2.;
  box_Z /= 2.;
  
  TGeoVolume *boxST = gGeoManager->MakeBox("boxST", pSteel, box_X, box_Y, box_Z);
  // --------------------------------------------------------------------------
  // make tubes
  Double_t tubeR,Rexit;
  tubeR = 1.;
  Rexit = 5.; // cm
  
 	TGeoTube *tubeH = new TGeoTube("tubeH", 0, tubeR, box_Z);
  
 	TGeoTube *tubeS = new TGeoTube("tubeS", 0, tubeR, box_Z + 0.0006); // 0.0006 or 0.0003???
 // Double_t rmin, Double_t rmax, Double_t dz
  TGeoVolume *entry = gGeoManager->MakeTube("entry", pMylar, 0, tubeR, 0.0002);
  
  TGeoVolume *exit = gGeoManager->MakeTube("exit", pMylar, 0, Rexit, 0.0002);
  // --------------------------------------------------------------------------
  // tube of hydrogen
  Double_t R_min = 4.95; //cm
  Double_t R_max = 5.05;   //cm
  Double_t thin = 0.1;   //cm
  Float_t thsp_min = 0.;
  Float_t thsp_max = TMath::ATan(1./R_min)*TMath::RadToDeg();
  
  TGeoSphere *sector1 = new TGeoSphere("sector1",R_min,R_max,thsp_min,thsp_max,0,360.);
  TGeoSphere *sector2 = new TGeoSphere("sector2",R_min,R_max,thsp_min,thsp_max,0,360.);

  TGeoCombiTrans* trans1 = new TGeoCombiTrans("trans1", 0., 0., - 2*box_Z - 0.1 + (R_min+R_max)/2, fRotation);
  trans1->RegisterYourself();
  TGeoCombiTrans* trans2 = new TGeoCombiTrans("trans2", 0., 0.,  2*box_Z + 0.1 - (R_min+R_max)/2, fZeroRotation); 
  trans2->RegisterYourself();
  
  TGeoCompositeShape *hydrogen = new TGeoCompositeShape("hydrogen", "sector1:trans1 + tubeH + sector1:trans2");
 	TGeoVolume* tubeVol = new TGeoVolume("tubeVol", hydrogen, pH2);
  
  tubeVol_trans_X = 0.;
  tubeVol_trans_Y = 0.;
  tubeVol_trans_Z = 0.;
  // --------------------------------------------------------------------------
  // tube of steel
  TGeoCombiTrans* trans3 = new TGeoCombiTrans("trans3", 0., 0., - 2*box_Z - 0.1003 + (R_min+R_max)/2, fRotation);
  trans3->RegisterYourself();
  TGeoCombiTrans* trans4 = new TGeoCombiTrans("trans4", 0., 0.,  2*box_Z + 0.1003 - (R_min+R_max)/2, fZeroRotation); 
  trans4->RegisterYourself();
  
  TGeoCompositeShape *steelTube = new TGeoCompositeShape("steelTube", "sector1:trans3 + tubeS + sector1:trans4");
 	TGeoVolume* tubeSt = new TGeoVolume("tubeSt", steelTube, pSteel);
  
  tubeSt->AddNode(tubeVol, 0, new TGeoCombiTrans(tubeVol_trans_X, tubeVol_trans_Y, tubeVol_trans_Z, fZeroRotation));
  
  tubeSt_trans_X = 0.;
  tubeSt_trans_Y = 0.;
  tubeSt_trans_Z = 0.;
  // --------------------------------------------------------------------------
  
  // -------------------------------------------------------------------------- 
  boxST->AddNode(tubeSt, 0, new TGeoCombiTrans(tubeSt_trans_X, tubeSt_trans_Y, tubeSt_trans_Z, fZeroRotation));
  
  top->AddNode(boxST, 0, new TGeoCombiTrans(trans_X,trans_Y,trans_Z, fZeroRotation));
  top->AddNode(entry, 0, new TGeoCombiTrans(trans_X,trans_Y, -2*box_Z - 0.1003 - 0.5, fZeroRotation));
  top->AddNode(exit, 0, new TGeoCombiTrans(trans_X,trans_Y, 2*box_Z + 0.1003 + 0.5, fZeroRotation));
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

