#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"



void create_collimator_geo()
{
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();

  Double_t transX = 0.;
  Double_t transY = 0.;
  Double_t transZ = 0.;

  TGeoManager*   gGeoMan = NULL;

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
  Double_t positionCollimator= -16.4;   //cm
  Double_t positionD1= -30.8;
  Double_t positionD1_2=-11.8;
  Double_t positionD2= -2. ;
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/N15.collimator.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mFe      = geoMedia->getMedium("ferum");
  if ( ! mFe ) Fatal("Main", "FairMedium ferum not found");
  geoBuild->createMedium(mFe);
  TGeoMedium* pFe = gGeoMan->GetMedium("ferum");
  if ( ! pFe ) Fatal("Main", "Medium ferum not found");
  
  FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");

// ------ Create media for collimator -----------------------------------------------
FairGeoMedium* maluminium   = geoMedia->getMedium("aluminium");
if ( ! maluminium ) Fatal("Main", "FairMedium aluminium not found");
geoBuild->createMedium(maluminium);
TGeoMedium* pmaluminium = gGeoMan->GetMedium("aluminium");
if ( ! pmaluminium ) Fatal("Main", "Medium aluminium not found");
  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  //------------------ target -----------------------------------------
  //Double_t R_min = .3;   //cm
  //Double_t R_max = 10.;   //cm
  //Double_t Z = 2.;   //cm
 // Z /= 2.;

//------------- Collimator parameters  ------------- ------
Double_t CoRmin = 1.5 ;
Double_t CoRmax = 1.6;   //cm
Double_t CoZ = 28.8; 

Double_t D1Rmin = .5;
Double_t D1Rmax = 1.5;   //cm
Double_t D1Z = 0.2; 

Double_t D2Rmin = .5;
Double_t D2Rmax = 1.5;   //cm
Double_t D2Z = 0.2 ; 
   
TGeoVolume* collimator= gGeoManager->MakeTube("collimator", pmaluminium, CoRmin,CoRmax, CoZ/2);

TGeoVolume* D1= gGeoManager->MakeTube("D1", pmaluminium, D1Rmin,D1Rmax, D1Z/2);

TGeoVolume* D2= gGeoManager->MakeTube("D2", pmaluminium, D2Rmin,D2Rmax, D2Z/2);
  //------------------ STRUCTURE  -----------------------------------------
  TGeoVolume* CollimatorAss = new TGeoVolumeAssembly("Collimator");




CollimatorAss->AddNode(D1, 1, new TGeoCombiTrans(transX, transY, positionD1, fZeroRotation));
CollimatorAss ->AddNode(D1, 2, new TGeoCombiTrans(transX, transY, positionD1_2, fZeroRotation));
CollimatorAss->AddNode(D2, 1, new TGeoCombiTrans(transX, transY, positionD2, fZeroRotation));
  CollimatorAss->AddNode(collimator, 1, new TGeoCombiTrans(transX, transY, positionCollimator, fZeroRotation));
  top->AddNode(CollimatorAss, 1, new TGeoCombiTrans(transX, transY, transZ, fZeroRotation));

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
