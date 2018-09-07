#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_KZ_geo()
{
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
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/KZ.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();


  FairGeoMedium* miron = geoMedia->getMedium("iron");
  if (!miron) Fatal("create_geo", "FairMedium iron not found");
  geoBuild->createMedium(miron);
  TGeoMedium* piron = gGeoMan->GetMedium("iron");
  if (!piron) Fatal("create_geo", "Medium iron not found");

  FairGeoMedium* mhelium = geoMedia->getMedium("helium");
  if (!mhelium) Fatal("create_geo", "FairMedium helium not found");
  geoBuild->createMedium(mhelium);
  TGeoMedium* phelium = gGeoMan->GetMedium("helium");
  if (!phelium) Fatal("create_geo", "Medium helium not found");

  FairGeoMedium* mStilbene = geoMedia->getMedium("Stilbene");
  if (!mStilbene) Fatal("create_geo", "FairMedium Stilbene not found");
  geoBuild->createMedium(mStilbene);
  TGeoMedium* pStilbene = gGeoMan->GetMedium("Stilbene");
  if (!pStilbene) Fatal("create_geo", "Medium Stilbene not found");

  FairGeoMedium* mMgO = geoMedia->getMedium("MgO");
  if (!mMgO) Fatal("create_geo", "FairMedium MgO not found");
  geoBuild->createMedium(mMgO);
  TGeoMedium* pMgO = gGeoMan->GetMedium("MgO");
  if (!pMgO) Fatal("create_geo", "Medium MgO not found");

  FairGeoMedium* maluminium = geoMedia->getMedium("aluminium");
  if (!maluminium) Fatal("create_geo", "FairMedium aluminium not found");
  geoBuild->createMedium(maluminium);
  TGeoMedium* paluminium = gGeoMan->GetMedium("aluminium");
  if (!paluminium) Fatal("create_geo", "Medium aluminium not found");

  FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
  if (!mSteel) Fatal("create_geo", "FairMedium Steel not found");
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
  if (!pSteel) Fatal("create_geo", "Medium Steel not found");
  

  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("KZgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
//  TGeoVolume* ChamberVol = new TGeoVolumeAssembly("Chamber");
  TGeoVolume* ND = new TGeoVolumeAssembly("ND");
  // --------------------------------------------------------------------------


 //------------------ chamber -----------------------------------------
  Double_t R_min_chamber = 0.; //cm
  Double_t R_max_chamber = 7.6;   //cm
  Double_t HeightZ_chamber = 52.4;   //cm

  //------------------ helium -----------------------------------------
  Double_t R_min_helium = 0.; //cm
  Double_t R_max_helium = 7.5;   //cm
  Double_t HeightZ_helium = 50.;   //cm

  //------------------ neutron detector -----------------------------------------

  Double_t R_min_Crystal = 0.0; // cm
  Double_t R_max_Crystal = 4.0; // cm
  Double_t HeightZ_Crystal = 5.0; // cm

  Double_t R_min_MgO = 0.0; // cm
  Double_t R_max_MgO = 4.1; // cm
  Double_t HeightZ_MgO = 5.1; // cm

  Double_t R_min_Shell = 0.0; // cm
  Double_t R_max_Shell = 4.15; // cm
  Double_t HeightZ_Shell = 5.15; // cm

  Double_t R_min_Housing = 0.0; // cm
  Double_t R_max_Housing = 4.3; // cm
  Double_t HeightZ_Housing = 5.3; // cm


  TGeoVolume *stilbene = gGeoManager->MakeTube("Stilbene", pStilbene, R_min_Crystal, R_max_Crystal, HeightZ_Crystal/2.);  
  TGeoVolume *MgO = gGeoManager->MakeTube("MgO", pMgO, R_min_MgO, R_max_MgO, HeightZ_MgO/2.); 
  TGeoVolume *shell = gGeoManager->MakeTube("shell", paluminium, R_min_Shell, R_max_Shell, HeightZ_Shell/2.); 
  TGeoVolume *housing = gGeoManager->MakeTube("housing", pSteel, R_min_Housing, R_max_Housing, HeightZ_Housing/2.); 


  TGeoVolume *helium = gGeoManager->MakeTube("helium", phelium, R_min_helium, R_max_helium, HeightZ_helium/2.); 
  TGeoVolume *chamber = gGeoManager->MakeTube("chamber", piron, R_min_chamber, R_max_chamber, HeightZ_chamber/2.);

/*
  TGeoVolume *chamber = gGeoManager->MakeBox("chamber", piron, chamber_X/2., chamber_Y/2., chamber_Z/2.);
  TGeoVolume *hell = gGeoManager->MakeBox("hell", phelium, hell_X/2., hell_Y/2., hell_Z/2.);
*/

  MgO->AddNode(stilbene, 0, new TGeoCombiTrans(0., 0., 0., fZeroRotation));
  shell->AddNode(MgO, 0, new TGeoCombiTrans(0., 0., 0., fZeroRotation));
  housing->AddNode(shell, 0, new TGeoCombiTrans(0., 0., 0., fZeroRotation));


  chamber->AddNode(helium, 0, new TGeoCombiTrans(0., 0., 0., fZeroRotation));


  ND->AddNode(housing, 0, new TGeoCombiTrans(.0,.0,28.95, fZeroRotation)); 
  ND->AddNode(chamber, 0, new TGeoCombiTrans(.0,.0,0.0, fZeroRotation));

  top->AddNode(ND, 0, new TGeoCombiTrans(.0,.0,0., fZeroRotation));

// ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  top->Draw();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

  TBrowser* bro = new TBrowser("bro", "bro");
  gGeoMan->GetTopVolume()->Draw("ogl");
  // --------------------------------------------------------------------------
}




