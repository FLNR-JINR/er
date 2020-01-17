void fileAnalysis2() {
  //auto file = TFile::Open("expCalib_reco_quad_mean.root");
  //auto file = TFile::Open("expCalib_reco_quad_mean_half_a_min.root");
  auto file = TFile::Open("expCalib_reco_quad_mean_half_a.root");
  //auto file = TFile::Open("expCalib_reco_quad_mean.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_0Fictious.";
  TString xCoord = trackBr + "fTelescopeLocalX";
  TString yCoord = trackBr + "fTelescopeLocalY";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_0Fictious_1000020040.";
  TString lvLeave = "fT";
  //TString lvLeave2 = "fT_noCorrections";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  auto saveFile = TFile::Open("expCalib_reco_hists.root", "RECREATE");
  double delta = 0.1;
  double thickStartLoc = 2.8125;
  double stepThick = -(2.8125 - 2.4375);
  double a = 0.0167078 *1e-3;
  //double a = 0.0170281*1e-3;

  //double b = -0.386704*1e-3;
  double b = -0.395058*1e-3;
  
  //double b = -0.290404*1e-3;
  double stepX = stepThick;
  double startCoordX = thickStartLoc;
  TString histParams = "(512, 0, 10.24e-3)";
  vector<TH1D*> hists;
  vector<TCanvas*> canvases;
  TH2D* hist2d = new TH2D("reco", "reco", 512, 0, 10.24e-3, 16, 0, 16);
  for (int i = 0; i < 16; i++) {
    TString canvName;
    canvName.Form("canvas_X%d", i);
    auto curCanv = new TCanvas(canvName);
    curCanv->Divide(4, 4);
    double currCoordX = startCoordX + i*stepX;
    TString halfbin;
    //halfbin.Form("(%lf+%lf*512)/512", a/2, b/2);
    halfbin.Form("%lf", a/2);
    cout << currCoordX << endl;
    TString histName, histNameStripX, histNameStripY;
    TString cond, condX, condY;
    condX.Form("%s>%lf&&%s<%lf", xCoord.Data(), currCoordX - delta,
                                 xCoord.Data(), currCoordX + delta);
    histNameStripX.Form("strip_X-%d", i);
    auto histStripX = new TH1D(histNameStripX, histNameStripX, 512, b, 512.*a + b);
    histStripX->GetYaxis()->SetTitle("Counts, [1]");
    histStripX->GetXaxis()->SetTitle("E, [GeV]");
    //tree->Draw(partBr+lvLeave+"-"+partBr+lvLeave2 + "+" + halfbin + ">>" + histNameStripX, condX);
    tree->Draw(partBr+lvLeave+"-"+partBr + "+" + halfbin + ">>" + histNameStripX, condX);
    //tree->Draw(partBr+lvLeave+"-"+partBr+lvLeave2 + ">>" + histNameStripX, condX);
    cout << "histNameStripX " << histNameStripX << endl;
    auto histTmp = (TH1D*)gDirectory->Get(histNameStripX);
    for (int iBin = 1; iBin <= histTmp->GetXaxis()->GetNbins(); iBin++) {
      int binContent = histTmp->GetBinContent(iBin);
      hist2d->SetBinContent(iBin, i + 1, binContent);
    }
    histStripX->Write();
  }
  hist2d->Write();
  TString histNameWhole;
  histNameWhole.Form("calib_90_all");
  auto histWhole = new TH1D(histNameWhole, histNameWhole, 512, 0, 10.24e-3);
  histWhole->GetYaxis()->SetTitle("Counts, [1]");
  histWhole->GetXaxis()->SetTitle("E, [GeV]");
  //tree->Draw(partBr+lvLeave+"-"+partBr+lvLeave2 + ">>" + histNameWhole, "");
  tree->Draw(partBr+lvLeave+"-"+partBr + ">>" + histNameWhole, "");
  histWhole->Write();

  // saveFile->Close();
} 
