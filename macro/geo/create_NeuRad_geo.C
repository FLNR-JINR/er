#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"




void create_NeuRad_geo()
{
  // Create a global translation
  TGeoTranslation *fGlobalTrans = new TGeoTranslation();
  fGlobalTrans->SetTranslation(0.0,0.0,1000.);

  // Create a zero rotation
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
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* NeuRad = new TGeoVolumeAssembly("NeuRad");
  // --------------------------------------------------------------------------

  //------------------ BC408 cladding -----------------------------------------
  Double_t cladding_X = 0.024; //cm
  Double_t cladding_Y = 0.6;   //cm
  Double_t cladding_Z = 0.6;   //cm
  cladding_X /= 2.;
  cladding_Y /= 2.;
  cladding_Z /= 2.;
  TGeoVolume *cladding = gGeoManager->MakeBox("cladding", pMed37, cladding_X, cladding_Y, cladding_Z);
  
  //------------------ BC408  fiber  -----------------------------------------
  Double_t fiber_X = 0.6;   //cm
  Double_t fiber_Y = 0.6;   //cm
  Double_t fiber_Z = 100.;  //cm
  fiber_X /= 2.;
  fiber_Y /= 2.;
  fiber_Z /= 2.;
  TGeoVolume *fiber = gGeoManager->MakeBox("fiber", pMed37, fiber_X, fiber_Y, fiber_Z);

  //------------------ vacuum  bundle  -----------------------------------------
  Int_t fibers_in_boundle_X_Nb = 8;
  Int_t fibers_in_boundle_Y_Nb = 8;
  
  Double_t boundle_X = fiber_X * fibers_in_boundle_X_Nb;
  Double_t boundle_Y = fiber_Y * fibers_in_boundle_Y_Nb;
  Double_t boundle_Z = fiber_Z;
  TGeoVolume *bundle = gGeoManager->MakeBox("bundle", pMed0,boundle_X, boundle_Y, boundle_Z);
  
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add claddings to fiber -----------------------------
  
  
  Double_t cladding_in_fiber_X_trans = 0.;
  Double_t cladding_in_fiber_Y_trans = 0.;
  Double_t cladding_in_fiber_Z_trans = fiber_Z - cladding_Z;
  
  fiber->AddNode(cladding, 1, new TGeoCombiTrans(cladding_in_fiber_X_trans, 
                                                  cladding_in_fiber_Y_trans, 
                                                  cladding_in_fiber_Z_trans, 
                                                  fZeroRotation));

  cladding_in_fiber_Z_trans = -cladding_in_fiber_Z_trans;
  
  fiber->AddNode(cladding, 1, new TGeoCombiTrans(cladding_in_fiber_X_trans, 
                                                  cladding_in_fiber_Y_trans, 
                                                  cladding_in_fiber_Z_trans, 
                                                  fZeroRotation));
  
  //------------------ Add fibers to bundle  -----------------------------
  Int_t i_fiber = 1;
  for (Int_t i_Y_fiber = 0; i_Y_fiber < fibers_in_boundle_Y_Nb; i_Y_fiber++){
    for (Int_t i_X_fiber = 0; i_X_fiber < fibers_in_boundle_X_Nb; i_X_fiber++){
      Double_t fiber_in_boundle_X_trans = boundle_X - fiber_X*2*(i_X_fiber)-fiber_X;
      Double_t fiber_in_boundle_Y_trans = boundle_Y - fiber_Y*2*(i_Y_fiber)-fiber_Y;
      Double_t fiber_in_boundle_Z_trans = 0.;
      bundle->AddNode( fiber, i_fiber, new TGeoCombiTrans(fiber_in_boundle_X_trans, 
                                                            fiber_in_boundle_Y_trans,
                                                            fiber_in_boundle_Z_trans, 
                                                            fZeroRotation));
      i_fiber++;
    }
  }
  
  NeuRad->AddNode(bundle, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
  top->AddNode(NeuRad, 1, new TGeoCombiTrans(.0,.0,0., fZeroRotation));

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
