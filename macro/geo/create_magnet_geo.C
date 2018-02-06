//simple magnet geo,etry for testing: Steel vauum box in steel box

void create_magnet_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/magnet.geo.root";

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
  if (!mSteel) Fatal("create_magnet_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = geoM->GetMedium(mediumName);
  if (!pSteel) Fatal("create_magnet_geo", "Medium %s not found", mediumName.Data());

  mediumName = "vacuum";
  FairGeoMedium* mVacuum = geoMedia->getMedium(mediumName);
  if (!mSteel) Fatal("create_magnet_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mVacuum);
  TGeoMedium* pVacuum = geoM->GetMedium(mediumName);
  if (!pVacuum) Fatal("create_magnet_geo", "Medium %s not found", mediumName.Data());

  // General dimensions
  Double_t extBoxSizeXY = 100.0; // cm
  Double_t intBoxSizeXY = 80.0; // cm
  Double_t thin = 20.; //cm

  // Shapes
  TGeoBBox* extBoxSh = new TGeoBBox("extBoxSh", extBoxSizeXY/2., extBoxSizeXY/2., thin/2.);
  TGeoBBox* intBoxSh = new TGeoBBox("intBoxsh", intBoxSizeXY/2., intBoxSizeXY/2., thin/2.);

  // Volumes
  TGeoVolume* extBoxVol = new TGeoVolume("extBoxVol", extBoxSh, pSteel);
  TGeoVolume* intBoxVol = new TGeoVolume("intBoxVol", intBoxSh, pVacuum);

  // Matrices
  // TGeoRotation* rot = new TGeoRotation("rot", 0., 45., 0.);
  // rot->RegisterYourself();
  TGeoRotation* rot = new TGeoRotation();
  rot->RotateY(0.);
  // Structure
  // intBoxVol in extBoxVol
  extBoxVol->AddNode(intBoxVol, 1);

  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("magnet");
  subdetectorVolAss->AddNode(extBoxVol, 1);
  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  topVolAss->AddNode(subdetectorVolAss, 1,new TGeoCombiTrans("mIntBoxInExtBox", 0., 0., 70., rot));

  // Finalize
  geoM->SetTopVolume(topVolAss);
  geoM->CloseGeometry();
  geoM->CheckOverlaps();
  geoM->PrintOverlaps();
  //geoM->CheckGeometry();
  //geoM->CheckGeometryFull();
  //geoM->Test();

  // Export
  TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
  geoM->GetTopVolume()->Write();
  outGeoFileRoot->Close();

  // Draw
  TBrowser* bro = new TBrowser("bro", "bro");
  geoM->GetTopVolume()->Draw("ogl");
}
