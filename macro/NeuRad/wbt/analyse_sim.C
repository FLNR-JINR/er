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

  TCanvas* canv1 = new TCanvas("canv1", "Basic simulation analysis 1", 1600, 800);
  canv1->Divide(2);
  canv1->cd(1);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fPmtId");
  canv1->cd(2);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fChId");

  TCanvas* canv2 = new TCanvas("canv2", "Basic simulation analysis 2", 1600, 800);
  canv2->Divide(2, 2);
  canv2->cd(1);
  //gPad->SetGrid(1, 1);
  inTree->Draw("NeuRadPoint.fELoss","TMath::Abs(NeuRadPoint.fELoss)<0.001","",1000,0);
  TH1F* htemp = (TH1F*)gPad->GetPrimitive("htemp");
  htemp->SetTitle("fELoss");
  ////inTree->Draw("fELoss");

  canv2->cd(2);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fLightYield");
  canv2->cd(3);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fTimeIn");
  canv2->cd(4);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fTimeOut");

  canv1->SaveAs("results/sim1.png");
  canv2->SaveAs("results/sim2.png");
}
