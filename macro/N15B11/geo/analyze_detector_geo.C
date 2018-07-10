struct stcNodeSig {
	TString iLayer;
	TString iDetContainer;
	TString iDetGasPart;
	TString iSemi;
};

static struct stcNodeSig mCurNodeSig;

void RecursiveFunc(TGeoNode* p_node)
{
	static unsigned int depthCounter = 0;
	depthCounter++;

	TString tabstr("");
	for (unsigned int i=0; i<depthCounter-1; i++) {
		tabstr += "\t";
	}

	unsigned int nDaughters = p_node->GetNdaughters();
	TString curNodeName = p_node->GetName();
	cout << tabstr << depthCounter << ": " << curNodeName << ": " << nDaughters << " daughters." << endl;

	TString fullPath("");

	if (curNodeName.Contains("vOneLayer")) {
		mCurNodeSig.iLayer = curNodeName;
	}
	if (curNodeName.Contains("vDetContainer")) {
		mCurNodeSig.iDetContainer = curNodeName;
	}
	if (curNodeName.Contains("vDetGasPart")) {
		mCurNodeSig.iDetGasPart = curNodeName;
		fullPath.Form("/cave_1/%s/%s/%s", mCurNodeSig.iLayer.Data(), mCurNodeSig.iDetContainer.Data(), mCurNodeSig.iDetGasPart.Data());
		cout << fullPath << endl;
	}
	if (curNodeName.Contains("vSemi")) {
		mCurNodeSig.iSemi = curNodeName;
		fullPath.Form("/cave_1/%s/%s/%s", mCurNodeSig.iLayer.Data(), mCurNodeSig.iDetContainer.Data(), mCurNodeSig.iSemi.Data());
		cout << fullPath << endl;
	}

	for (unsigned int iDaughter=0; iDaughter<nDaughters; iDaughter++) {
		TGeoNode* curDaughter = p_node->GetDaughter(iDaughter);
		RecursiveFunc(curDaughter);
	}

	depthCounter--;
}

void analyze_detector_geo(TString inFilename = "../../../geometry/N15B11_detector.geo.root")
{
	TGeoManager* geom = new TGeoManager("geom", "geom");

	TFile* f = new TFile(inFilename, "READ");

	//TODO check that file has been opened

	TGeoVolumeAssembly* topVol = (TGeoVolumeAssembly*)f->Get("TOP");

	geom->SetTopVolume(topVol);
	geom->CloseGeometry();

	TGeoNode* topNode = geom->GetTopNode();

	RecursiveFunc(topNode);
}
