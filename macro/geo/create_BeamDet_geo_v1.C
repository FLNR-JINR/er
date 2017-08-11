Double_t plasticX = 20;
Double_t plasticY = 20;
Double_t plasticZ = 0.01;

Double_t gasX = 0.125;
Double_t gasY = 5;
Double_t gasZ = 0.8; //cm

Int_t    plasticsCount = 2;
Double_t plasticOffsetZ = 1500;

Double_t mwpcOffsetZ = 20;

Double_t TOFpozZInBeamdet = -((plasticOffsetZ) / 2 + mwpcOffsetZ + 10);

Double_t MWPCpozZInBeamdet = - (((mwpcOffsetZ) / 2) + 8);

void create_BeamDet_geo_v1()
{
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  //Create gloabal Rotation
  TGeoRotation *fGlobalRotation = new TGeoRotation();
  fGlobalRotation->RotateX(0.);
  fGlobalRotation->RotateY(0.);
  fGlobalRotation->RotateZ(0.);
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
    // Create a 90 degree rotation around Z axis
  TGeoRotation *f90ZRotation = new TGeoRotation();
  f90ZRotation->RotateX(0.);
  f90ZRotation->RotateY(0.);
  f90ZRotation->RotateZ(90.);

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
TString geoFileName = geoPath + "/geometry/beamdet.v1.geo.root";
// --------------------------------------------------------------------------
// -----------------   Get and create the required media    -----------------
FairGeoMedia*   geoMedia = geoFace->getMedia();
FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

FairGeoMedium* mMylar = geoMedia->getMedium("mylar");
if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
geoBuild->createMedium(mMylar);
TGeoMedium* pMedMylar = gGeoMan->GetMedium("mylar");
if ( ! pMedMylar ) Fatal("Main", "Medium mylar not found");

FairGeoMedium* mRPCgas      = geoMedia->getMedium("RPCgas");
if ( ! mRPCgas ) Fatal("Main", "FairMedium RPCgas not found");
geoBuild->createMedium(mRPCgas);
TGeoMedium* pMedRPCgas = gGeoMan->GetMedium("RPCgas");
if ( ! pMedRPCgas ) Fatal("Main", "Medium RPCgas not found");

FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
geoBuild->createMedium(mBC408);
TGeoMedium* pMedBC408 = gGeoMan->GetMedium("BC408");
if ( ! pMedBC408 ) Fatal("Main", "Medium BC408 not found");

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
TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
gGeoMan->SetTopVolume(top);

TGeoVolume* beamdet = new TGeoVolumeAssembly("beamdet");

//Double_t beamdetBoxEdge = 2000;
//TGeoVolume* baemdet = gGeoManager->MakeBox("beamdet", pMed0, beamdetBoxEdge / 5, beamdetBoxEdge / 5, beamdetBoxEdge)


TGeoVolume* TOF     = new TGeoVolumeAssembly("TOF");
TGeoVolume* MWPC    = new TGeoVolumeAssembly("MWPC");
//TGeoVolume* MWPC1   = new TGeoVolumeAssembly("MWPC1");
//TGeoVolume* MWPC2   = new TGeoVolumeAssembly("MWPC2");
// --------------------------------------------------------------------------

gasX /= 2.0;
gasY /= 2.0;
gasZ /= 2.0;
TGeoVolume* gas   = gGeoManager->MakeBox("gas", pMedRPCgas, gasX, gasY, gasZ);

Double_t shellThickness = 20 * 1e-4; //cm
shellThickness /= 2.0;
Double_t shellX = gasY + shellThickness;
Double_t shellY = gasY + shellThickness;
Double_t shellZ = gasZ; 
TGeoVolume* shell = gGeoManager->MakeBox("shell", pMedMylar, shellX, shellY, shellZ);

plasticX /= 2.0;
plasticY /= 2.0;
plasticZ /= 2.0;
TGeoVolume* plastic = gGeoManager->MakeBox("plastic", pMedBC408, plasticX, plasticY, plasticZ);

//------------------ STRUCTURE  -----------------------------------------
//------------------ Add fibers to det  --------------------------


Double_t plasticPosZ = - plasticOffsetZ / 2;

for (Int_t i_plastic = 0; i_plastic < plasticsCount; i_plastic++)
{
  plasticPosZ = plasticPosZ + i_plastic * plasticOffsetZ;
  TOF->AddNode(plastic, i_plastic  + 1, new TGeoCombiTrans(0, 0, plasticPosZ, fZeroRotation));
}

Double_t mwpcWidth = 5;
Double_t mwpcHeight = 5;
mwpcWidth /= 2.0;
Int_t gasCountX = mwpcHeight / (2 * gasX);

Double_t gasPosX;

for(Int_t i_gas = 1; i_gas <= gasCountX; i_gas++)
{
  gasPosX = mwpcWidth - gasX * 2 * (i_gas - 1) - gasX;
  shell->AddNode(gas, i_gas, new TGeoCombiTrans(gasPosX, 0, 0, fZeroRotation));
}

TGeoVolume* MWPC1   = gGeoManager->MakeBox("MWPC1", pMed0, shellX, shellY, 2*shellZ);
TGeoVolume* MWPC2   = gGeoManager->MakeBox("MWPC2", pMed0, shellX, shellY, 2*shellZ);

mwpcOffsetZ /= 2.0;

MWPC1->AddNode(shell, 1, new TGeoCombiTrans(0, 0, -shellZ, fZeroRotation));
MWPC1->AddNode(shell, 2, new TGeoCombiTrans(0, 0, shellZ, f90ZRotation));

MWPC2->AddNode(shell, 1, new TGeoCombiTrans(0, 0, -shellZ, fZeroRotation));
MWPC2->AddNode(shell, 2, new TGeoCombiTrans(0, 0, shellZ, f90ZRotation));

MWPC->AddNode(MWPC1, 1, new TGeoCombiTrans(0, 0, -mwpcOffsetZ, fZeroRotation));
MWPC->AddNode(MWPC2, 2, new TGeoCombiTrans(0, 0, mwpcOffsetZ, fZeroRotation));

beamdet->AddNode(TOF, 1, new TGeoCombiTrans(global_X, global_Y, TOFpozZInBeamdet, fGlobalRotation));
beamdet->AddNode(MWPC, 1, new TGeoCombiTrans(global_X, global_Y, MWPCpozZInBeamdet, fGlobalRotation));

top->AddNode(beamdet, 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));
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