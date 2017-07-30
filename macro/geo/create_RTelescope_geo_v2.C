#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_RTelescope_geo_v2()
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
  TString geoFileName = geoPath + "/geometry/RTelescope.v2.geo.root";
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

  FairGeoMedium* mCsI      = geoMedia->getMedium("CsI");
  if ( ! mCsI ) Fatal("Main", "FairMedium Csi not found");
  geoBuild->createMedium(mCsI);
  TGeoMedium* pCsI = gGeoMan->GetMedium("CsI");
  if ( ! pCsI ) Fatal("Main", "Medium CsI not found");
  // --------------------------------------------------------------------------
  
  //------------------------- VOLUMES -----------------------------------------
  
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("DETgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  TGeoVolume* RTelescope = new TGeoVolumeAssembly("RTelescope");
  TGeoVolume* RingCsI = new TGeoVolumeAssembly("RingCsI");
  // --------------------------------------------------------------------------

  //------------------ RTelescope -----------------------------------------
  Double_t R_min = 1.6; //cm
  Double_t R_max = 4.1;   //cm
  Double_t thin = 0.1;   //cm
  Float_t rsp_min = 2000.;
  Float_t rsp_max = rsp_min + thin;
  Float_t thsp_min = TMath::ATan(R_min/rsp_min)*TMath::RadToDeg();
  Float_t thsp_max = TMath::ATan(R_max/rsp_max)*TMath::RadToDeg();

  //------------------ CsI crystall-----------------------------------------
  //-------------------First trapezoid--------------------------------------
  Double_t t1_dx1 = 0.5967;
  Double_t t1_dx2 = 2.0319;
  Double_t t1_dy = 2.5;
  Double_t t1_dz = 3.60742634;
  t1_dx1 /= 2;
  t1_dx2 /= 2;
  t1_dy /= 2;
  t1_dz /= 2;

  //-------------------Second trapezoid-------------------------------------
  Double_t t2_dx1 = 2.0319;
  Double_t t2_dx2 = 1.02;
  Double_t t2_dy1 = 2.5;
  Double_t t2_dy2 = 1.02;
  Double_t t2_dz = 0.8763;
  t2_dx1 /= 2;
  t2_dx2 /= 2;
  t2_dy1 /= 2;
  t2_dy2 /=2;
  t2_dz /= 2;


  //------------------ STRUCTURE  -----------------------------------------
  //------------------ Add sensor in sector -----------------------------
  Double_t deltaR = (R_max-R_min)/16;
  for (Int_t iSector=0; iSector < 16; iSector++){
    TGeoVolume *sector = gGeoManager->MakeSphere("sector",pSi,rsp_min,rsp_max,thsp_min,thsp_max,0,22.5);
    TGeoRotation *rotation = new TGeoRotation();
    rotation->RotateX(0.); 
    rotation->RotateY(0.);
    rotation->RotateZ(22.5*iSector);
    RTelescope->AddNode(sector, iSector, new TGeoCombiTrans(.0,.0,-2000., rotation));

    for (Int_t iSensor=0; iSensor < 16; iSensor++){
      Float_t thsp_min = TMath::ATan((R_min+iSensor*deltaR)/rsp_min)*TMath::RadToDeg();
      Float_t thsp_max = TMath::ATan((R_min+(iSensor+1)*deltaR)/rsp_max)*TMath::RadToDeg();
      TGeoVolume *sensor = gGeoManager->MakeSphere("sensor", pSi,rsp_min,rsp_max,thsp_min,thsp_max,0,22.5);
      sector->AddNode(sensor, iSensor, new TGeoCombiTrans(.0,.0,0., fZeroRotation));
    }
  }

  //------------------ Make CsI crystall and add  CsI crystall in ring -----------------------------
  Double_t mid = (t1_dx1+ t1_dx2)/2;
  Double_t angle = 22.5;
  Double_t prev_x = 0.;
  Double_t prev_y = 0.;
  Int_t i = 0;

  TGeoTrd1 *trap1 = new TGeoTrd1(t1_dx1, t1_dx2, t1_dy, t1_dz);
  TGeoTrd2 *trap2 = new TGeoTrd2(t2_dx1, t2_dx2, t2_dy1, t2_dy2, t2_dz);
  TGeoCombiTrans *t1 = new TGeoCombiTrans(0, 0, 0,fZeroRotation);
  TGeoCombiTrans *t2 = new TGeoCombiTrans(0, 0, t1_dz+t2_dz,fZeroRotation);
  TGeoUnion *uni = new TGeoUnion(trap1,trap2,t1,t2);
  TGeoCompositeShape *cryst_shape = new TGeoCompositeShape("cryst_shape", uni);
  for(Int_t iCryst=0;iCryst < 16; iCryst++){
    TGeoVolume *crystall = new TGeoVolume("crystall", cryst_shape, pCsI);
    TGeoRotation *fXZRotation = new TGeoRotation();
    fXZRotation->RotateX(90.);
    fXZRotation->RotateY(0.);
    fXZRotation->RotateZ(22.5*iCryst);
    RingCsI->AddNode(crystall, iCryst, new TGeoCombiTrans((prev_x + mid*TMath::Cos((angle-22.5)*TMath::DegToRad())+mid*TMath::Cos(angle*TMath::DegToRad()))*i,(prev_y +mid*TMath::Sin((angle-22.5)*TMath::DegToRad())+ mid*TMath::Sin(angle*TMath::DegToRad()))*i,0, fXZRotation));    
    prev_x += (mid*TMath::Cos((angle-22.5)*TMath::DegToRad())+mid*TMath::Cos(angle*TMath::DegToRad()))*i;
    prev_y += (mid*TMath::Sin((angle-22.5)*TMath::DegToRad())+mid*TMath::Sin(angle*TMath::DegToRad()))*i;
    if(i==1)
      angle += 22.5;
    i = 1;
  }

  RTelescope->AddNode(RingCsI, 1, new TGeoCombiTrans(.0,-3.3, thin+t1_dy+0.1, fZeroRotation));
  top->AddNode(RTelescope, 1, new TGeoCombiTrans(.0,.0,0, fZeroRotation));
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




