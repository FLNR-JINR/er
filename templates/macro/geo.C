void geo() //TODO give name create_XXXXX_geo
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/XXXXX.geo.root"; //TODO give name
  TString outGeoFilenameGdml = erPath + "/geometry/XXXXX.gdml"; //TODO give name

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

  mediumName = "vacuum";
  FairGeoMedium* mvacuum = geoMedia->getMedium(mediumName);
  if (!mvacuum) Fatal("create_XXXXX_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mvacuum);
  TGeoMedium* pvacuum = geoM->GetMedium(mediumName);
  if (!pvacuum) Fatal("create_XXXXX_geo", "Medium %s not found", mediumName.Data());

//TODO create materials you need.
// You don't need to create the cave and define its material (be it air or vacuum)
// in this macro. The cave in handled by the framework.
/*
  //template - duplicate it and change the names accordingly
  mediumName = "Air";
  FairGeoMedium* mAir = geoMedia->getMedium(mediumName);
  if (!mAir) Fatal("create_XXXXX_geo", "FairMedium %s not found", mediumName.Data());
  geoBuild->createMedium(mAir);
  TGeoMedium* pAir = geoM->GetMedium(mediumName);
  if (!pAir) Fatal("create_XXXXX_geo", "Medium %s not found", mediumName.Data());
*/

  //NOTE you may use gGeoManager-Make***() methods, there is no strong reason
  // not to use them. However we think that using separate shape and volume definition
  // improves code readability and uniformity. One of the reasons is that you
  // anyway need to use separate shape and volume definitions when creating Boolean shapes.

  // Shapes
  //TODO create you shapes here
  TGeoBBox* XXXXXShape = new TGeoBBox("XXXXXShape", 10./2., 10./2., 1./2.); //TODO give name

  // Volumes
  //TODO create your volumes here
  TGeoVolume* XXXXXVol = new TGeoVolume("XXXXXVol", XXXXXShape, pvacuum); //TODO give name

  //TODO define volume hierarchy here

  // Matrices
  TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
  rotNoRot->RegisterYourself();

  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("XXXXX"); //TODO give name
  subdetectorVolAss->AddNode(XXXXXVol, 1,
    new TGeoCombiTrans("mXXXXXInSubdet", 0., 0., 0., rotNoRot)); //TODO give name

  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  topVolAss->AddNode(subdetectorVolAss, 1);

  // Finalize
  geoM->SetTopVolume(topVolAss);
  geoM->CloseGeometry();
  geoM->CheckOverlaps();
  geoM->PrintOverlaps();
  //TODO you may want to uncomment this if you want to perform additional checks
  //geoM->CheckGeometry();
  //geoM->CheckGeometryFull();
  //geoM->Test();

  // Export
  //TODO you may want to uncomment this if you need an output GDML file
  //geoM->Export(outGeoFilenameGdml);
  TFile* outGeoFileRoot = new TFile(outGeoFilenameRoot, "RECREATE");
  geoM->GetTopVolume()->Write();
  outGeoFileRoot->Close();

  // Draw
  TBrowser* bro = new TBrowser("bro", "bro");
  geoM->GetTopVolume()->Draw("ogl");
}
