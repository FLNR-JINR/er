#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

void create_QTelescope_geo_v2()
{
TGeoRotation *fZeroRotation = new TGeoRotation();
fZeroRotation->RotateX(0.);
fZeroRotation->RotateY(0.);
fZeroRotation->RotateZ(0.);
TGeoRotation *f90Rotation = new TGeoRotation();
f90Rotation->RotateX(0.);
f90Rotation->RotateY(0.);
f90Rotation->RotateZ(90.);

Float_t traslateX = 0.;
Float_t traslateY = 0.;
Float_t traslateZ = 25.;

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
TString geoFileName = geoPath + "/geometry/QTelescope.v2.geo.root";
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
TGeoVolume* QTelescope = new TGeoVolumeAssembly("QTelescope");
// --------------------------------------------------------------------------

//------------------ QTelescope station -------------------------------------
Double_t stationX = 6.1; //cm
Double_t stationY = 6.1;   //cm
Double_t stationZ = 0.1;   //cm
TGeoVolume *station = gGeoManager->MakeBox("station", pSi, stationX/2., stationY/2., stationZ/2.);
//------------------ QTelescope strip -------------------------------------
Double_t stripX = 0.1; //cm
Double_t stripY = stationY;   //cm
Double_t stripZ = stationZ;   //cm
TGeoVolume *strip = gGeoManager->MakeBox("strip", pSi, stripX/2., stripY/2., stripZ/2.);
//------------------ STRUCTURE  -----------------------------------------
//------------------ Add strips in station ---------------------------------
Int_t stripNb = (Int_t)(stationX/stripX);
for (Int_t iStrip = 0; iStrip < stripNb; iStrip++){
  Float_t translateX = (-1)*stripX*(stripNb/2) + stripX/2. + iStrip*stripX;
  station->AddNode(strip,iStrip,new TGeoCombiTrans(translateX,0.,0., fZeroRotation));
}
//------------------ Add station in QTelescope -----------------------------
QTelescope->AddNode(station,0,new TGeoCombiTrans(0.,0.,stationZ/2., fZeroRotation));
QTelescope->AddNode(station,1,new TGeoCombiTrans(0.,0.,stationZ/2.+0.65, f90Rotation));
QTelescope->AddNode(station,2,new TGeoCombiTrans(0.,0.,stationZ/2.+1.6, f90Rotation));
QTelescope->AddNode(station,3,new TGeoCombiTrans(0.,0.,stationZ/2.+2.55, f90Rotation));
QTelescope->AddNode(station,4,new TGeoCombiTrans(0.,0.,stationZ/2.+3.5, f90Rotation));
QTelescope->AddNode(station,5,new TGeoCombiTrans(0.,0.,stationZ/2.+4.45, f90Rotation));

top->AddNode(QTelescope, 0, new TGeoCombiTrans(traslateX,traslateY,traslateZ, fZeroRotation));

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




