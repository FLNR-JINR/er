void drawProjections() {
// ------------------------------------------------------------------------------------------------
	auto file = TFile::Open("he8_10_0010.reco.root");
	auto tree = (TTree*)file->Get("reco");
	gStyle->SetOptStat(11);
	auto canv = new TCanvas();
	// gStyle->SetOptTitle(0);
	canv->Divide(1, 2);
	canv->cd(1);
	tree->Draw("fYt:fXt>>hist", "trigger==2&&TMath::Sqrt(fXt*fXt + fYt*fYt)<=15", "col&text");
// ------------------------------------------------------------------------------------------------
	file = TFile::Open("he8_10_0010.trigger2.reco.root");
	tree = (TTree*)file->Get("er");
	canv->cd(2);
	tree->Draw("BeamDetTrack.fYt:BeamDetTrack.fXt>>hist","","col&text");
// ------------------------------------------------------------------------------------------------	
	canv->cd(0);
	TLatex *l = new TLatex(0.45, 0.492, "Clusters: not used; target: embedded in BeamDet");
    l->SetTextSize(0.02);
    l->Draw();
    canv->SaveAs("canv_compERReco_noClusters_BeamDetTarget.png");
}
