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
  TGeoVolume* ND = new TGeoVolumeAssembly("ND");
  // --------------------------------------------------------------------------

  //------------------ Stilbene cladding -----------------------------------------
  Double_t R_min = 0.; //cm
  Double_t R_max = 7.62;   //cm
  Double_t depth_Z1 = 10.;   //cm
  depth_Z1 /= 2.;
  TGeoVolume *cover = gGeoManager->MakeTube("cover", pMed39, R_min, R_max, depth_Z1);
  
  //------------------ Stilbene  fiber  -----------------------------------------
  Double_t depth_Z2 = 8.;  //cm
  depth_Z2 /= 2.;
  TGeoVolume *stilbene = gGeoManager->MakeTube("Stilbene", pMed37, R_min, R_max, depth_Z2);
  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add claddings to fiber -----------------------------
  cover->AddNode(stilbene, 0, new TGeoCombiTrans(0., 0.,0., fZeroRotation));

  const Double_t r = 250;				// Sphere's radius
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
  				ND->AddNode(cover, 26, new TGeoCombiTrans("r1",0,0,r,new TGeoRotation("r2",0,0,0)));
  			}
        //2. Cylinders along the Y-axis:
  			else if((i==0)&&!(j==0)){
  				alpha[j]= j*(l/r);
  				l1[j] =r*sin(alpha[j]);
  				h[j] = sqrt(4*r*r*sin(alpha[j]/2)*sin(alpha[j]/2) - r*r*sin(alpha[j])*sin(alpha[j]));
  				ND->AddNode(cover,2*j, new TGeoCombiTrans("r1",0,l1[j], r-h[j], new TGeoRotation("r2",0, -(alpha[j]*rad),0)));
  				ND->AddNode(cover,2*j+1, new TGeoCombiTrans("r1",0, -l1[j], r-h[j], new TGeoRotation("r2",0, (alpha[j]*rad),0)));
  			//3. Cylinders along the X-axis:
  			}else if((j==0)&&!(i==0)){
  				alpha[i]= i*(l/r);
  				l1[i] =r*sin(alpha[i]);
  				h[i] = sqrt(4*r*r*sin(alpha[i]/2)*sin(alpha[i]/2) - r*r*sin(alpha[i])*sin(alpha[i]));
  				ND->AddNode(cover,2*i+4, new TGeoCombiTrans("r1",l1[i],0, r-h[i], new TGeoRotation("r2",0, 0,-(alpha[i]*rad))));
  				ND->AddNode(cover,2*i+5, new TGeoCombiTrans("r1",-l1[i],0, r-h[i], new TGeoRotation("r2",0,0, (alpha[i]*rad))));
  			//4. Cylinders placed on quadrants:
  			}
  			else {
  				ND->AddNode(cover,8*i+2*j, 
  					new TGeoCombiTrans("r1",l1[i],l1[j], r-h[j]-h[i], 
  					new TGeoRotation("r2",0, -(alpha[j]*rad),-(alpha[i]*rad))));
  				ND->AddNode(cover,8*i+2*j+1, 
  					new TGeoCombiTrans("r1",l1[i], -l1[j], r-h[j]-h[i], 
  					new TGeoRotation("r2",0,(alpha[j]*rad), -(alpha[i]*rad))));
  				ND->AddNode(cover,8*i+2*j+4, 
  					new TGeoCombiTrans("r1",-l1[i],-l1[j], r-h[j]-h[i], 
  					new TGeoRotation("r2",0, (alpha[j]*rad),(alpha[i]*rad))));
  				ND->AddNode(cover,8*i+2*j+5, 
  					new TGeoCombiTrans("r1",-l1[i], l1[j], r-h[j]-h[i], 
  					new TGeoRotation("r2",0, -(alpha[j]*rad),(alpha[i]*rad))));
  			}//else
  		}//for j
  	}//for i
    top->AddNode(ND, 0, new TGeoCombiTrans(.0,.0,0., fZeroRotation));
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




