void create_ND_geo_exp1904_10he_8m()
{
  TString erPath = gSystem->Getenv("VMCWORKDIR");

  // Output paths
  TString outGeoFilenameRoot = erPath + "/geometry/ND.geo.exp1904.10he.8m.root";

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
  TGeoBBox* PlaneShape = new TGeoBBox("PlaneShape",400./2., 400./2., 900./2.);
  TGeoTube* crystalSh = new TGeoTube("crystalSh", R_min_Crystal, R_max_Crystal, HeightZ_Crystal/2.);
  TGeoTube* shellSh = new TGeoTube("shellSh", R_min_Shell, R_max_Shell, HeightZ_Shell/2.);
  TGeoTube* housingSh = new TGeoTube("housingSh", R_min_Housing, R_max_Housing, HeightZ_Housing/2.);

  // Volumes
  TGeoVolume* AirBox = new TGeoVolume("NDAirBox", PlaneShape, pAir);

  TGeoVolume* crystalVol = new TGeoVolume("crystalVol", crystalSh, pStilbene);
  crystalVol->SetLineColor(kBlue);

  TGeoVolume* shellVol = new TGeoVolume("shellVol", shellSh, paluminium);
  shellVol->SetLineColor(kRed);

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

float R = 48; //mm		
float h = TMath::Sqrt(3)*R;	//высота равностороннего треугольника со стороной 2R

float xyz[45][3] = {  
	8*R,	-2*h,	7865	,	//0
	6*R,	-2*h,	7900	,	//1
	4*R,	-2*h,	7925	,	//2
	2*R,	-2*h,	7939.9	,	//3
	0.0,	-2*h,	7945	,	//4
	-2*R,	-2*h,	7939.9	,	//5
	-4*R,	-2*h,	7925	,	//6
	-6*R,	-2*h,	7900	,	//7
	-8*R,	-2*h,	7865	,	//8
	9*R,	-h,	7865	,	//9
	7*R,	-h,	7900	,	//10
	5*R,	-h,	7925	,	//11
	3*R,	-h,	7939.9	,	//12
	R,	-h,	7945	,	//13
	-R,	-h,	7939.9	,	//14
	-3*R,	-h,	7925	,	//15
	-5*R,	-h,	7900	,	//16
	-7*R,	-h,	7865	,	//17
	8*R,	0.0,	7865	,	//18
	6*R,	0.0,	7900	,	//19
	4*R,	0.0,	7925	,	//20
	2*R,	0.0,	7939.9	,	//21
	0.0,	0.0,	7945	,	//22
	-2*R,	0.0,	7939.9	,	//23
	-4*R,	0.0,	7925	,	//24
	-6*R,	0.0,	7900	,	//25
	-8*R,	0.0,	7865	,	//26
	9*R,	h,	7865	,	//27
	7*R,	h,	7900	,	//28
	5*R,	h,	7925	,	//29
	3*R,	h,	7939.9	,	//30
	R,	h,	7945	,	//31
	-R,	h,	7939.9	,	//32
	-3*R,	h,	7925	,	//33
	-5*R,	h,	7900	,	//34
	-7*R,	h,	7865	,	//35
	8*R,	2*h,	7865	,	//36
	6*R,	2*h,	7900	,	//37
	4*R,	2*h,	7925	,	//38
	2*R,	2*h,	7939.9	,	//39
	0.0,	2*h,	7945	,	//40
	-2*R,	2*h,	7939.9	,	//41
	-4*R,	2*h,	7925	,	//42
	-6*R,	2*h,	7900	,	//43
	-8*R,	2*h,	7865	,	//44
	   
    };		


    for (int i_module(0); i_module < 45; i_module++) {                                                                          
        AirBox->AddNode(moduleVol, i_module, new TGeoCombiTrans("module", xyz[i_module][0] / 10., 
                                                                          xyz[i_module][1] / 10., 
                                                                          xyz[i_module][2] / 10. - 490., rotNoRot));	//-200
    }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // World ------------------------------------
  TGeoVolumeAssembly* topVolAss = new TGeoVolumeAssembly("TOP");
  // This is the one but last level in the hierarchy
  // This volume-assembly is the only volume to be inserted into TOP
  TGeoVolumeAssembly* ND = new TGeoVolumeAssembly("ND");
  topVolAss->AddNode(ND, 1, new TGeoCombiTrans("AirBox", 0., 0., 490., rotNoRot));	//("AirBox", 0., 0., 200., rotNoRot))
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
