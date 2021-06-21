void create_ND_geo_exp1904()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/ND.geo.exp1904.root";

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

  mediumName = "Air";
  FairGeoMedium* mAir = geoMedia->getMedium(mediumName);
  if (!mAir) Fatal("create_ND_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mAir);
  TGeoMedium* pAir = geoM->GetMedium(mediumName);
  if (!pAir) Fatal("create_ND_geo", "Medium %s not found", mediumName.Data());
  
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
  TGeoBBox* PlaneShape = new TGeoBBox("PlaneShape",200./2., 200./2., 100./2.);
  TGeoTube* crystalSh = new TGeoTube("crystalSh", R_min_Crystal, R_max_Crystal, HeightZ_Crystal/2.);
  TGeoTube* shellSh = new TGeoTube("shellSh", R_min_Shell, R_max_Shell, HeightZ_Shell/2.);
  TGeoTube* housingSh = new TGeoTube("housingSh", R_min_Housing, R_max_Housing, HeightZ_Housing/2.);

  // Volumes
  TGeoVolume* AirBox = new TGeoVolume("NDAirBox", PlaneShape, pAir);

  TGeoVolume* crystalVol = new TGeoVolume("crystalVol", crystalSh, pStilbene);
  crystalVol->SetLineColor(kBlue);
  crystalVol->SetTransparency(10);

  TGeoVolume* shellVol = new TGeoVolume("shellVol", shellSh, paluminium);
  shellVol->SetLineColor(kRed);
  //shellVol->SetTransparency(40);

  TGeoVolume* housingVol = new TGeoVolume("housingVol", housingSh, pSteel);
  housingVol->SetLineColor(kGray);
  housingVol->SetTransparency(60);

  TGeoVolume* moduleVol = new TGeoVolumeAssembly("moduleVol");
  moduleVol->SetLineColor(kOrange);
  moduleVol->SetTransparency(70);
  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // Structure
  // Crystal in shell
  shellVol->AddNode(crystalVol, 1, new TGeoCombiTrans("mCrystalInShell", 0., 0., (HeightZ_Shell-HeightZ_Crystal)/2., rotNoRot));

  // Shell in housing
  housingVol->AddNode(shellVol, 1, new TGeoCombiTrans("mCrystalInShell", 0., 0., (HeightZ_Shell-HeightZ_Crystal)/2., rotNoRot));
  // housing in Module
  moduleVol->AddNode(housingVol, 1, new TGeoCombiTrans("mCrystalInShell", 0., 0., (HeightZ_Shell-HeightZ_Crystal)/2., rotNoRot));

  float xyz[45][3] = {  
    450., -300.,  1900.,
    300., -300.,  1925.,
    150., -300.,  1939.9,
    0.0,  -300.,  1945.,
    -150.,  -300.,  1939.9,
    -300.,  -300.,  1925.,
    -450.,  -300.,  1900.,
    450., -150.,  1900.,
    300., -150.,  1925.,
    150., -150.,  1939.9,
    0.0,  -150.,  1945.,
    -150.,  -150.,  1939.9,
    -300.,  -150.,  1925.,
    -450.,  -150.,  1900.,
    450., 0.0,  1900.,
    300., 0.0,  1925.,
    150., 0.0,  1939.9,
    -100.,  0.0,  1943.,
    -250.,  0.0,  1931.1,
    -400.,  0.0,  1909.4,
    -550.,  0.0,  1877.8,
    450., 150., 1900.,
    300., 150., 1925.,
    150., 150., 1939.9,
    0.0,  150., 1945.,
    -150.,  150., 1939.9,
    -300.,  150., 1925.,
    -450.,  150., 1900.,
    450., 300., 1900.,
    300., 300., 1925.,
    150., 300., 1939.9,
    0.0,  300., 1945.,
    -150.,  300., 1939.9,
    -300.,  300., 1925.,
    -450.,  300., 1900.,
    600., 300., 1865.,
    600., 150., 1865.,
    750., 0.0,  1820.2,
    600., 0.0,  1865.,
    600., -150.,  1865.,
    600., -300.,  1865.,
    -600.,  300., 1865.,
    -600.,  150., 1865.,
    -600.,  -150.,  1865.,
    -600.,  -300.,  1865.
    };

    for (int i_module(0); i_module < 45; i_module++) {
        AirBox->AddNode(moduleVol, i_module, new TGeoCombiTrans("module", xyz[i_module][0] / 10.,
                                                                          xyz[i_module][1] / 10.,
                                                                          xyz[i_module][2] / 10., rotNoRot));
    }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* ND = new TGeoVolumeAssembly("ND");
  topVolAss->AddNode(ND, 1, new TGeoCombiTrans("AirBox", 0., 0., 200., rotNoRot));
  ND->AddNode(AirBox,1);
  
  // Finalize
  geoM->SetTopVolume(topVolAss);
  geoM->CloseGeometry();
  geoM->CheckOverlaps();
  geoM->PrintOverlaps();
  geoM->CheckGeometry();
  geoM->CheckGeometryFull();
  geoM->Test();

  // Export
  TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
  geoM->GetTopVolume()->Write();
  outGeoFileRoot->Close();

  // Draw
  TBrowser* bro = new TBrowser("bro", "bro");
  geoM->GetTopVolume()->Draw("ogl");
}
