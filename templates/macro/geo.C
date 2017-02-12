void geo(){
	// Create a zero rotation
	TGeoRotation *fZeroRotation = new TGeoRotation();
	fZeroRotation->RotateX(0.);
	fZeroRotation->RotateY(0.);
	fZeroRotation->RotateZ(0.);

	Double_t transX = 0.;
	Double_t transY = 0.;
	Double_t transZ = 5.;

	TGeoManager*   gGeoMan = NULL;
	// -------   Load media from media file   -----------------------------------
	FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
	FairGeoInterface* geoFace = geoLoad->getGeoInterface();
	TString dir = gSystem->Getenv("VMCWORKDIR");
	TString tut_dir = dir + "/../Tutorial1";
	TString medFile = tut_dir + "/geometry/media.geo";
	geoFace->setMediaFile(medFile);
	geoFace->readMedia();
	gGeoMan = gGeoManager;
	TString geoFileName = tut_dir + "/geometry/det.geo.root";
	// --------------------------------------------------------------------------
// -----------------   Get and create the required media    -----------------
	FairGeoMedia*   geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

	FairGeoMedium* mSi      = geoMedia->getMedium("silicon");
	if ( ! mSi ) Fatal("Main", "FairMedium silicon not found");
	geoBuild->createMedium(mSi);
	TGeoMedium* pSi = gGeoMan->GetMedium("silicon");
	if ( ! pSi ) Fatal("Main", "Medium silicon not found");

	FairGeoMedium* vacuum      = geoMedia->getMedium("vacuum");
	if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
	geoBuild->createMedium(vacuum);
	TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
	if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
	// --------------------------------------------------------------------------
	// --------------   Create geometry and top volume  -------------------------
	gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
	gGeoMan->SetName("DETgeom");
	TGeoVolume* top = new TGeoVolumeAssembly("TOP");
	gGeoMan->SetTopVolume(top);
	TGeoVolume* det = new TGeoVolumeAssembly("det");
	// --------------------------------------------------------------------------

	//------------------ det station -----------------------------------------
	Double_t station_X = 6.; //cm
	Double_t station_Y = 6.;   //cm
	Double_t station_Z = 10;   //cm
	station_X /= 2.;
	station_Y /= 2.;
	station_Z /= 2.;
	TGeoVolume *station = gGeoManager->MakeBox("station", pSi, station_X, station_Y, station_Z);
	//------------------ STRUCTURE  -----------------------------------------
	det->AddNode(station, 0, new TGeoCombiTrans(.0,.0,.0, fZeroRotation));
	top->AddNode(det, 1, new TGeoCombiTrans(transX, transY, transZ, fZeroRotation));

	// ---------------   Finish   -----------------------------------------------
	gGeoMan->CloseGeometry();
	gGeoMan->CheckOverlaps(0.001);
	gGeoMan->PrintOverlaps();
	gGeoMan->Test();

	TFile* geoFile = new TFile(geoFileName, "RECREATE");
	top->Write();
	geoFile->Close();
	top->Draw();
	// --------------------------------------------------------------------------
}
