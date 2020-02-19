void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco_quad_mean_1_sens.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Telescope_1_DoubleSi_SSD_1_XY_0_XTelescope_1_DoubleSi_SSD_1_XY_1_Y.";
  TString xCoord = trackBr + "fTelescopeLocalX";
  TString yCoord = trackBr + "fTelescopeLocalY";
  TString partBr = "ERQTelescopeParticle_Telescope_1_DoubleSi_SSD_1_XY_0_XTelescope_1_DoubleSi_SSD_1_XY_1_Y_1000020040.";
  TString lvLeave = "fKineticEnergy";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  auto saveFile = TFile::Open("expCalib_reco_hists.root", "RECREATE");
  double delta = 0.1;
  double thickStartLoc = 2.34375;
  double stepThick = -(2.34375 - 2.03125);
  double thickStartLoc = 2.8125;
  double stepThick = -(2.8125 - 2.4375);
  double stepX = stepThick;
  double startCoordX = thickStartLoc;
  TString histParams = "(65536, 0, 10.24e-3)";
  double a = 0.0167078 *1e-3;
  double b = -0.395058*1e-3;
  TString halfbin;
  halfbin.Form("%lf", a/2);
  vector<TH1D*> hists;
  vector<TCanvas*> canvases;
  TH2D* hist2d = new TH2D("reco", "reco", 65536, 0, 10.24e-3, 16, 0, 16);
  for (int i = 0; i < 16; i++) {
    TString canvName;
    canvName.Form("canvas_X%d", i);
    auto curCanv = new TCanvas(canvName);
    curCanv->Divide(4, 4);
    double currCoordX = startCoordX + i*stepX;
    cout << currCoordX << endl;
    TString histName, histNameStripX, histNameStripY;
    TString cond, condX, condY;
    condX.Form("%s>%lf&&%s<%lf", xCoord.Data(), currCoordX - delta,
                                 xCoord.Data(), currCoordX + delta);
    histNameStripX.Form("strip_X-%d", i);
    //auto histStripX = new TH1D(histNameStripX, histNameStripX, 65536, b, 10.24e-3 + b);
    auto histStripX = new TH1D(histNameStripX, histNameStripX, 65536, 0, 10.24e-3);
    histStripX->GetYaxis()->SetTitle("Counts, [1]");
    histStripX->GetXaxis()->SetTitle("E, [GeV]");
    tree->Draw(partBr+lvLeave + "+" + halfbin + ">>" + histNameStripX, condX);
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
  histNameWhole.Form("calib_65");
  auto histWhole = new TH1D(histNameWhole, histNameWhole, 65536, 0, 10.24e-3);
  histWhole->GetYaxis()->SetTitle("Counts, [1]");
  histWhole->GetXaxis()->SetTitle("E, [GeV]");
  //tree->Draw(partBr+lvLeave+"-"+partBr+lvLeave2 + ">>" + histNameWhole, "");
  tree->Draw(partBr+lvLeave+"-"+partBr + ">>" + histNameWhole, "");
  histWhole->Write();

  // saveFile->Close();
} 
