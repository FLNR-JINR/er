/*
Neurad prototype for Wuppertal tests. With one 64-pixel MAPMT H12700  
Todo SB: creare or find material for black paper, currently polypropilen is used!
*/

#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_NeuRad_Wupper_Proto_geo()
{
  // ---------------  INIT ----------------------------------------------------
  // Create a global translation
  Float_t trans_X = 0,trans_Y = 0, trans_Z = 50.; 
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  
  Double_t fiber_X = 0.3; //cm
  Double_t fiber_Y = 0.3; //cm
  Double_t fiber_Z = 25.; //cm
  Double_t dead_cladding_thick = 0.005; //cm dead layer between fibers (cladidng and wrapping)
  Double_t submodule_wrapping = 0.01; //cm Teflon wrapping around 8x8 fibers assembly
  Double_t housing_Thick=0.01; //cm Paper wrapping (housing) around module
  Double_t ersatz_pmt_Z = 0.24; //cm simulation of the overall pmt material
  Double_t cover_Z = 0.1; //cm simulation of the plastic cover at the opposite (w.r.t PMT) end 
  
  Int_t fibers_in_pixel_X = 2;
  Int_t fibers_in_pixel_Y = 2;
  Int_t pixels_in_submodule_X = 4;
  Int_t pixels_in_submodule_Y = 4;
  Int_t submodules_in_module_X = 2;
  Int_t submodules_in_module_Y = 2;
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
  TString geoFileName = geoPath + "/geometry/NeuRad.v4.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
  if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
  geoBuild->createMedium(mBC408);
  TGeoMedium* pBC408 = gGeoMan->GetMedium("BC408");
  if ( ! pBC408 ) Fatal("Main", "Medium BC408 not found");

   FairGeoMedium* mTefl      = geoMedia->getMedium("Tefflon");
  if ( ! mTefl ) Fatal("Main", "FairMedium Tefflon not found");
  geoBuild->createMedium(mTefl);
  TGeoMedium* pTefl = gGeoMan->GetMedium("Tefflon");
  if ( ! pTefl ) Fatal("Main", "Medium Tefflon not found");
  
   FairGeoMedium* mPolypr      = geoMedia->getMedium("polypropylene");
  if ( ! mPolypr ) Fatal("Main", "FairMedium polypropylene not found");
  geoBuild->createMedium(mPolypr);
  TGeoMedium* pPolypr = gGeoMan->GetMedium("polypropylene");
  if ( ! pPolypr ) Fatal("Main", "Medium polypropylene not found");
  
  FairGeoMedium* mAl      = geoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium* pAl = gGeoMan->GetMedium("aluminium");
  if ( ! pAl ) Fatal("Main", "Medium aluminium not found");
  
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
 
  //------------------ BC408  fiber  -----------------------------------------
 
  TGeoVolume* fiber = gGeoManager->MakeBox("fiber", pBC408, fiber_X/2.-dead_cladding_thick, fiber_Y/2.-dead_cladding_thick, fiber_Z/2.);
  TGeoVolume* dead_cladding = gGeoManager->MakeBox("dead_cladding", pBC408, fiber_X/2., fiber_Y/2., fiber_Z/2.);  
  //------------------  pixel  vacuum box -----------------------------------------
  Double_t pixel_X = fiber_X *fibers_in_pixel_X;
  Double_t pixel_Y = fiber_Y *fibers_in_pixel_Y;
  TGeoVolume* pixel = gGeoManager->MakeBox("pixel", pMed0, pixel_X/2., pixel_Y/2., fiber_Z/2.);
    
  //-------------------------submodule-------------------
 
  TGeoVolume* submodule = new TGeoVolumeAssembly("submodule");
  Double_t submodule_X = pixel_X*pixels_in_submodule_X + 2.*submodule_wrapping;
  Double_t submodule_Y = pixel_Y*pixels_in_submodule_Y + 2.*submodule_wrapping;
  //------------------ teflon  module  -----------------------------------------
  
  Double_t module_X = submodule_X*submodules_in_module_X+2.*housing_Thick;
  Double_t module_Y = submodule_Y*submodules_in_module_Y+2.*housing_Thick;
  Double_t module_Z = fiber_Z+2.*(ersatz_pmt_Z+cover_Z);
  TGeoVolume* module =  gGeoManager->MakeBox("module", pTefl, module_X/2., module_Y/2., module_Z/2.);
  
  
   //------------------  ersatz PMT, paper wrapping (module housing) and cover -----------------------------------------
    
  TGeoVolume* ersatz_pmt = gGeoManager->MakeBox("ersatz_pmt", pAl, module_X/2., module_Y/2., ersatz_pmt_Z/2.);
  TGeoVolume* cover = gGeoManager->MakeBox("cover", pPolypr, module_X/2., module_Y/2., cover_Z/2.);
  TGeoBBox* sh_module_housing_out = new TGeoBBox("sh_module_housing_out", module_X/2.,  module_Y/2., fiber_Z/2.);
  TGeoBBox* sh_module_housing_in = new TGeoBBox("sh_module_housing_in", module_X/2.-housing_Thick, module_Y/2.-housing_Thick, fiber_Z/2.);
  TGeoCompositeShape* sh_module_housing = new TGeoCompositeShape("sh_module_housing","sh_module_housing_out-sh_module_housing_in");
  TGeoVolume* module_housing = new TGeoVolume("module_housing",sh_module_housing,pPolypr);
 
  //------------------ STRUCTURE  -----------------------------------------
  dead_cladding->AddNode(fiber, 0, new TGeoCombiTrans(.0,.0,.0, 
                                                  fZeroRotation));
  //------------------ Add fibers to pixel  -----------------------------
  Int_t i_fiber = 0;
  for (Int_t i_Y_fiber = 0; i_Y_fiber < fibers_in_pixel_Y; i_Y_fiber++){
    for (Int_t i_X_fiber = 0; i_X_fiber < fibers_in_pixel_X; i_X_fiber++){
      Double_t fiber_in_pixel_X_trans = pixel_X/2. - fiber_X*(i_X_fiber+0.5);
      Double_t fiber_in_pixel_Y_trans = pixel_Y/2. - fiber_Y*(i_Y_fiber+0.5);
      Double_t fiber_in_pixel_Z_trans = 0.;
      pixel->AddNode( dead_cladding, i_fiber, new TGeoCombiTrans(fiber_in_pixel_X_trans, 
                                                            fiber_in_pixel_Y_trans,
                                                            fiber_in_pixel_Z_trans, 
                                                            fZeroRotation));
      i_fiber++;
    }
  }
  //----------- Add pixels to submodule
  
  Int_t i_pixel = 0;
  for (Int_t i_Y_pixel = 0; i_Y_pixel < pixels_in_submodule_Y; i_Y_pixel++){
    for (Int_t i_X_pixel = 0; i_X_pixel < pixels_in_submodule_X; i_X_pixel++){
      Double_t pixel_in_submodule_X_trans = pixel_X*(pixels_in_submodule_X/2.-i_X_pixel-0.5);
      Double_t pixel_in_submodule_Y_trans = pixel_Y*(pixels_in_submodule_Y/2.-i_Y_pixel-0.5);
      Double_t pixel_in_submodule_Z_trans = 0.;
      submodule->AddNode( pixel, i_pixel, new TGeoCombiTrans(pixel_in_submodule_X_trans, 
                                                            pixel_in_submodule_Y_trans,
                                                            pixel_in_submodule_Z_trans, 
                                                            fZeroRotation));
      i_pixel++;
    }
  }
// ------------Add submodules to module
Int_t i_submodule = 0;
  for (Int_t i_Y_submodule = 0; i_Y_submodule < submodules_in_module_Y; i_Y_submodule++){
    for (Int_t i_X_submodule = 0; i_X_submodule <submodules_in_module_X; i_X_submodule++){
		
      Double_t submodule_in_module_X_trans = submodule_X*(submodules_in_module_X/2.-i_X_submodule-0.5);
      Double_t submodule_in_module_Y_trans = submodule_Y*(submodules_in_module_Y/2.-i_Y_submodule-0.5);
	  Double_t submodule_in_module_Z_trans = 0.;
      module->AddNode( submodule, i_submodule, new TGeoCombiTrans(submodule_in_module_X_trans, 
                                                            submodule_in_module_Y_trans,
                                                            submodule_in_module_Z_trans, 
                                                            fZeroRotation));
      i_submodule++;
    }
  }
  
//------------------ Add ersatz_pmt, paper wrapping (module housing)and cover to module -----------
  Double_t ersatz_pmt_in_module_X_trans = 0.;
  Double_t ersatz_pmt_in_module_Y_trans = 0.;
  Double_t ersatz_pmt_in_module_Z_trans = fiber_Z/2. + ersatz_pmt_Z/2.;
  
  module->AddNode(ersatz_pmt, 0, new TGeoCombiTrans(ersatz_pmt_in_module_X_trans, 
                                                  ersatz_pmt_in_module_Y_trans, 
                                                  ersatz_pmt_in_module_Z_trans, 
                                                  fZeroRotation));

  cover_in_module_Z_trans = - fiber_Z/2. - cover_Z/2.;
  cover_in_module_X_trans = ersatz_pmt_in_module_X_trans;
  cover_in_module_Y_trans = ersatz_pmt_in_module_Y_trans;
  
  module->AddNode(cover, 0, new TGeoCombiTrans( cover_in_module_X_trans, 
                                                   cover_in_module_Y_trans, 
                                                   cover_in_module_Z_trans, 
                                                  fZeroRotation));
  
   module->AddNode(module_housing, 0, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
 
  
  
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
