void analyse(Int_t index=0, TString dirName="output", TString fileNamePrefix="sim_")
{
	TString fileNameSuffix = ".root";

	TString simFilename;
	simFilename.Form("%s/%s%d%s", dirName.Data(), fileNamePrefix.Data(), index, fileNameSuffix.Data());

	TFile* inFile = new TFile(simFilename, "READ");

	if (inFile->IsZombie()) {
		cerr << "Could not open file '" << simFilename << "'" << endl;
		return;
	} else {
		cerr << "Succesfully opene input file '" << simFilename << "'" << endl;
	}


	TTree* inTree = (TTree*)inFile->Get("er");

	if (inTree == NULL) {
		cerr << "Could not open tree 'er' in the input file '" << simFilename << "'" << endl;
		return;
	} else {
		cerr << "Found tree 'er' in the input file '" << simFilename << "'" << endl;
	}

	TCanvas* canv1 = new TCanvas("canv1", "canv1");
	canv1->cd();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inTree->Draw("Gas_Layer_0_Det_12_Point.fELoss:Semi_Layer_0_Det_12_Point.fELoss", "");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	canv1->Close();

	TGraph* gr = new TGraph(inTree->GetSelectedRows(), inTree->GetV2(), inTree->GetV1());

	cerr << gr->GetN() << " points in the output graph" << endl;

	TCanvas* canv2 = new TCanvas("canv2", "canv2");
	canv2->cd();
	gr->Draw("AP");
	gr->SetMarkerStyle(7);

	//TODO close the input file?
	//inFile->Close();

	// Write the output file ========================================================

	TString outputFilename;
	outputFilename.Form("%s/graph_%d.root", dirName.Data(), index);

	TFile* outputFile = new TFile(outputFilename, "RECREATE");
	gr->Write();
	outputFile->Close();
}
