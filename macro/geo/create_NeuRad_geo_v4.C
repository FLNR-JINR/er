/*
Геометрия для Вумпертальских тестов 
ФЭУ H12700 64 пикселя
*/

#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_NeuRad_geo_v4()
{
  // ---------------  INIT ----------------------------------------------------
  // Create a global translation
  Float_t trans_X = 0,trans_Y = 0,trans_Z = 50.; 
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);

  Double_t fiber_dead = 0.005; //cm dead layer between fibers
  Double_t fiber_X = 0.3; //cm
  Double_t fiber_Y = 0.3; //cm
  Double_t fiber_Z = 25.; //cm
  Double_t cladding_Z = 0.024; //cm pmt simulation

  Int_t fibers_in_pixel_X = 1;
  Int_t fibers_in_pixel_Y = 1;

  Int_t pixels_in_module_X_Nb = 16;
  Int_t pixels_in_module_Y_Nb = 16;
 // --------------------------------------------------------------------------
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
  TString geoFileName = geoPath + "/geometry/NeuRad.v3.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
  if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
  geoBuild->createMedium(mBC408);
  TGeoMedium* pMed37 = gGeoMan->GetMedium("BC408");
  if ( ! pMed37 ) Fatal("Main", "Medium BC408 not found");

  FairGeoMedium* mAl      = geoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
  if ( ! pMedAl ) Fatal("Main", "Medium aluminium not found");
  
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
  TGeoVolume* NeuRad = new TGeoVolumeAssembly("NeuRad");
  // --------------------------------------------------------------------------
  //------------------ BC408  fiber  -----------------------------------------
  fiber_X /= 2.;
  fiber_Y /= 2.;
  fiber_Z /= 2.;
  TGeoVolume *fiber = gGeoManager->MakeBox("fiber", pMed37, fiber_X-fiber_dead, fiber_Y-fiber_dead, fiber_Z);
  TGeoVolume *fiber_dead_zone = gGeoManager->MakeBox("fiber_dead_zone", pMed37, fiber_X, fiber_Y, fiber_Z);  
  //------------------ assembly  pixel  -----------------------------------------
  TGeoVolume* pixel = new TGeoVolumeAssembly("pixel");
  Double_t pixel_X = fiber_X *fibers_in_pixel_X;
  Double_t pixel_Y = fiber_Y *fibers_in_pixel_Y;
  //------------------ al cladding -----------------------------------------
  Double_t cladding_X = pixel_X; //cm
  Double_t cladding_Y = pixel_Y;   //cm
  cladding_Z /= 2.;
  TGeoVolume *cladding = gGeoManager->MakeBox("cladding", pMedAl, cladding_X, cladding_Y, cladding_Z);
  //------------------ vacuum  module  -----------------------------------------
  Double_t module_X = pixel_X*pixels_in_module_X_Nb;
  Double_t module_Y = pixel_Y*pixels_in_module_Y_Nb;
  Double_t module_Z = fiber_Z;
  TGeoVolume *module = new TGeoVolumeAssembly("module");
  
  //------------------ STRUCTURE  -----------------------------------------
  fiber_dead_zone->AddNode(fiber, 0, new TGeoCombiTrans(.0,.0,.0, 
                                                  fZeroRotation));
  //------------------ Add fibers to pixel  -----------------------------
  Int_t i_fiber = 0;
  for (Int_t i_Y_fiber = 0; i_Y_fiber < fibers_in_pixel_Y; i_Y_fiber++){
    for (Int_t i_X_fiber = 0; i_X_fiber < fibers_in_pixel_X; i_X_fiber++){
      Double_t fiber_in_pixel_X_trans = pixel_X - fiber_X*2*(i_X_fiber)-fiber_X;
      Double_t fiber_in_pixel_Y_trans = pixel_Y - fiber_Y*2*(i_Y_fiber)-fiber_Y;
      Double_t fiber_in_pixel_Z_trans = 0.;
      pixel->AddNode( fiber_dead_zone, i_fiber, new TGeoCombiTrans(fiber_in_pixel_X_trans, 
                                                            fiber_in_pixel_Y_trans,
                                                            fiber_in_pixel_Z_trans, 
                                                            fZeroRotation));
      i_fiber++;
    }
  }
  //------------------ Add claddings in pixel
  Double_t cladding_in_fiber_X_trans = 0.;
  Double_t cladding_in_fiber_Y_trans = 0.;
  Double_t cladding_in_fiber_Z_trans = fiber_Z + cladding_Z;
  
  pixel->AddNode(cladding, 0, new TGeoCombiTrans(cladding_in_fiber_X_trans, 
                                                  cladding_in_fiber_Y_trans, 
                                                  cladding_in_fiber_Z_trans, 
                                                  fZeroRotation));

  cladding_in_fiber_Z_trans = (-1)*cladding_in_fiber_Z_trans;
  
  pixel->AddNode(cladding, 1, new TGeoCombiTrans(cladding_in_fiber_X_trans, 
                                                  cladding_in_fiber_Y_trans, 
                                                  cladding_in_fiber_Z_trans, 
                                                  fZeroRotation));
  //------------------ Add pixel to module  -----------------------------
  Int_t i_pixel = 0;
  for (Int_t i_Y_pixel = 0; i_Y_pixel < pixels_in_module_Y_Nb; i_Y_pixel++){
    for (Int_t i_X_pixel = 0; i_X_pixel < pixels_in_module_X_Nb; i_X_pixel++){
      Double_t pixel_in_module_X_trans = module_X - pixel_X*2*(i_X_pixel)-pixel_X;
      Double_t pixel_in_module_Y_trans = module_Y - pixel_Y*2*(i_Y_pixel)-pixel_Y;
      Double_t pixel_in_module_Z_trans = 0.;
      module->AddNode( pixel, i_pixel, new TGeoCombiTrans(pixel_in_module_X_trans, 
                                                            pixel_in_module_Y_trans,
                                                            pixel_in_module_Z_trans, 
                                                            fZeroRotation));
      i_pixel++;
    }
  }
  //------------------ top structure -----------------------------
  NeuRad->AddNode(module, 0, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  top->AddNode(NeuRad, 0, new TGeoCombiTrans(trans_X,trans_Y,trans_Z, fZeroRotation));

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
