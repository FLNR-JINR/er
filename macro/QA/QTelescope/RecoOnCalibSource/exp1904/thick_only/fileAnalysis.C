void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco_1.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Telescope_1_SingleSi_SSD_1_X_0Fictious.";
  TString xCoord = trackBr + "fTelescopeGlobalX";
  TString yCoord = trackBr + "fTelescopeGlobalY";
  TString partBr = "ERQTelescopeParticle_Telescope_1_SingleSi_SSD_1_X_0Fictious_1000020040.";
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
  auto saveFile = TFile::Open("expCalib_reco_SSD_1m_1_oneSensor.root", "RECREATE");
  double delta = 0.05;
//  double stepY = 0.313;
  double stepX = 0.38;
//  double startCoordY = -2.344;
  double startCoordX = -2.85;

  TString histParams;  
  Int_t binCount = 1024;
  double upBound = 10e-3;
  histParams.Form("(%d, 0, %lf)", binCount, upBound);
  double halfBinE = upBound / binCount;
  TString halfBinEstr;
  halfBinEstr.Form("+%lf", halfBinE);
  for (int i = 0; i < 16; i++) {
    double currCoordX = startCoordX + i*stepX;
    
    TString cond;
    cond.Form("%s>%lf&&%s<%lf", xCoord.Data(), currCoordX - delta, xCoord.Data(), currCoordX + delta);
    TString histName;
    histName.Form("strip_%d", i);
    tree->Draw(partBr+lvLeave+halfBinEstr+">>" + histName + histParams, cond);
  }
  saveFile->Write();
  saveFile->Close();
} 
