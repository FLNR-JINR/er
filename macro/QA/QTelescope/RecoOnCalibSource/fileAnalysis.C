void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco_pid_check.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y.";
  TString xCoord = "fTelescopeGlobalX";
  TString yCoord = "fTelescopeGlobalY";
  TString partBr = "ERQTelescopeParticle_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y_1000020040.";
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
  tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55&&"
                            +yCoord + "<-0.4&&" + yCoord + ">-0.6");
  // tree->Draw(partBr+lvLeave,"");
} 