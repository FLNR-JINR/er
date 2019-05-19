void fileAnalysis() {
  auto file = TFile::Open("expCalib_reco.root");
  auto tree = (TTree*)file->Get("er");

  TString trackBr = "ERQTelescopeTrack_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y.";
  TString xCoord = "fTelescopeGlobalX";
  TString yCoord = "fTelescopeGlobalY";
  TString partBr = "ERQTelescopeParticle_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y_1000020040.";
  TString lvLeave = "fT";
  TString lvLeaveNoCorr = "fT_noCorrections";
  auto canv = new TCanvas();
  cout << trackBr+xCoord << endl;
  // tree->Draw(trackBr+xCoord,"", "");
  // tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55");
  tree->Draw(partBr+lvLeave,xCoord + "<0.6&&" + xCoord + ">0.4&&"
                            +yCoord + "<0.6&&" + yCoord + ">0.5");
  auto canv1 = new TCanvas();
  tree->Draw(partBr+lvLeaveNoCorr,xCoord + "<0.6&&" + xCoord + ">0.4&&"
                            +yCoord + "<0.6&&" + yCoord + ">0.5");


  // tree->Draw(partBr+lvLeave,yCoord + "<0.6&&" + yCoord + ">0.5");
  // tree->Draw(partBr+lvLeave,xCoord + "<0.6&&" + xCoord + ">0.4");
  // tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55");
  // tree->Draw(partBr+lvLeave,yCoord + "<-0.4&&" + yCoord + ">-0.6");
  // tree->Draw(partBr+lvLeave,xCoord + "<-0.4&&" + xCoord + ">-0.55&&"
  //                           +yCoord + "<-0.4&&" + yCoord + ">-0.6");
  // tree->Draw(partBr+lvLeave,"");
  // TClonesArray* particles = new TClonesArray("ERQTelescopeParticle",10000);
  // TClonesArray* tracks = new TClonesArray("ERQTelescopeTrack",10000);
  // tree->SetBranchAddress("ERQTelescopeParticle_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y_1000020040", &particles);
  // tree->SetBranchAddress("ERQTelescopeTrack_Left_telescope_SingleSi_SSD20_L_X_0Left_telescope_DoubleSi_DSD_L_XY_0_Y", &tracks);

  // for (unsigned entrNb = 0; entrNb < tree->GetEntriesFast(); entrNb++) {
  //   tree->GetEntry(entrNb);
  //   Int_t xStripNb;
  //   for (Int_t iTrack = 0; iTrack < SetBranchAddress->GetEntriesFast(); iTrack++) {
  //     ERQTelescopeTrack* track = (ERQTelescopeTrack*) tracks->At(iTrack);
  //     TVector3 trackVec = GetTelescopeVertex();
  //     double trackXcoord = trackVec.X();
  //     double trackYcoord = trackVec.Y();
  //     if (xCoord < 0.6 && xCoord > 0.4 && yCoord<0.6 && yCoord >0.5) {
  //       ERQTelescopeParticle* particle = (ERQTelescopeParticle*) particles->At(iTrack);
  //       double fullE = particle->GetT();
  //       double edepPart = particle->GetDeadEloss();
  //     }
  //     if (strip->GetStripNb() >= 0) {
  //       // cout << "StripSize " << strip->GetStripNb() << endl;
  //       xStripNb = strip->GetStripNb();
  //     } 
  //   }

  //   for (Int_t iStripY = 0; iStripY < yStrip->GetEntriesFast(); iStripY++) {
  //   }
  // }

} 