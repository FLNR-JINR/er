void create_sensPlane_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/sensPlane.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/sensPlane.gdml";

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
  if (!mAir) Fatal("create_sensPlane_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mAir);
  TGeoMedium* pAir = geoM->GetMedium(mediumName);
  if (!pAir) Fatal("create_sensPlane_geo", "Medium %s not found", mediumName.Data());
  
  // General dimensions
  Double_t xyz_size = 500.; // Full size, not half
  Double_t thickness = 0.1; // Full size, not half

  // Shapes
  TGeoBBox* sensPlaneShape = new TGeoBBox("sensPlaneShape", xyz_size/2., xyz_size/2., thickness/2.);

  // Volumes
  TGeoVolume* sensPlaneVol = new TGeoVolume("sensPlaneVol", sensPlaneShape, pAir);

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("sensPlane");

  //TODO note the position of the first (or only) station!
  subdetectorVolAss->AddNode(sensPlaneVol, 1,
    new TGeoCombiTrans("mSt1InSubdet", 0., 0., 10., rotNoRot));
/*
  subdetectorVolAss->AddNode(sensPlaneVol, 2,
    new TGeoCombiTrans("mSt2InSubdet", 0., 0., 10., rotNoRot));
  subdetectorVolAss->AddNode(sensPlaneVol, 3,
    new TGeoCombiTrans("mSt3InSubdet", 0., 0., 20., rotNoRot));
  subdetectorVolAss->AddNode(sensPlaneVol, 4,
    new TGeoCombiTrans("mSt4InSubdet", 0., 0., 30., rotNoRot));
*/

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
