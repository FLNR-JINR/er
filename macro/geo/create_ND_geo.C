#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_ND_geo()
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
  TString geoFileName = geoPath + "/geometry/ND.geo.root";
  // --------------------------------------------------------------------------
  
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  /*stilbene medium creation*/
  FairGeoMedium* mStilbene      = geoMedia->getMedium("Stilbene");
  if ( ! mStilbene ) Fatal("Main", "FairMedium Stilbene not found");
  geoBuild->createMedium(mStilbene);
  TGeoMedium* pMed37 = gGeoMan->GetMedium("Stilbene");
  if ( ! pMed37 ) Fatal("Main", "Medium Stilbene not found");

  /*Steel medium creation*/
  FairGeoMedium* Steel	= geoMedia->getMedium("Steel");
  if ( ! Steel ) Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(Steel);
  TGeoMedium* pMed39 = gGeoMan->GetMedium("Steel");
  if (! pMed39 ) Fatal("Main", "Medium Steel not found");

  /*vacuum medium creation*/
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
  TGeoVolume* Stilbene = new TGeoVolumeAssembly("Stilbene");
  // --------------------------------------------------------------------------

  //------------------ Stilbene cladding -----------------------------------------
  /*Double_t cladding_X = 0.024; //cm
  Double_t cladding_Y = 0.6;   //cm
  Double_t cladding_Z = 0.6;   //cm
  cladding_X /= 2.;
  cladding_Y /= 2.;
  cladding_Z /= 2.;
  TGeoVolume *cladding = gGeoManager->MakeBox("cladding", pMed37, cladding_X, cladding_Y, cladding_Z);
  */
  Double_t R_min = 0.; //cm
  Double_t R_max = 7.62;   //cm
  Double_t depth_Z1 = 10.;   //cm
  R_min /= 2.;
  R_max /= 2.;
  depth_Z1 /= 2.;
  TGeoVolume *cover = gGeoManager->MakeBox("cladding", pMed39, R_min, R_max, depth_Z1);
  
  //------------------ Stilbene  fiber  -----------------------------------------
  /*Double_t fiber_X = 0.6;   //cm
  Double_t fiber_Y = 0.6;   //cm
  Double_t fiber_Z = 100.;  //cm
  fiber_X /= 2.;
  fiber_Y /= 2.;
  fiber_Z /= 2.;
  TGeoVolume *fiber = gGeoManager->MakeBox("fiber", pMed37, fiber_X, fiber_Y, fiber_Z);
  */
  Double_t depth_Z2 = 8.;  //cm
  R_min /= 2.;
  R_max /= 2.;
  depth_Z2 /= 2.;
  TGeoVolume *stilbene = gGeoManager->MakeTube("Stilbene", pMed37, R_min, R_max, depth_Z2);
  
  //------------------ vacuum  bundle  -----------------------------------------
  /*Int_t fibers_in_boundle_X_Nb = 8;
  Int_t fibers_in_boundle_Y_Nb = 8;
  
  Double_t boundle_X = fiber_X * fibers_in_boundle_X_Nb;
  Double_t boundle_Y = fiber_Y * fibers_in_boundle_Y_Nb;
  Double_t boundle_Z = fiber_Z;
  TGeoVolume *bundle = gGeoManager->MakeBox("bundle", pMed0,boundle_X, boundle_Y, boundle_Z);
  */
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add claddings to fiber -----------------------------
  
  //Double_t covered_stilbene_X_trans = 0.;
  //Double_t covered_stilbene_Y_trans = 0.;
  //Double_t covered_stilbene_Z_trans = 0.;
  
  cover->AddNode(stilbene, 27);
  
 /* Double_t cladding_in_fiber_X_trans = 0.;
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
  */
  //----------------- Add bundles to NeuRad ----------------------------------
  /*Int_t boundles_nb = 1;
  Int_t boundles_row_nb = TMath::Sqrt(boundles_nb);
  Int_t i_boundle = 1;
  for (Int_t i_Y_boundle = 0; i_Y_boundle < boundles_row_nb; i_Y_boundle++){
    for (Int_t i_X_boundle = 0; i_X_boundle < boundles_row_nb; i_X_boundle++){
      Double_t boundle_in_fiber_X_trans = boundles_row_nb*boundle_X - i_X_boundle*2*boundle_X - boundle_X;
      Double_t boundle_in_fiber_Y_trans = boundles_row_nb*boundle_Y - i_Y_boundle*2*boundle_Y - boundle_Y;
      Double_t boundle_in_fiber_Z_trans = 0.;
      NeuRad->AddNode(bundle, i_boundle, new TGeoCombiTrans(boundle_in_fiber_X_trans,
                                                    boundle_in_fiber_Y_trans,
                                                    boundle_in_fiber_Z_trans,
                                                    fZeroRotation));
      i_boundle++;
    }
  }
  top->AddNode(NeuRad, 1, new TGeoCombiTrans(.0,.0,0., fZeroRotation));
*/
const Double_t r = 2500;				// Sphere's radius
	const Double_t l = 20;				// Length of the arc between two cylinders
	Double_t alpha[3];				// Angle of rotation of cylinders around axes:  alpha = l/r
	Double_t h[3];					// h = sqrt[4*r*r*sin(alpha/2)*sin(alpha/2) - r*r*sin(alpha)*sin(alpha)]
							// h - defines the location of the cylinder on the Z-axis
	Double_t l1[3];					// span: l1=r*sin(alpha)
	Double_t pi = 3.141592653589793;
	//TMath::Pi();
	Double_t rad = 180./pi;				// conversion of rads to degrees

