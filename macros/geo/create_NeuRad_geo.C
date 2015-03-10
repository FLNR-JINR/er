#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
fGlobalTrans->SetTranslation(0.0,0.0,1000.);
TGeoRotation *fRefRot = NULL;

TGeoManager*   gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi   = 0.;
Double_t fTheta = 0.;
Double_t fPsi   = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 1550.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kTRUE;

void create_NeuRad_geo()
{
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
  TString geoFileName = geoPath + "/geometry/NeuRad.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
  if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
  geoBuild->createMedium(mBC408);
  TGeoMedium* pMed37 = gGeoMan->GetMedium("BC408");
  if ( ! pMed37 ) Fatal("Main", "Medium BC408 not found");
  
  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
  // --------------------------------------------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  //------------------ BC408 paddles -----------------------------------------
  TGeoVolume *mini_padle = gGeoManager->MakeBox("mini_padle", pMed37,500.,25.,25.);

  TGeoVolume *container = gGeoManager->MakeBox("container", pMed0,500.,50.,50.);

  TGeoRotation *zeroRotation = new TGeoRotation();
  zeroRotation->RotateX(0.);
  zeroRotation->RotateY(0.);
  zeroRotation->RotateZ(0.);
  
  container->AddNode(mini_padle, 1, new TGeoCombiTrans(0., 25., -25., zeroRotation));
  container->AddNode(mini_padle, 2, new TGeoCombiTrans(0., 25., 25., zeroRotation));
  container->AddNode(mini_padle, 3, new TGeoCombiTrans(0., -25., -25., zeroRotation));
  container->AddNode(mini_padle, 4, new TGeoCombiTrans(0., -25., 25., zeroRotation));
  
  top->AddNode(container, 1, /*GetGlobalPosition(t0)*/new TGeoCombiTrans(0.,0.,1000., zeroRotation));

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
