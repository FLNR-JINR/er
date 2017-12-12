Double_t SPEfunc(Double_t *x, Double_t *par) {
  Double_t fitval;
  if (x[0]<63) {
    fitval = 0;
  }
  if (x[0]>=63 && x[0]<par[0]) {
    fitval = (x[0]-63) * (par[1]) / (par[0]-63) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[0]) {
    fitval = par[1]*(x[0]-par[2])*(x[0]+par[2]-2*par[3])/((par[0]-par[2])*(par[0]+par[2]-2*par[3])) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[2]) {
    fitval = par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  return fitval;
}

void analyse_digi(TString filename="results/digi.root")
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

  TBranch* neuradPhotoElectrons = inTree->GetBranch("NeuRadPhotoElectron");
  if (!neuradPhotoElectrons) {
    cerr << "Could not find branch 'NeuRadPhotoElectron'. Aborting." << endl;
    return;
  }

  TBranch* neuRadPixelSignals = inTree->GetBranch("NeuRadPixelSignal");
  if (!neuRadPixelSignals) {
    cerr << "Could not find branch 'NeuRadPixelSignal'. Aborting." << endl;
    return;
  }

  gStyle->SetOptStat(1111111);

  TCanvas* canv1 = new TCanvas("canv1", "Basic digitization analysis 1", 1600, 800);
  canv1->Divide(2, 2);
  canv1->cd(1);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fIndex");
  canv1->cd(2);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fSide");
  TH1F* htemp = (TH1F*)gPad->GetPrimitive("htemp");
  htemp->SetMinimum(0.);
  canv1->cd(3);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fPhotonCount");
  canv1->cd(4);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fAmplitude");

  TF1* fSPEfunc = new TF1("fSPEfunc", "SPEfunc", 0., 2000., 7);
  fSPEfunc->SetParameters(85.8656, 30.6158, 447.112, 447.111, 52., 433., 141.);
  fSPEfunc->Draw("sames][");

  TCanvas* canv2 = new TCanvas("canv2", "Basic digitization analysis 2", 1600, 800);
  canv2->Divide(2, 2);
  canv2->cd(1);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fLYTime");
  canv2->cd(2);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fCathodeTime");
  canv2->cd(3);
  //gPad->SetGrid(1, 1);
  inTree->Draw("fAnodeTime");

  canv1->SaveAs("results/digi1.png");
  canv2->SaveAs("results/digi2.png");


  
}