for(Int_t i=0; i<3; i++){

		for(Int_t j=0; j<3; j++){

			//1. Central cylinder:
			if((i==0)&&(j==0)){
				//cover->AddNode(Stilbene,25);
				top->AddNode(cover, 26, 
					new TGeoCombiTrans("r1",0,0,r, 
					new TGeoRotation("r2",0,0,0)));

			//2. Cylinders along the Y-axis:
			}
			else if((i==0)&&!(j==0)){
				alpha[j]= j*(l/r);
				l1[j] =r*sin(alpha[j]);
				h[j] = sqrt(4*r*r*sin(alpha[j]/2)*sin(alpha[j]/2) - r*r*sin(alpha[j])*sin(alpha[j]));
				//you can write
				//h = TMath::Sqrt( TMath::Power(2*r*sin(alpha/2), 2) - TMath::Power( r*sin(alpha), 2 ) );

				//cover->AddNode(Stilbene,2*j-1);
				top->AddNode(cover,2*j, new TGeoCombiTrans("r1",0,l1[j], r-h[j], new TGeoRotation("r2",0, -(alpha[j]*rad),0)));
				//cover->AddNode(Stilbene,2*j);
				top->AddNode(cover,2*j+1, new TGeoCombiTrans("r1",0, -l1[j], r-h[j], new TGeoRotation("r2",0, (alpha[j]*rad),0)));

			//3. Cylinders along the X-axis:
			}else if((j==0)&&!(i==0)){
				alpha[i]= i*(l/r);
				l1[i] =r*sin(alpha[i]);
				h[i] = sqrt(4*r*r*sin(alpha[i]/2)*sin(alpha[i]/2) - r*r*sin(alpha[i])*sin(alpha[i]));

				//cover->AddNode(Stilbene,2*i+3);
				top->AddNode(cover,2*i+4, new TGeoCombiTrans("r1",l1[i],0, r-h[i], new TGeoRotation("r2",0, 0,-(alpha[i]*rad))));

				//cover->AddNode(Stilbene,2*i+4);
				top->AddNode(cover,2*i+5, new TGeoCombiTrans("r1",-l1[i],0, r-h[i], new TGeoRotation("r2",0,0, (alpha[i]*rad))));

			//4. Cylinders placed on quadrants:
			}
			else {
				//cover->AddNode(Stilbene,8*i+j);
				top->AddNode(cover,8*i+2*j, 
					new TGeoCombiTrans("r1",l1[i],l1[j], r-h[j]-h[i], 
					new TGeoRotation("r2",0, -(alpha[j]*rad),-(alpha[i]*rad))));
				//cover->AddNode(Stilbene,8*i+j+2);
				top->AddNode(cover,8*i+2*j+1, 
					new TGeoCombiTrans("r1",l1[i], -l1[j], r-h[j]-h[i], 
					new TGeoRotation("r2",0,(alpha[j]*rad), -(alpha[i]*rad))));
				//cover->AddNode(Stilbene,8*i+j+4);
				top->AddNode(cover,8*i+2*j+4, 
					new TGeoCombiTrans("r1",-l1[i],-l1[j], r-h[j]-h[i], 
					new TGeoRotation("r2",0, (alpha[j]*rad),(alpha[i]*rad))));
				//cover->AddNode(Stilbene,8*i+j+6);
				top->AddNode(cover,8*i+2*j+5, 
					new TGeoCombiTrans("r1",-l1[i], l1[j], r-h[j]-h[i], 
					new TGeoRotation("r2",0, -(alpha[j]*rad),(alpha[i]*rad))));
				
			}//else

		}//for j
	}//for i
  
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  top -> Draw();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}




