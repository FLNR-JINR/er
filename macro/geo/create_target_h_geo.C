void create_target_h_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/target.h.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/target.h.gdml";

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

  mediumName = "Steel";
  FairGeoMedium* mSteel = geoMedia->getMedium(mediumName);
  if (!mSteel) Fatal("create_target_h_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = geoM->GetMedium(mediumName);
  if (!pSteel) Fatal("create_target_h_geo", "Medium %s not found", mediumName.Data());

  mediumName = "H2";
  FairGeoMedium* mH2 = geoMedia->getMedium(mediumName);
  if (!mH2) Fatal("create_target_h_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mH2);
  TGeoMedium* pH2 = geoM->GetMedium(mediumName);
  if (!pH2) Fatal("create_target_h_geo", "Medium %s not found", mediumName.Data());

  // General dimensions
  Double_t transX = 0.; // cm
  Double_t transY = 0.; // cm
  Double_t transZ = 0.; // cm
  Double_t targetH2R = 2.; // cm
  Double_t targetH2Z = 0.4; // cm
  Double_t shellOuterThickness = 1.5*1e-4; // cm default=[1.5*1e-4]
  Double_t shellZThickness = 6.0*1e-4; // cm default=[6.0*1e-4]

  // Shapes
  TGeoTube* targetH2Shape = new TGeoTube("targetH2Shape",
    0.,
    targetH2R,
    targetH2Z/2.);
  TGeoTube* targetShellShape = new TGeoTube("targetShellShape",
    0.,
    targetH2R + shellOuterThickness,
    (targetH2Z + 2*shellZThickness)/2.);

  // Volumes
  TGeoVolume* targetH2Vol = new TGeoVolume("targetH2Vol", targetH2Shape, pH2);
  TGeoVolume* targetShellVol = new TGeoVolume("targetShellVol", targetShellShape, pSteel);

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // Structure
  targetShellVol->AddNode(targetH2Vol, 1);

  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("target_h");
  subdetectorVolAss->AddNode(targetShellVol, 1,
    new TGeoCombiTrans("mTargetH2VolInTarget", transX, transY, transZ, rotNoRot));

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
  geoM->GetTopVolume()->Draw("ogl");
}
