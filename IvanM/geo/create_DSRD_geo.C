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
  thin /= 2.;
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add sensor in sector -----------------------------
  Double_t deltaR = (R_max-R_min)/16;
  for (Int_t iSector=0; iSector < 16; iSector++){
  	TString title;
  	title.Form("sensor%d", iSector);
  	TGeoVolume *sensor = gGeoManager->MakeSphere(title.Data(), pSi, R_min+iSector*deltaR, R_min+(iSector+1)*deltaR,87.5, 90.,0,22.5);
  	for (Int_t iSensor=0; iSensor < 16; iSensor++){
  		TGeoRotation *rotation = new TGeoRotation();
		rotation->RotateX(0.); 
		rotation->RotateY(0.);
		rotation->RotateZ(iSensor*22.5);
		DSRD->AddNode(sensor, iSensor, new TGeoCombiTrans(.0,.0,0., rotation));
  	}
  }
  top->AddNode(DSRD, 0, new TGeoCombiTrans(.0,.0,-5., fZeroRotation));
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




