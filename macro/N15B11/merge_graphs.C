void merge_graphs()
{
	TString pwd(gSystem->pwd());
	TString workDir = pwd + "/output_parallel";

	TSystemDirectory dire(workDir, workDir);
	TList *files = dire.GetListOfFiles();

	TMultiGraph* mg = new TMultiGraph();

	TIter next(files);
	while (TSystemFile* curFile = (TSystemFile*)next())
	{
		TString curFileName = curFile->GetName();

		if (curFileName.Contains("graph") && curFileName.Contains(".root")) {
			TString fullPath = workDir + "/" + curFile->GetName();
			cout << fullPath << endl;

			TFile f(fullPath, "READ");

			if (f.IsZombie()) {
				cerr << "Error opening file '" << fullPath << "'" << endl;
				return;
			}

			TString graphName("Graph");
			TGraph* g = (TGraph*)f.Get(graphName);
			if (g == NULL) {
				cerr << "Graph '" << graphName << "' not found." << endl;
				return;
			} else {
				mg->Add(g);
			}
			f.Close();

		}

	}

	TCanvas* canv = new TCanvas("canv", "canv");
	canv->cd();
	mg->Draw("AP");
	mg->SetTitle("Gas Eloss vs. Semi Eloss; GeV; GeV");


/////////////////////////////////////////////////////////////////
	mg->GetXaxis()->SetLimits(0., 0.050); // Up to 50 MeV
	mg->SetMinimum(0.);
	mg->SetMaximum(0.010); // Up to 10 MeV;
/////////////////////////////////////////////////////////////////

	gPad->Modified();
}
