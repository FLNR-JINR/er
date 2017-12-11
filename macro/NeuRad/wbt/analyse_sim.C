void analyse_sim(TString filename="results/sim.root")
{
  TFile* inFile = new TFile(filename, "READ");
  if (inFile->IsZombie()) {
    cerr << "Could not open file '" << filename << "'. Aborting." << endl;
    return;
  }

  TTree* inTree = (TTree*)inFile->Get("cbmsim");
  if (!inTree) {
    cerr << "Could not find tree 'cbmsim'. Aborting." << endl;
    return;
  }

  TBranch* neuradPoints = inTree->GetBranch("NeuRadPoint");
  if (!neuradPoints) {
    cerr << "Could not find branch 'NeuRadPoint'. Aborting." << endl;
    return;
  }

  //UInt_t nEvents = neuradPoints->GetEntries();

  TBranch* neuradPoints_pmtId = inTree->GetBranch("NeuRadPoint.fPmtId");
  if (!neuradPoints_pmtId) {
    cerr << "Could not find branch 'NeuRadPoint.fPmtId'. Aborting." << endl;
    return;
  }

  TBranch* neuradPoints_chId = inTree->GetBranch("NeuRadPoint.fChId");
  if (!neuradPoints_chId) {
    cerr << "Could not find branch 'NeuRadPoint.fChId'. Aborting." << endl;
    return;
  }

  gStyle->SetOptStat(1111111);

  TCanvas* canv1 = new TCanvas("canv1", "Basic simulation analysis", 1600, 800);
  canv1->Divide(2);
  canv1->cd(1);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fPmtId");
  canv1->cd(2);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fChId");

  canv1->SaveAs("results/sim.png");
}
