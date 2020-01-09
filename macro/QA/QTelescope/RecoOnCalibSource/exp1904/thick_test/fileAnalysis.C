void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco_quad_mean.root");
  auto tree = (TTree*)file->Get("er");

  //TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD_1_Y_0.";
  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_0Fictious.";
  //TString xCoord = trackBr + "fTelescopeGlobalX";
  //TString yCoord = trackBr + "fTelescopeGlobalY";
  TString xCoord = trackBr + "fTelescopeLocalX";
  TString yCoord = trackBr + "fTelescopeLocalY";
  //TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD_1_Y_0_1000020040.";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_0Fictious_1000020040.";
  TString lvLeave = "fT";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  auto saveFile = TFile::Open("expCalib_reco_hists.root", "RECREATE");
  double delta = 0.05;
  double thickStartLoc = 2.8125;
  double stepThick = -(2.8125 - 2.4375);

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
    // for (int j = 0; j < 16; j++) {
      TString histName, histNameStripX, histNameStripY;
      // histName.Form("pixel_X-%d_Y-%d", i, j);
      // auto hist = new TH1D(histName, histName, 1024, 0, 10.24e-3);
      // hist->GetYaxis()->SetTitle("Counts, [1]");
      // hist->GetXaxis()->SetTitle("E, [GeV]");
      // double currCoordY = startCoordY + j*stepY;
      TString cond, condX, condY;
      // cond.Form("%s>%lf&&%s<%lf&&%s>%lf&&%s<%lf", yCoord.Data(), currCoordY - delta, 
      //                                             yCoord.Data(), currCoordY + delta, 
      // 						  xCoord.Data(), currCoordX - delta,
      //                                             xCoord.Data(), currCoordX + delta);
      condX.Form("%s>%lf&&%s<%lf", xCoord.Data(), currCoordX - delta,
                                   xCoord.Data(), currCoordX + delta);
      // condY.Form("%s>%lf&&%s<%lf", yCoord.Data(), currCoordY - delta,
      //                              yCoord.Data(), currCoordY + delta);
      // //curCanv->cd(j + 1);
      // tree->Draw(partBr+lvLeave + ">>" + histName, cond);
      // if (j == 0) {
      histNameStripX.Form("strip_X-%d", i);
      auto histStripX = new TH1D(histNameStripX, histNameStripX, 512, 0, 10.24e-3);
      histStripX->GetYaxis()->SetTitle("Counts, [1]");
      histStripX->GetXaxis()->SetTitle("E, [GeV]");
      tree->Draw(partBr+lvLeave + ">>" + histNameStripX, condX);
      cout << "histNameStripX " << histNameStripX << endl;
      auto histTmp = (TH1D*)gDirectory->Get(histNameStripX);
      for (int iBin = 1; iBin <= histTmp->GetXaxis()->GetNbins(); iBin++) {
        int binContent = histTmp->GetBinContent(iBin);
        hist2d->SetBinContent(iBin, i + 1, binContent);
      }
      hist2d->Write();
      histStripX->Write();
      // }
      // if (i == 0) {
      //   histNameStripY.Form("strip_Y-%d", j);
      //   auto histStripY = new TH1D(histNameStripY, histNameStripY, 1024, 0, 10.24e-3);
      //   histStripY->GetYaxis()->SetTitle("Counts, [1]");
      //   histStripY->GetXaxis()->SetTitle("E, [GeV]");
      //   tree->Draw(partBr+lvLeave + ">>" + histNameStripY, condY);
      //   histStripY->Write();
      // }
      // hist->Write();
    // }
    //curCanv->Write();
  }
  TString histNameWhole;
  histNameWhole.Form("calib_90_all");
  auto histWhole = new TH1D(histNameWhole, histNameWhole, 1024, 0, 10.24e-3);
  histWhole->GetYaxis()->SetTitle("Counts, [1]");
  histWhole->GetXaxis()->SetTitle("E, [GeV]");
  tree->Draw(partBr+lvLeave + ">>" + histNameWhole, "");
  histWhole->Write();

  // saveFile->Close();
} 
