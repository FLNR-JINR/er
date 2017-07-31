// ----- BeamDet parameters -------------------------------------------------
Double_t plateX = 100;
Double_t plateY = 100;
Double_t plateZ = 0.01;

Double_t gasX = 0.125;
Double_t gasY = 5;
Double_t gasZ = 0.8; //cm

Int_t    platesCount = 2;
Double_t plateOffsetZ = 1500;

Double_t mwpcOffsetZ = 20;

Double_t TOFpozZInBeamdet = -((plateOffsetZ) / 2 + mwpcOffsetZ + 10);

Double_t MWPCpozZInBeamdet = - (((mwpcOffsetZ) / 2) + 8);

// -------- Target parameters -----------------------------------------------

Double_t targetH2R = 2.;   //cm
Double_t targetH2Z = 0.4;   //cm

Double_t transTargetX = 0.;
Double_t transTargetY = 0.; 
Double_t transTargetZ = targetH2Z / 2;

Double_t shellThickness = 1.5 * 1e-4;

void create_BeamDet_geo_v2()
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
TString geoFileName = geoPath + "/geometry/beamdet.v2.geo.root";
// --------------------------------------------------------------------------
// -----------------   Get and create the required media    -----------------
FairGeoMedia*   geoMedia = geoFace->getMedia();
FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

// ----- Create media for TOF -----------------------------------------------
FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
geoBuild->createMedium(mBC408);
TGeoMedium* pMedBC408 = gGeoMan->GetMedium("BC408");
if ( ! pMedBC408 ) Fatal("Main", "Medium BC408 not found");
// --------------------------------------------------------------------------
// ----- Create media for MWPC ----------------------------------------------
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
// --------------------------------------------------------------------------
// ------ Create media for target -------------------------------------------
FairGeoMedium* mH2      = geoMedia->getMedium("H2");
if ( ! mH2 ) Fatal("Main", "FairMedium H2 not found");
geoBuild->createMedium(mH2);
TGeoMedium* pH2 = gGeoMan->GetMedium("H2");
if ( ! pH2 ) Fatal("Main", "Medium H2 not found"); 

FairGeoMedium* mSteel      = geoMedia->getMedium("Steel");
if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
geoBuild->createMedium(mSteel);
TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
if ( ! pSteel ) Fatal("Main", "Medium vacuum not found");
// --------------------------------------------------------------------------
// ------ Create vacuum media -----------------------------------------------
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
TGeoVolume* TOF     = new TGeoVolumeAssembly("TOF");
TGeoVolume* MWPC    = new TGeoVolumeAssembly("MWPC");
TGeoVolume* target  = new TGeoVolumeAssembly("target");

// --------------------------------------------------------------------------
// ---------------- target --------------------------------------------------
targetH2Z /= 2.;

Double_t targetShellR = targetH2R + shellThickness;
Double_t targetShellZ = targetH2Z;

TGeoVolume *targetH2 = gGeoManager->MakeTube("targetH2", pH2, 0, targetH2R, targetH2Z);
TGeoVolume *targetShell = gGeoManager->MakeTube("targetShell", pSteel, 0, targetShellR, targetShellZ);
//TGeoVolume *targetH2 = gGeoManager->MakeBox("targetH2", pH2, targetH2R, targetH2R, targetH2Z);
//TGeoVolume *targetShell = gGeoManager->MakeBox("targetShell", pSteel, targetShellR, targetShellR, targetShellZ);
// --------------------------------------------------------------------------
// ----------------- MWPC ---------------------------------------------------
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
// --------------------------------------------------------------------------
// ---------------- TOF -----------------------------------------------------
plateX /= 2.0;
plateY /= 2.0;
plateZ /= 2.0;
TGeoVolume* plate = gGeoManager->MakeBox("plate", pMedBC408, plateX, plateY, plateZ);
// --------------------------------------------------------------------------
//------------------ STRUCTURE  ---------------------------------------------

Double_t platePosZ = - plateOffsetZ / 2;

for (Int_t i_plate = 0; i_plate < platesCount; i_plate++)
{
  platePosZ = platePosZ + i_plate * plateOffsetZ;
  TOF->AddNode(plate, i_plate  + 1, new TGeoCombiTrans(0, 0, platePosZ, fZeroRotation));
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

targetShell->AddNode(targetH2, 1, new TGeoCombiTrans(.0, .0, .0, fZeroRotation));
target->AddNode(targetShell, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));

beamdet->AddNode(target, 1, new TGeoCombiTrans(transTargetX, transTargetY, transTargetZ, fZeroRotation));
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