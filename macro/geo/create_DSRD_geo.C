#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_DSRD_geo()
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
  TString geoFileName = geoPath + "/geometry/DSRD.geo.root";
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
  TGeoVolume* DSRD = new TGeoVolumeAssembly("DSRD");
  // --------------------------------------------------------------------------

  //------------------ DSRD station -----------------------------------------
  Double_t R_min = 1.2; //cm
  Double_t R_max = 4.5;   //cm
  Double_t thin = .1;   //cm
  R_min /= 2.;
  R_max /= 2.;
  thin /= 2.;
  TGeoVolume *station = gGeoManager->MakeTube("station", pSi, R_min, R_max, thin);

  //------------------ DSRD sector -----------------------------------------
  TGeoTubeSeg *tubs = new TGeoTubeSeg(R_min, R_max, 20*thin,0,22.5);
  TGeoVolume *sector = new TGeoVolume("sector", tubs, pSi);
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add sensor in sector -----------------------------
  /*Double_t deltaR = (R_max-R_min)/16;
  for (Int_t iSensor=0; iSensor < 16; iSensor++){
  	//TGeoVolume *sensor = gGeoManager->MakeTubs("sensor", pSi, R_min+iSensor*deltaR, R_min+(iSensor+1)*deltaR,thin,0,22.5);
  	TGeoVolume *sensor = gGeoManager->MakeTube("sensor", pSi, R_min+iSensor*deltaR, R_min+(iSensor+1)*deltaR,thin);
  	station->AddNode(sensor, 0, new TGeoCombiTrans(0,0,0,fZeroRotation));
  }
  //------------------ Add sectors to station -----------------------------
  for (Int_t iSector=0; iSector < 16; iSector++){
  	TGeoRotation *rotation = new TGeoRotation();
	rotation->RotateX(0.); 
	rotation->RotateY(0.);
	rotation->RotateZ(iSector*22.5);
	sensor->AddNode(sector, iSector, new TGeoCombiTrans(.0,.0,0.,rotation));
  }*/
  //station->AddNode(sector, 0, new TGeoCombiTrans(.0,.0,0., fZeroRotation));
  DSRD->AddNode(sector, 1, new TGeoCombiTrans(.0,.0,0., fZeroRotation));
  top->AddNode(DSRD, 1, new TGeoCombiTrans(.0,.0,-2., fZeroRotation));
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  top->Draw();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}




