void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0.";
  TString xCoord = trackBr + "fTelescopeGlobalX";
  TString yCoord = trackBr + "fTelescopeGlobalY";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0_1000020040.";
  TString lvLeave = "fT";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  // tree->Draw(trackBr+xCoord,"", "");
  // tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55");
  // tree->Draw(partBr+lvLeave,xCoord + "<0.6&&" + xCoord + ">0.4&&"
  //                           +yCoord + "<0.6&&" + yCoord + ">0.5");
  // tree->Draw(partBr+lvLeave,yCoord + "<0.6&&" + yCoord + ">0.5");
  // tree->Draw(partBr+lvLeave,xCoord + "<0.6&&" + xCoord + ">0.4");
  // tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55");
  // tree->Draw(partBr+lvLeave,yCoord + "<-0.4&&" + yCoord + ">-0.6");
  auto saveFile = TFile::Open("expCalib_reco_SSD_1m_1_st14.root", "RECREATE");
  double delta = 0.05;
  double stepY = 0.313;
  double stepX = 0.38;
  double startCoordY = -2.344;
  double startCoordX = 2.85 - stepX;
  TString histParams = "(1048, 0, 10.24e-3)";
  for (int i = 0; i < 16; i++) {
    double currCoordY = startCoordY + i*stepY;
    
    TString cond;
    cond.Form("%s>%lf&&%s<%lf&&%s>%lf&&%s<%lf", yCoord.Data(), currCoordY - delta, 
		                                  yCoord.Data(), currCoordY + delta, 
					          xCoord.Data(), startCoordX - delta,
					          xCoord.Data(), startCoordX + delta);
    TString histName;
    histName.Form("strip_%d", i);
    tree->Draw(partBr+lvLeave + ">>" + histName + histParams, cond);
  }
  saveFile->Write();
  saveFile->Close();
} 
