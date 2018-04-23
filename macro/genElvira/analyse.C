void analyse()
{
	ifstream f("data.txt");

	TH1D* histohisto = new TH1D("histohisto", "histohisto", 700, 0., 14.);

	Float_t val;
	while (f >> val) {
		histohisto->Fill(val);
	}

	TCanvas* canv = new TCanvas("canv", "canv");
	canv->cd();
	histohisto->Draw();

	TFile* histoFile = new TFile("histo.root", "RECREATE");
	histohisto->Write();
	histoFile->Close();
}
