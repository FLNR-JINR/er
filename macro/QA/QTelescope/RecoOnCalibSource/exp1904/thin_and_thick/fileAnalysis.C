void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco.root");
  auto tree = (TTree*)file->Get("er");

  //TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0.";
  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1.";
  //TString xCoord = trackBr + "fTelescopeGlobalX";
  //TString yCoord = trackBr + "fTelescopeGlobalY";
  TString xCoord = trackBr + "fTelescopeLocalX";
  TString yCoord = trackBr + "fTelescopeLocalY";
  //TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_1Telescope_1_SingleSi_SSD20_1_Y_0_1000020040.";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1_1000020040.";
  TString lvLeave = "fT";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  auto saveFile = TFile::Open("expCalib_reco_SSD_1m_1_st14.root", "RECREATE");
  double delta = 0.05;
  double thinStartLoc = -2.344;
  double thickStartLoc = -2.85;
  double stepThin = 0.313;
  double stepThick = 0.38;

  double stepX = stepThin;
  double stepY = stepThick;
  double startCoordX = thinStartLoc;
  double startCoordY = thickStartLoc;
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
