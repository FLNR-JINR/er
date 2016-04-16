#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create a zero rotation
TGeoRotation *fZeroRotation = new TGeoRotation();
fZeroRotation->RotateX(0.);
fZeroRotation->RotateY(0.);
fZeroRotation->RotateZ(0.);

Double_t transX = 0.;
Double_t transY = 0.;
Double_t transZ = 5.;

TGeoManager*   gGeoMan = NULL;

void create_muSi_geo()
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
  TString geoFileName = geoPath + "/geometry/muSi.geo.rot.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mSi      = geoMedia->getMedium("silicon");
  if ( ! mSi ) Fatal("Main", "FairMedium silicon not found");
  geoBuild->createMedium(mSi);
  TGeoMedium* pSi = gGeoMan->GetMedium("silicon");
  if ( ! pSi ) Fatal("Main", "Medium silicon not found");
  
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
  TGeoVolume* muSi = new TGeoVolumeAssembly("muSi");
  // --------------------------------------------------------------------------

  //------------------ muSi station -----------------------------------------
  Double_t station_X = 6.; //cm
  Double_t station_Y = 6.;   //cm
  Double_t station_Z = 0.03;   //cm
  station_X /= 2.;
  station_Y /= 2.;
  station_Z /= 2.;
  TGeoVolume *station = gGeoManager->MakeBox("station", pSi, station_X, station_Y, station_Z);
  
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add station to muSi -----------------------------
  muSi->AddNode(station, 0, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  muSi->AddNode(station, 1, new TGeoCombiTrans(.0,.0,5., fZeroRotation));
  TGeoRotation *Rotation = new TGeoRotation();
  Rotation->RotateX(0.);
  Rotation->RotateY(30.);
  Rotation->RotateZ(0.);
  muSi->AddNode(station, 2 , new TGeoCombiTrans(.0,.0,20., Rotation));

  top->AddNode(muSi, 1, new TGeoCombiTrans(transX, transY, transZ, fZeroRotation));

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
