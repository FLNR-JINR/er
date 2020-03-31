void create_ND_geo_v2()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/ND2ch.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/ND2ch.gdml";

  // Input paths
  TString medFile = erPath + "/geometry/media.geo";

  // Materials and media
  FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // Geometry manager
  TGeoManager* geoM = (TGeoManager*)gROOT->FindObject("FAIRGeom");

  TString mediumName;

  mediumName = "Stilbene";
  FairGeoMedium* mStilbene = geoMedia->getMedium(mediumName);
  if (!mStilbene) Fatal("create_ND_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mStilbene);
  TGeoMedium* pStilbene = geoM->GetMedium(mediumName);
  if (!pStilbene) Fatal("create_ND_geo", "Medium %s not found", mediumName.Data());

  mediumName = "Steel";
  FairGeoMedium* mSteel = geoMedia->getMedium(mediumName);
  if (!mSteel) Fatal("create_ND_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = geoM->GetMedium(mediumName);
  if (!pSteel) Fatal("create_ND_geo", "Medium %s not found", mediumName.Data());

  mediumName = "aluminium";
  FairGeoMedium* maluminium = geoMedia->getMedium(mediumName);
  if (!maluminium) Fatal("create_ND_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(maluminium);
  TGeoMedium* paluminium = geoM->GetMedium(mediumName);
  if (!paluminium) Fatal("create_ND_geo", "Medium %s not found", mediumName.Data());

  // General dimensions
  Double_t R_min_Crystal = 0.0; // cm
  Double_t R_max_Crystal = 4.0; // cm
  Double_t HeightZ_Crystal = 5.0; // cm
  Double_t R_min_Shell = 0.0; // cm
  Double_t R_max_Shell = 4.2; // cm
  Double_t HeightZ_Shell = 5.2; // cm
  Double_t R_min_Housing = 0.0; // cm
  Double_t R_max_Housing = 4.8; // cm
  Double_t HeightZ_Housing = 5.4; // cm

  // Shapes
  TGeoTube* crystalSh = new TGeoTube("crystalSh", R_min_Crystal, R_max_Crystal, HeightZ_Crystal/2.);
  TGeoTube* shellSh = new TGeoTube("shellSh", R_min_Shell, R_max_Shell, HeightZ_Shell/2.);
  TGeoTube* housingSh = new TGeoTube("housingSh", R_min_Housing, R_max_Housing, HeightZ_Housing/2.);

  // Volumes
  TGeoVolume* crystalVol = new TGeoVolume("crystalVol", crystalSh, pStilbene);
  TGeoVolume* shellVol = new TGeoVolume("shellVol", shellSh, paluminium);
  TGeoVolume* housingVol = new TGeoVolume("housingVol", housingSh, pSteel);
  TGeoVolume* moduleVol = new TGeoVolumeAssembly("moduleVol");

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // Structure
  // Crystal in shell
  shellVol->AddNode(crystalVol, 1, new TGeoCombiTrans("mCrystalInShell", 0., 0., (HeightZ_Shell-HeightZ_Crystal)/2., rotNoRot));

  // Shell in housing
  housingVol->AddNode(shellVol, 1);
  // housing in Module
  moduleVol->AddNode(housingVol, 1);

  // Module in ND
  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("neutrondet2ch");

  Float_t axesDist = 20.;

  subdetectorVolAss->AddNode(moduleVol, 1, new TGeoCombiTrans("moduleInNeutrondet2ch", axesDist/2., 0., HeightZ_Housing/2., rotNoRot));
  subdetectorVolAss->AddNode(moduleVol, 2, new TGeoCombiTrans("moduleInNeutrondet2ch", -axesDist/2., 0., HeightZ_Housing/2., rotNoRot));


  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  topVolAss->AddNode(subdetectorVolAss, 1);

  // Finalize
  geoM->SetTopVolume(topVolAss);
  geoM->CloseGeometry();
  geoM->CheckOverlaps();
  geoM->PrintOverlaps();
  //geoM->CheckGeometry();
  //geoM->CheckGeometryFull();
  //geoM->Test();

  // Export
  //geoM->Export(outGeoFilenameGdml);
  TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
  geoM->GetTopVolume()->Write();
  outGeoFileRoot->Close();

  // Draw
  TBrowser* bro = new TBrowser("bro", "bro");
  geoM->GetTopVolume()->Draw();
}
