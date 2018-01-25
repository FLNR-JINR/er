#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create a zero rotation
TGeoRotation *fZeroRotation = new TGeoRotation();

Double_t transX = 0.;
Double_t transY = 0.;
Double_t transZ = 0.;

TGeoManager*   gGeoMan = NULL;

void create_target_geo()
{
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/N15.media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/N15.target.root";
  // --------------------------------------------------------------------------
 
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mBe      = geoMedia->getMedium("beryllium");
  if ( ! mBe ) Fatal("Main", "FairMedium beryllium not found");
  geoBuild->createMedium(mBe);
  TGeoMedium* pBe = gGeoMan->GetMedium("beryllium");
  if ( ! pBe ) Fatal("Main", "Medium silicon not found");
  
  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
 // if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
  // --------------------------------------------------------------------------

// ------ Create media for target -------------------------------------------
FairGeoMedium* mborum11  = geoMedia->getMedium("borum11");
if ( ! mborum11  ) Fatal("Main", "FairMedium borum11 not found");
geoBuild->createMedium(mborum11);
TGeoMedium* pborum11 = gGeoMan->GetMedium("borum11");
if ( ! pborum11  ) Fatal("Main", "Medium borum11 not found"); 

FairGeoMedium* mSteel      = geoMedia->getMedium("Steel");
if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
geoBuild->createMedium(mSteel);
TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
if ( ! pSteel ) Fatal("Main", "Medium vacuum not found");
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  //------------------ target -----------------------------------------
  //Double_t target_R = 2.;   //cm
  //Double_t target_Z = 2.;   //cm
  //target_Z /= 2.;

// -------- Target parameters -----------------------------------------------
  Double_t targetB11R = 1.5;   //cm
  Double_t targetB11Z = 0.0007;   //cm 7 micron
  Double_t targetShellThickness=0.5;

Double_t targetShellZ =0.002;
  //TGeoVolume *target = gGeoManager->MakeTube("target_vol", pBe, 0, target_R, target_Z);


Double_t targetShellRmin = targetB11R ;
Double_t targetShellRmax = targetB11R + targetShellThickness;

TGeoVolume *targetB11 = gGeoManager->MakeTube("targetB11", pborum11, 0, targetB11R, targetB11Z/2);

TGeoVolume * targetShell = gGeoManager->MakeTube("targetShell", pSteel, targetShellRmin, targetShellRmax, targetShellZ/2);
  
  //------------------ STRUCTURE  -----------------------------------------
  TGeoVolume* targetAss = new TGeoVolumeAssembly("target");
  targetAss->AddNode(targetB11, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  targetAss->AddNode(targetShell, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
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
