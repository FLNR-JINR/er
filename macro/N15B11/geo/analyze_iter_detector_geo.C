void analyze_iter_detector_geo(TString inFilename = "../../../geometry/N15B11_detector.geo.root")
{
	TGeoManager* geom = new TGeoManager("geom", "geom");

	TFile* f = new TFile(inFilename, "READ");

	//TODO check that file has been opened

	TGeoVolumeAssembly* topVol = (TGeoVolumeAssembly*)f->Get("TOP");

	geom->SetTopVolume(topVol);
	geom->CloseGeometry();


	TGeoIterator next(topVol);

	TGeoNode *node;
	while ((node=next())) {
		TString curPath;
		next.GetPath(curPath);
		cout << curPath << endl;
	}
}
