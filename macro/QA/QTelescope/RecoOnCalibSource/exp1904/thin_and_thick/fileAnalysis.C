void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco_2_sources.root");
  auto tree = (TTree*)file->Get("er");
  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0.";
  TString xCoord = trackBr + "fTelescopeLocalX";
  TString yCoord = trackBr + "fTelescopeLocalY";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0_1000020040.";
  TString lvLeave = "fKineticEnergy";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  auto saveFile = TFile::Open("expCalib_reco_hists.root", "RECREATE");
  double delta = 0.1;
  double thickStartLoc = 2.8125;
  double thinStartLoc = 2.34375;
  double stepThick = -(2.8125 - 2.4375);
  double stepThin = -(2.34375 - 2.03125);
  double stepX = stepThick;
  double stepY = stepThin;
  double startCoordX = thickStartLoc;
  double startCoordY = thinStartLoc;
  TString histParams = "(65536, 0, 10.24e-3)";
  for (int i = 0; i < 16; i++) {
    TString canvName;
    TString stripXName;
    canvName.Form("canvas_pixels_X%d", i);
    stripXName.Form("strip_pixel_x-%d", i);
    TH2D* hist2dPixels = new TH2D(stripXName, stripXName, 65536, 0, 10.24e-3, 16, 0, 16);
    auto curCanv = new TCanvas(canvName);
    curCanv->Divide(4, 4);
    double currCoordX = startCoordX + i*stepX;
    cout << currCoordX << endl;
    for (int j = 0; j < 16; j++) {
      double currCoordY = startCoordY + j*stepY;
      curCanv->cd(j + 1);
      TString histNamePix;
      TString cond;
      cond.Form("%s>%lf&&%s<%lf&&%s>%lf&&%s<%lf", xCoord.Data(), currCoordX - delta,
                                                  xCoord.Data(), currCoordX + delta,
                                                  yCoord.Data(), currCoordY - delta,                                           
                                                  yCoord.Data(), currCoordY + delta);
      histNamePix.Form("pixel_X-%d_Y%d", i, j);
      auto histPix = new TH1D(histNamePix, histNamePix, 65536, 0, 10.24e-3);
      histPix->GetYaxis()->SetTitle("Counts, [1]");
      histPix->GetXaxis()->SetTitle("E, [GeV]");
      tree->Draw(partBr+lvLeave + ">>" + histNamePix, cond);
      auto histTmp = (TH1D*)gDirectory->Get(histNamePix);
      for (int iBin = 1; iBin <= histTmp->GetXaxis()->GetNbins(); iBin++) {
        int binContent = histTmp->GetBinContent(iBin);
        hist2dPixels->SetBinContent(iBin, j + 1, binContent);
      }
      histPix->Write();
    }
    curCanv->Write();
    hist2dPixels->Write();
  }
  TH2D* hist2d = new TH2D("reco_strip_x", "reco_strip_x", 65536, 0, 10.24e-3, 16, 0, 16);
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
    auto histStripX = new TH1D(histNameStripX, histNameStripX, 65536, 0, 10.24e-3);
    histStripX->GetYaxis()->SetTitle("Counts, [1]");
    histStripX->GetXaxis()->SetTitle("E, [GeV]");
    tree->Draw(partBr+lvLeave + ">>" + histNameStripX, condX);
    cout << "histNameStripX " << histNameStripX << endl;
    auto histTmp = (TH1D*)gDirectory->Get(histNameStripX);
    for (int iBin = 1; iBin <= histTmp->GetXaxis()->GetNbins(); iBin++) {
      int binContent = histTmp->GetBinContent(iBin);
      hist2d->SetBinContent(iBin, i + 1, binContent);
    }
    histStripX->Write();
  }
  hist2d->Write();
  TH2D* hist2dY = new TH2D("reco_strip_y", "reco_strip_y", 65536, 0, 10.24e-3, 16, 0, 16);
  for (int i = 0; i < 16; i++) {
    TString canvName;
    canvName.Form("canvas_Y%d", i);
    auto curCanv = new TCanvas(canvName);
    curCanv->Divide(4, 4);
    double currCoordY = startCoordY + i*stepY;
    cout << currCoordY << endl;
    TString histName, histNameStripY;
    TString condY;
    condY.Form("%s>%lf&&%s<%lf", yCoord.Data(), currCoordY - delta,
                                 yCoord.Data(), currCoordY + delta);
    histNameStripY.Form("strip_Y-%d", i);
    auto histStripY = new TH1D(histNameStripY, histNameStripY, 65536, 0, 10.24e-3);
    histStripY->GetYaxis()->SetTitle("Counts, [1]");
    histStripY->GetXaxis()->SetTitle("E, [GeV]");
    tree->Draw(partBr+lvLeave + ">>" + histNameStripY, condY);
    auto histTmp = (TH1D*)gDirectory->Get(histNameStripY);
    for (int iBin = 1; iBin <= histTmp->GetXaxis()->GetNbins(); iBin++) {
      int binContent = histTmp->GetBinContent(iBin);
      hist2dY->SetBinContent(iBin, i + 1, binContent);
    }
    histStripY->Write();
  }
  hist2dY->Write();
  TString histNameWhole;
  histNameWhole.Form("calib_90_all");
  auto histWhole = new TH1D(histNameWhole, histNameWhole, 65536, 0, 10.24e-3);
  histWhole->GetYaxis()->SetTitle("Counts, [1]");
  histWhole->GetXaxis()->SetTitle("E, [GeV]");
  tree->Draw(partBr+lvLeave+"-"+partBr + ">>" + histNameWhole, "");
  histWhole->Write();
  // saveFile->Close();
} 
