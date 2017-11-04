void create_target_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/target.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/target.gdml";

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

  mediumName = "beryllium";
  FairGeoMedium* mberyllium = geoMedia->getMedium(mediumName);
  if (!mberyllium) Fatal("create_target_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mberyllium);
  TGeoMedium* pberyllium = geoM->GetMedium(mediumName);
  if (!pberyllium) Fatal("create_target_geo", "Medium %s not found", mediumName.Data());

  // General dimensions
  Double_t transX = 0.; // cm
  Double_t transY = 0.; // cm
  Double_t transZ = 1.; // cm
  Double_t target_R = 2.; // cm
  Double_t target_Z = 2.; // cm
  
  // Shapes
  TGeoTube* targetShape = new TGeoTube("targetShape", 0., target_R, target_Z/2.);

  // Volumes
  TGeoVolume* targetVol = new TGeoVolume("targetVol", targetShape, pberyllium);

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("target");
  subdetectorVolAss->AddNode(targetVol, 1,
    new TGeoCombiTrans("mTargetVolInTarget", transX, transY, transZ, rotNoRot));

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
