void create_ND_geo()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/ND.geo.root";
  TString outGeoFilenameGdml = erPath + "/geometry/ND.gdml";

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
  TGeoVolumeAssembly* subdetectorVolAss = new TGeoVolumeAssembly("neutrondet1ch");
  //subdetectorVolAss->AddNode(moduleVol, 1);

  //  for (UInt_t iX=0; iX<iXmax; iX++) {
  //    for (UInt_t iY=0; iY<iYmax; iY++) {
  //      matrixName.Form("mHousingInND_%d", iX*iYmax+iY);
  //      moduleVol->AddNode(submoduleVol, iX*2+iY, new TGeoCombiTrans(matrixName,
  // Modules' Positioning Here 
  //          -submoduleXsize/2.+submoduleXsize*iX, -submoduleYsize/2.+submoduleYsize*iY, 0., rotNoRot));
  //  Modules' Positioning Here 
  //    }
  //  }
  // Regular positioning below

  const Double_t r = 100;        // Sphere's radius
  const Double_t l = 2*R_max_Housing - 0.1;        // Length of the arc between two cylinders
  Double_t alpha[3];        // Angle of rotation of cylinders around axes:  alpha = l/r
  Double_t h[3];          // h = sqrt[4*r*r*sin(alpha/2)*sin(alpha/2) - r*r*sin(alpha)*sin(alpha)]
                // h - defines the location of the cylinder on the Z-axis
  Double_t l1[3];         // span: l1=r*sin(alpha)
  Double_t pi = 3.141592653589793;
  //TMath::Pi();
  Double_t rad = 180./pi;       // conversion of rads to degrees

  for(Int_t i=0; i<3; i++){
  for(Int_t j=0; j<3; j++){
  //1. Central cylinder:
  if((i==0)&&(j==0)){
  subdetectorVolAss->AddNode(moduleVol, 26, new TGeoCombiTrans("r1",0,0,r,new TGeoRotation("r2",0,0,0)));
  }
  //2. Cylinders along the Y-axis:
  else if((i==0)&&!(j==0)){
  alpha[j]= j*(l/r);
  l1[j] =r*sin(alpha[j]);
  h[j] = sqrt(4*r*r*sin(alpha[j]/2)*sin(alpha[j]/2) - r*r*sin(alpha[j])*sin(alpha[j]));
  subdetectorVolAss->AddNode(moduleVol,2*j, new TGeoCombiTrans("r1",0,l1[j], r-h[j], new TGeoRotation("r2",0, -(alpha[j]*rad),0)));
  subdetectorVolAss->AddNode(moduleVol,2*j+1, new TGeoCombiTrans("r1",0, -l1[j], r-h[j], new TGeoRotation("r2",0, (alpha[j]*rad),0)));
  //3. Cylinders along the X-axis:
  }else if((j==0)&&!(i==0)){
  alpha[i]= i*(l/r);
  l1[i] =r*sin(alpha[i]);
  h[i] = sqrt(4*r*r*sin(alpha[i]/2)*sin(alpha[i]/2) - r*r*sin(alpha[i])*sin(alpha[i]));
  subdetectorVolAss->AddNode(moduleVol,2*i+4, new TGeoCombiTrans("r1",l1[i],0, r-h[i], new TGeoRotation("r2",0, 0,-(alpha[i]*rad))));
  subdetectorVolAss->AddNode(moduleVol,2*i+5, new TGeoCombiTrans("r1",-l1[i],0, r-h[i], new TGeoRotation("r2",0,0, (alpha[i]*rad))));
  //4. Cylinders placed on quadrants:
  }
  else {
  subdetectorVolAss->AddNode(moduleVol,8*i+2*j, 
  new TGeoCombiTrans("r1",l1[i],l1[j], r-h[j]-h[i], 
  new TGeoRotation("r2",0, -(alpha[j]*rad),-(alpha[i]*rad))));
  subdetectorVolAss->AddNode(moduleVol,8*i+2*j+1, 
  new TGeoCombiTrans("r1",l1[i], -l1[j], r-h[j]-h[i], 
  new TGeoRotation("r2",0,(alpha[j]*rad), -(alpha[i]*rad))));
  subdetectorVolAss->AddNode(moduleVol,8*i+2*j+4, 
  new TGeoCombiTrans("r1",-l1[i],-l1[j], r-h[j]-h[i], 
  new TGeoRotation("r2",0, (alpha[j]*rad),(alpha[i]*rad))));
  subdetectorVolAss->AddNode(moduleVol,8*i+2*j+5, 
  new TGeoCombiTrans("r1",-l1[i], l1[j], r-h[j]-h[i], 
  new TGeoRotation("r2",0, -(alpha[j]*rad),(alpha[i]*rad))));
  }//else
  }//for j
  }//for i


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
