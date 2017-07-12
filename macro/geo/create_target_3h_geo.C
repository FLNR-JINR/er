#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create a zero rotation
TGeoRotation *fZeroRotation = new TGeoRotation();

Double_t transX = 0.;
Double_t transY = 0.; 
Double_t transZ = 1.;

Double_t targetR = 2.;   //cm
Double_t target3HZ = .02;
Double_t targetShell = .5;   //cm

TGeoManager*   gGeoMan = NULL;

void create_target_3h_geo()
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
  TString geoFileName = geoPath + "/geometry/target.3h.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* m3H      = geoMedia->getMedium("tritium");
  if ( ! m3H ) Fatal("Main", "FairMedium tritium not found");
  geoBuild->createMedium(m3H);
  TGeoMedium* p3H = gGeoMan->GetMedium("tritium");
  if ( ! p3H ) Fatal("Main", "Medium tritium not found"); 

  FairGeoMedium* mMylar      = geoMedia->getMedium("mylar");
  if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium* pMylar = gGeoMan->GetMedium("mylar");
  if ( ! pMylar ) Fatal("Main", "Medium mylar not found");
  
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
  TGeoVolume *target3H = gGeoManager->MakeTube("target3H", p3H, 0, targetR, target3HZ/2.);
  Double_t shellRadius = targetR+targetShell;
  Double_t shellZ = target3HZ+targetShell;
  TGeoVolume *targetShell = gGeoManager->MakeTube("targetShell", pMylar, 0, shellRadius, shellZ/2.);

  targetShell->AddNode(target3H, 0, new TGeoCombiTrans(.0, .0, .0, fZeroRotation));

  //------------------ STRUCTURE  -----------------------------------------
  TGeoVolume* target = new TGeoVolumeAssembly("target");
  target->AddNode(targetShell, 0, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  top->AddNode(target, 0, new TGeoCombiTrans(transX, transY, transZ, fZeroRotation));

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