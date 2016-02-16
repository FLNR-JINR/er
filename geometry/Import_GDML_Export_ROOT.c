void Import_GDML_Export_ROOT()
{
	TString vmcDir = TString(gSystem->Getenv("VMCWORKDIR"));

	TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");

	TGDMLParse parser;
	// Define your input GDML file HERE
	TGeoVolume* gdmlTop = parser.GDMLReadFile ("gadast.gdml");
	TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");

	gGeoManager->SetTopVolume(gdmlTop);

  
	// Define your position HERE
	TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
	TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 0., rot);

	rootTop->AddNode (gdmlTop, 1, posrot);

	gGeoManager->CloseGeometry();

	// Define you output ROOT file HERE
	TFile* outfile = new TFile(vmcDir + "/geometry/gadast.root", "RECREATE");
	rootTop->Write();
	outfile->Close();
  
}

