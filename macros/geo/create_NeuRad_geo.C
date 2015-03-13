#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create a global translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
fGlobalTrans->SetTranslation(0.0,0.0,1000.);

// Create a zero rotation
TGeoRotation *fZeroRotation = new TGeoRotation();
fZeroRotation->RotateX(0.);
fZeroRotation->RotateY(0.);
fZeroRotation->RotateZ(0.);

TGeoManager*   gGeoMan = NULL;

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
  
  //------------------ BC408  module  -----------------------------------------
  Double_t module_X = 0.6;   //cm
  Double_t module_Y = 0.6;   //cm
  Double_t module_Z = 50.;  //cm
  module_X /= 2.;
  module_Y /= 2.;
  module_Z /= 2.;
  TGeoVolume *module = gGeoManager->MakeBox("module", pMed37, module_X, module_Y, module_Z);

  //------------------ vacuum  bundle  -----------------------------------------
  Int_t modules_in_boundle_X_Nb = 8;
  Int_t modules_in_boundle_Y_Nb = 8;
  
  Double_t boundle_X = module_X * modules_in_boundle_X_Nb;
  Double_t boundle_Y = module_Y * modules_in_boundle_Y_Nb;
  Double_t boundle_Z = module_Z;
  TGeoVolume *bundle = gGeoManager->MakeBox("bundle", pMed0,boundle_X, boundle_Y, boundle_Z);
  
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add claddings to module -----------------------------
  
  
  Double_t cladding_in_module_X_trans = 0.;
  Double_t cladding_in_module_Y_trans = 0.;
  Double_t cladding_in_module_Z_trans = module_Z - cladding_Z;
  
  module->AddNode(cladding, 1, new TGeoCombiTrans(cladding_in_module_X_trans, 
                                                  cladding_in_module_Y_trans, 
                                                  cladding_in_module_Z_trans, 
                                                  fZeroRotation));

  cladding_in_module_Z_trans = -cladding_in_module_Z_trans;
  
  module->AddNode(cladding, 1, new TGeoCombiTrans(cladding_in_module_X_trans, 
                                                  cladding_in_module_Y_trans, 
                                                  cladding_in_module_Z_trans, 
                                                  fZeroRotation));
  
  //------------------ Add modules to bundle  -----------------------------
  Int_t i_module = 1;
  for (Int_t i_Y_module = 0; i_Y_module < modules_in_boundle_Y_Nb; i_Y_module++){
    for (Int_t i_X_module = 0; i_X_module < modules_in_boundle_X_Nb; i_X_module++){
      Double_t module_in_boundle_X_trans = boundle_X - module_X*(i_X_module+1);
      Double_t module_in_boundle_Y_trans = boundle_Y - module_Y*(i_Y_module+1);
      Double_t module_in_boundle_Z_trans = 0.;
      bundle->AddNode( module, i_module, new TGeoCombiTrans(module_in_boundle_X_trans, 
                                                            module_in_boundle_Y_trans,
                                                            module_in_boundle_Z_trans, 
                                                            fZeroRotation));
      i_module++;
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
