// ----- BeamDet parameters -------------------------------------------------

// ----- GasCone)parameters -----------------------------------------------------

Double_t  dz=5.;      
Double_t   rmin1=0.;
Double_t  rmax1= .5;
Double_t   rmin2=0.;
Double_t rmax2=1.5;

Double_t  mylarZ=0.0002;
Double_t shell_Z=0.2;
// --------------------------------------------------------------------------
// ----SI parameters ----------------------------------------------------

Double_t SiR = 1.5;   //cm
Double_t SiZ = 0.4; 
// --------------------------------------------------------------------------




// ------ Position of detector's parts relative to zero ---------------------

Double_t positionSi = 27.15045;
Double_t positiongasCone = 24.50045;
Double_t positionshell=21.90035; 

Double_t positionmylar1=22.00045;


void create_BeamDet_geo_v3_1(Double_t p_angle=10)
{
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  //Create gloabal Rotation
  TGeoRotation *fGlobalRotation = new TGeoRotation();
  fGlobalRotation->RotateX(0.);
  fGlobalRotation->RotateY(p_angle);
  fGlobalRotation->RotateZ(0.);

 
  // Create a zero rot1ation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // Create a 90 degree rotation around Z axis
  TGeoRotation *f90ZRotation = new TGeoRotation();
  f90ZRotation->RotateX(0.);
  f90ZRotation->RotateY(0.);
  f90ZRotation->RotateZ(0.);
  // Create a 90 degree rotation around X axis
  TGeoRotation *f90XRotation = new TGeoRotation();
  f90XRotation->RotateX(0.);
  f90XRotation->RotateY(0.);
  f90XRotation->RotateZ(0.);

  TGeoManager*   gGeoMan = NULL;
// -------   Load media from media file   -----------------------------------
FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
FairGeoInterface* geoFace = geoLoad->getGeoInterface();
TString geoPath = gSystem->Getenv("VMCWORKDIR");
TString medFile = geoPath + "/geometry/N15.media.geo";
geoFace->setMediaFile(medFile);
geoFace->readMedia();
gGeoMan = gGeoManager;
// --------------------------------------------------------------------------
// -------   Geometry file name (output)   ----------------------------------
TString geoFileName = geoPath + "/geometry/N15.base.geo_1.root";
// --------------------------------------------------------------------------
// -----------------   Get and create the required media    -----------------
FairGeoMedia*   geoMedia = geoFace->getMedia();
FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

// ----- Create media for MWPC (GasCone) -----------------------------------------------
FairGeoMedium* mmylar     = geoMedia->getMedium("mylar");
if ( ! mmylar ) Fatal("Main", "FairMedium mylar not found");
geoBuild->createMedium( mmylar );
TGeoMedium* pmmylar  = gGeoMan->GetMedium("mylar");
//if ( ! pMedBC408 ) Fatal("Main", "Medium BC408 not found");
//FairGeoMedium* mCF4      = geoMedia->getMedium("CF4_CH4");
//if ( ! mCF4 ) Fatal("Main", "FairMedium CF4_CH4 not found");
//geoBuild->createMedium(mCF4);
//TGeoMedium* pMedCF4 = gGeoMan->GetMedium("CF4_CH4");
//if ( ! pMedCF4 ) Fatal("Main", "Medium CF4_CH4 not found");


FairGeoMedium* misobutan003= geoMedia->getMedium("isobutan003");
if ( ! misobutan003) Fatal("Main", "FairMedium isobutan003  not found");
geoBuild->createMedium(misobutan003);
TGeoMedium* pmisobutan003 = gGeoMan->GetMedium("isobutan003");
if ( ! pmisobutan003) Fatal("Main", "isobutan003 not found");
// --------------------------------------------------------------------------
// ----- Create media for TOF (Si) ----------------------------------------------
//FairGeoMedium* mCF4      = geoMedia->getMedium("CF4_CH4");
//if ( ! mCF4 ) Fatal("Main", "FairMedium CF4_CH4 not found");
//geoBuild->createMedium(mCF4);
//TGeoMedium* pMedCF4 = gGeoMan->GetMedium("CF4_CH4");
//if ( ! pMedCF4 ) Fatal("Main", "Medium CF4_CH4 not found");

//FairGeoMedium* mKapton      = geoMedia->getMedium("kapton");
//if ( ! mKapton ) Fatal("Main", "FairMedium kapton not found");
//geoBuild->createMedium(mKapton);
//TGeoMedium* pMedKapton = gGeoMan->GetMedium("kapton");
//if ( ! pMedKapton ) Fatal("Main", "Medium kapton not found");

//FairGeoMedium* mAluminium      = geoMedia->getMedium("aluminium");
//if ( ! mAluminium ) Fatal("Main", "FairMedium aluminium not found");
//geoBuild->createMedium(mAluminium);
//TGeoMedium* pMedAluminium = gGeoMan->GetMedium("aluminium");
//if ( ! pMedAluminium ) Fatal("Main", "Medium aluminium not found");

//FairGeoMedium* mTungsten      = geoMedia->getMedium("tungsten");
//if ( ! mTungsten ) Fatal("Main", "FairMedium tungsten not found");
//geoBuild->createMedium(mTungsten);
//TGeoMedium* pMedTungsten = gGeoMan->GetMedium("tungsten");
//if ( ! pMedTungsten ) Fatal("Main", "Medium tungsten not found");

FairGeoMedium* msilicon      = geoMedia->getMedium("silicon");
if ( ! msilicon ) Fatal("Main", "FairMedium silicon not found");
geoBuild->createMedium(msilicon);
TGeoMedium* silicon_Si = gGeoMan->GetMedium("silicon");
if ( ! silicon_Si) Fatal("Main", "Medium silicon not found");
// --------------------------------------------------------------------------
// ------ Create media for target -------------------------------------------
//FairGeoMedium* mborum11  = geoMedia->getMedium("borum11");
//if ( ! mborum11  ) Fatal("Main", "FairMedium borum11 not found");
//TgeoBuild->createMedium(mborum11);
//GeoMedium* pborum11 = gGeoMan->GetMedium("borum11");
//if ( ! pborum11  ) Fatal("Main", "Medium borum11 not found"); 

FairGeoMedium* mSteel      = geoMedia->getMedium("Steel");
if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
geoBuild->createMedium(mSteel);
TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
if ( ! pSteel ) Fatal("Main", "Medium vacuum not found");
// --------------------------------------------------------------------------

// ------ Create media for collimator -----------------------------------------------
FairGeoMedium* maluminium   = geoMedia->getMedium("aluminium");
if ( ! maluminium ) Fatal("Main", "FairMedium aluminium not found");
geoBuild->createMedium(maluminium);
TGeoMedium* pmaluminium = gGeoMan->GetMedium("aluminium");
if ( ! pmaluminium ) Fatal("Main", "Medium aluminium not found");
// --------------------------------------------------------------------------
// ------ Create vacuum media  -----------------------------------------------
FairGeoMedium* mvacuum    = geoMedia->getMedium("vacuum");
if ( ! mvacuum ) Fatal("Main", "FairMedium vacuum not found");
geoBuild->createMedium(mvacuum);
TGeoMedium* pmvacuum = gGeoMan->GetMedium("vacuum");
if ( ! pmvacuum ) Fatal("Main", "Medium vacuum not found");
// --------------------------------------------------------------------------
//------------------------- VOLUMES -----------------------------------------

// --------------   Create geometry and top volume  -------------------------
gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
gGeoMan->SetName("DETgeom");
TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
gGeoMan->SetTopVolume(top);
TGeoVolume* beamdet = new TGeoVolumeAssembly("beamdet_1");
// --------------------------------------------------------------------------
//----------------collimator ---------------------

//TGeoVolume* collimator= gGeoManager->MakeTube("collimator", pmaluminium, CoRmin,CoRmax, CoZ/2);

//TGeoVolume* D1= gGeoManager->MakeTube("D1", pmaluminium, D1Rmin,D1Rmax, D1Z/2);

//TGeoVolume* D2= gGeoManager->MakeTube("D2", pmaluminium, D2Rmin,D2Rmax, D2Z/2);
// ---------------- target --------------------------------------------------

 //targetH2Z /= 2.;
//Double_t targetShellRmin = targetB11R ;
//Double_t targetShellRmax = targetB11R + targetShellThickness;

//TGeoVolume *targetB11 = gGeoManager->MakeTube("targetB11", pborum11, 0, targetB11R, targetB11Z/2);

//TGeoVolume * targetShell = gGeoManager->MakeTube("targetShell", pSteel, targetShellRmin, targetShellRmax, targetShellZ/2);
//TGeoVolume *targetShell = gGeoManager->MakeTube("targetShell", pSteel, 0, targetShellR, targetShellZ);


// --------------------------------------------------------------------------
// ---------------- gasCone-----------------------------------------------------

 
TGeoVolume* gasCone = gGeoManager->MakeCone("gas1",pmisobutan003, dz/2, rmin1,rmax1, rmin2,rmax2);

TGeoVolume* gCone_shell = gGeoManager->MakeCone("gCone_shell",pmaluminium, dz/2, rmax1,rmax1+0.2, rmax2,rmax2+0.2);
TGeoVolume *mylar1 = gGeoManager->MakeTube("mylar1", pmmylar, 0, rmax1, mylarZ/2);
TGeoVolume *Shell_Z = gGeoManager->MakeTube("Shell_Z", pmaluminium, 0.2, rmax1+0.2, shell_Z/2);

// --------------------------------------------------------------------------

// -----------------   (Si) ---------------------------------------------------


//TGeoVolume* gasVol = gGeoManager->MakeBox("MWPCVol", pMedCF4, gasVolX, gasVolY, gasVolZ);

TGeoVolume* Si = gGeoManager->MakeTube("plastic1", silicon_Si, 0,SiR, SiZ/2);



//TGeoVolume* gas = gGeoManager->MakeBox("gas", pMedCF4, gasX, gasY, gasZ);

//TGeoVolume* gasPlane = gGeoManager->MakeBox("gasPlane", pMedCF4, gasVolX, gasVolY, gasZ + aluminiumThickness);

//TGeoVolume* tungstenWire = gGeoManager->MakeTube("tungstenWire", pMedTungsten, 0, wireDiameter / 2, gasY);
//------------------ STRUCTURE  ---------------------------------------------
//gas->AddNode(tungstenWire, 1, new TGeoCombiTrans(0, 0, 0, f90XRotation));

//Int_t gasCount = gasVolX / (2 * gasX);

//Double_t gasPosX;

//for(Int_t i_gas = 1; i_gas <= 2*gasCount; i_gas++) 
//{ gasPosX = gasVolX - gasX * 2 * (i_gas - 1) - gasX;
//gasPlane->AddNode(gas, i_gas, new TGeoCombiTrans(gasPosX, 0, 0, fZeroRotation));
//}

//gasVol->AddNode(gasPlane, 1, new TGeoCombiTrans(0, 0, -distBetweenXandY / 2, fZeroRotation));
//gasVol->AddNode(gasPlane, 2, new TGeoCombiTrans(0, 0, distBetweenXandY / 2, f90ZRotation));

//MWPC->AddNode(gasVol, 1, new TGeoCombiTrans(0, 0, 0, fZeroRotation));

//beamdet->AddNode(plastic, 1, new TGeoCombiTrans(global_X, global_Y, positionToF1, fGlobalRotation));
beamdet->AddNode(gasCone,1, new TGeoCombiTrans(global_X, global_Y, positiongasCone,  fZeroRotation));
beamdet->AddNode(gCone_shell,1, new TGeoCombiTrans(global_X, global_Y, positiongasCone,  fZeroRotation));
beamdet->AddNode(mylar1,1, new TGeoCombiTrans(global_X, global_Y, positionmylar1,  fZeroRotation));
beamdet->AddNode(Si, 1, new TGeoCombiTrans(global_X, global_Y, positionSi, fZeroRotation));
beamdet->AddNode(Shell_Z,1, new TGeoCombiTrans(global_X, global_Y, positionshell,  fZeroRotation));

//beamdet->AddNode(MWPC, 2, new TGeoCombiTrans(global_X, global_Y, positionMWPC2, fGlobalRotation));
//Collimator->AddNode(D1, 1, new TGeoCombiTrans(global_X, global_Y, positionD1, fGlobalRotation));
//Collimator ->AddNode(D1, 2, new TGeoCombiTrans(global_X, global_Y, positionD1_2, fGlobalRotation));
//Collimator->AddNode(D2, 1, new TGeoCombiTrans(global_X, global_Y, positionD2, fGlobalRotation));
//beamdet -> AddNode(collimator, 1, new TGeoCombiTrans(global_X ,global_Y, positionCollimator, fGlobalRotation));
//targetShell->AddNode(targetB11 , 1, new TGeoCombiTrans(.0,.0,.0, f90ZRotation));
//target->AddNode(targetShell, 1, new TGeoCombiTrans(.0, .0, .0, f90ZRotation));


//beamdet -> AddNode(targetShell, 1, new TGeoCombiTrans(global_X ,global_Y, positiontargetShell, fGlobalRotation));


//top->AddNode(Collimator , 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));
//top->AddNode(target , 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));
top->AddNode(beamdet , 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));

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
