// ----- BeamDet parameters -------------------------------------------------
// ----- ToF parameters -----------------------------------------------------
Double_t plasticX = 100;
Double_t plasticY = 100;
Double_t plasticZ = 0.01;

// --------------------------------------------------------------------------
// ----- MWPC parameters ----------------------------------------------------
Double_t gasVolX = 5.;
Double_t gasVolY = 5.;
Double_t gasVolZ = 8.2;

Double_t gasX = 0.125;
Double_t gasY = 5.;
Double_t gasZ = 0.6; //cm

Double_t distBetweenXandY = 1.;

Double_t aluminiumThickness = 5 * 1e-4; // cathode

Double_t kaptonThickness = 12.5 * 1e-4; //kaptonWindow

Double_t wireDiameter  = 20 * 1e-4; //anodeWire
// --------------------------------------------------------------------------
// ------ Position of detector's parts relative to zero ---------------------
Double_t positionToF1 = -1550.;
Double_t positionToF2 = -50.;

Double_t positionMWPC1 = -40.;
Double_t positionMWPC2 = -8.;
// -------- Target parameters -----------------------------------------------
Double_t targetH2R = 2.;   //cm
Double_t targetH2Z = 0.4;   //cm

Double_t targetShellThicknessSide = 20 * 1e-4;
Double_t targetShellThicknessZ = 6 * 1e-4;

Double_t transTargetX = 0.;
Double_t transTargetY = 0.; 
Double_t transTargetZ = 0.;
// --------------------------------------------------------------------------

void create_BeamDet_geo_v3()
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
  // Create a 90 degree rotation around X axis
  TGeoRotation *f90XRotation = new TGeoRotation();
  f90XRotation->RotateX(90.);
  f90XRotation->RotateY(0.);
  f90XRotation->RotateZ(0.);

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
TString geoFileName = geoPath + "/geometry/beamdet.v3.geo.root";
// --------------------------------------------------------------------------
// -----------------   Get and create the required media    -----------------
FairGeoMedia*   geoMedia = geoFace->getMedia();
FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

// ----- Create media for ToF -----------------------------------------------
FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
geoBuild->createMedium(mBC408);
TGeoMedium* pMedBC408 = gGeoMan->GetMedium("BC408");
if ( ! pMedBC408 ) Fatal("Main", "Medium BC408 not found");
// --------------------------------------------------------------------------
// ----- Create media for MWPC ----------------------------------------------
FairGeoMedium* mCF4      = geoMedia->getMedium("CF4_CH4");
if ( ! mCF4 ) Fatal("Main", "FairMedium CF4_CH4 not found");
geoBuild->createMedium(mCF4);
TGeoMedium* pMedCF4 = gGeoMan->GetMedium("CF4_CH4");
if ( ! pMedCF4 ) Fatal("Main", "Medium CF4_CH4 not found");

FairGeoMedium* mKapton      = geoMedia->getMedium("kapton");
if ( ! mKapton ) Fatal("Main", "FairMedium kapton not found");
geoBuild->createMedium(mKapton);
TGeoMedium* pMedKapton = gGeoMan->GetMedium("kapton");
if ( ! pMedKapton ) Fatal("Main", "Medium kapton not found");

FairGeoMedium* mAluminium      = geoMedia->getMedium("aluminium");
if ( ! mAluminium ) Fatal("Main", "FairMedium aluminium not found");
geoBuild->createMedium(mAluminium);
TGeoMedium* pMedAluminium = gGeoMan->GetMedium("aluminium");
if ( ! pMedAluminium ) Fatal("Main", "Medium aluminium not found");

FairGeoMedium* mTungsten      = geoMedia->getMedium("tungsten");
if ( ! mTungsten ) Fatal("Main", "FairMedium tungsten not found");
geoBuild->createMedium(mTungsten);
TGeoMedium* pMedTungsten = gGeoMan->GetMedium("tungsten");
if ( ! pMedTungsten ) Fatal("Main", "Medium tungsten not found");
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
//TGeoVolume* MWPC    = new TGeoVolumeAssembly("MWPC");
TGeoVolume* target  = new TGeoVolumeAssembly("target");

// --------------------------------------------------------------------------
// ---------------- target --------------------------------------------------
targetH2Z /= 2.;

Double_t targetShellR = targetH2R + targetShellThicknessSide;
Double_t targetShellZ = targetH2Z + targetShellThicknessZ;

TGeoVolume *targetH2 = gGeoManager->MakeTube("targetH2", pH2, 0, targetH2R, targetH2Z);
TGeoVolume *targetShell = gGeoManager->MakeTube("targetShell", pSteel, 0, targetShellR, targetShellZ);
// --------------------------------------------------------------------------
// ----------------- MWPC ---------------------------------------------------
gasVolX /= 2.;
gasVolY /= 2.;
gasVolZ /= 2.;
TGeoVolume* gasVol = gGeoManager->MakeBox("MWPCVol", pMedCF4, gasVolX, gasVolY, gasVolZ);

TGeoVolume* MWPC = gGeoManager->MakeBox("MWPC", pMedKapton, gasVolX, gasVolY, gasVolZ + kaptonThickness);

gasX /= 2.0;
gasY /= 2.0;
gasZ /= 2.0;
TGeoVolume* gasStrip = gGeoManager->MakeBox("gasStrip", pMedCF4, gasX, gasY, gasZ);

TGeoVolume* gasPlane = gGeoManager->MakeBox("gasPlane", pMedAluminium, gasVolX, gasVolY, gasZ + aluminiumThickness);

TGeoVolume* tungstenWire = gGeoManager->MakeTube("tungstenWire", pMedTungsten, 0, wireDiameter / 2, gasY);
// --------------------------------------------------------------------------
// ---------------- ToF -----------------------------------------------------
plasticX /= 2.0;
plasticY /= 2.0;
plasticZ /= 2.0;
TGeoVolume* plastic = gGeoManager->MakeBox("plastic", pMedBC408, plasticX, plasticY, plasticZ);
// --------------------------------------------------------------------------
//------------------ STRUCTURE  ---------------------------------------------
//gasStrip->AddNode(tungstenWire, 1, new TGeoCombiTrans(0, 0, 0, f90XRotation));

Int_t gasCount = gasVolX / (2 * gasX);

Double_t gasPosX;

for(Int_t i_gas = 1; i_gas <= 2*gasCount; i_gas++)
{
  gasPosX = gasVolX - gasX * 2 * (i_gas - 1) - gasX;
  gasPlane->AddNode(gasStrip, i_gas, new TGeoCombiTrans(gasPosX, 0, 0, fZeroRotation));
}

gasVol->AddNode(gasPlane, 1, new TGeoCombiTrans(0, 0, -distBetweenXandY / 2, fZeroRotation));
gasVol->AddNode(gasPlane, 2, new TGeoCombiTrans(0, 0, distBetweenXandY / 2, f90ZRotation));

MWPC->AddNode(gasVol, 1, new TGeoCombiTrans(0, 0, 0, fZeroRotation));

beamdet->AddNode(plastic, 1, new TGeoCombiTrans(global_X, global_Y, positionToF1, fGlobalRotation));
beamdet->AddNode(plastic, 2, new TGeoCombiTrans(global_X, global_Y, positionToF2, fGlobalRotation));
beamdet->AddNode(MWPC, 1, new TGeoCombiTrans(global_X, global_Y, positionMWPC1, fGlobalRotation));
beamdet->AddNode(MWPC, 2, new TGeoCombiTrans(global_X, global_Y, positionMWPC2, fGlobalRotation));

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