void compareResultsBeamDetReco (TString fileInd = "") {
  TString simFileName;
  TString recoFileName;
  // simFileName.Form("sim_digi_%s.root", fileInd.Data());
  // recoFileName.Form("track_%s.root", fileInd.Data());
  simFileName.Form("sim_digi.root", fileInd.Data());
  recoFileName.Form("track.root", fileInd.Data());
  auto file_sim = TFile::Open(simFileName.Data());
  // auto file_track = TFile::Open("track_spread.root");
  auto file_track = TFile::Open(recoFileName.Data());
// sim_digi_lowDens.root
// track_lowDens.root
// sim_digi_lowDens_spread.root
// track_lowDens_spread.root
// sim_digi_origin.root
// track_origin.root
// sim_digi_origin_spread.root
// track_origin_spread.root
  auto tree_sim = (TTree*)file_sim->Get("er");
  auto tree_track = (TTree*)file_track->Get("er");

  int histLeftBoundCoords = -2;
  int histRightBoundCoords = 2;
  int histBinCountCoords = 120*4;
  int histLeftBoundAng = -2;
  int histRightBoundAng = 2;
  int histBinCountAng = 120*50;
  int halfOfBinCoords = ((double)(histRightBoundCoords - histLeftBoundCoords))/histBinCountCoords/2.;

  auto randGen = new TRandom3();

  auto hist_diffX = new TH1D ("X_reco-X_sim", "X_reco-X_sim", histBinCountCoords, 
                                                              histLeftBoundCoords, 
                                                              histRightBoundCoords);
  hist_diffX->GetXaxis()->SetTitle("X_reco-X_sim, [cm]");
  hist_diffX->GetYaxis()->SetTitle("Counts");
  auto hist_diffPxPz = new TH1D ("PxPz_reco-PxPz_sim", "PxPz_reco-PxPz_sim", histBinCountAng, 
                                                              histLeftBoundAng, 
                                                              histRightBoundAng);
  hist_diffPxPz->GetXaxis()->SetTitle("PxPz_reco-PxPz_sim, [1]");
  hist_diffPxPz->GetYaxis()->SetTitle("Counts");
  auto hist_diffX_Spread = new TH1D ("X_reco-X_sim, spread", "X_reco-X_sim, spread", 
                                                              histBinCountCoords, 
                                                              histLeftBoundCoords, 
                                                              histRightBoundCoords);
  hist_diffX_Spread->GetXaxis()->SetTitle("X_reco+U(-0.5, 0.5)*WireStep-X_sim, [cm]");
  hist_diffX_Spread->GetYaxis()->SetTitle("Counts");
  auto hist_diffY_Spread = new TH1D ("Y_reco-Y_sim, spread", "Y_reco-Y_sim, spread", 
                                                              histBinCountCoords, 
                                                              histLeftBoundCoords, 
                                                              histRightBoundCoords);
  hist_diffY_Spread->GetXaxis()->SetTitle("Y_reco+U(-0.5, 0.5)*WireStep-Y_sim, [cm]");
  hist_diffY_Spread->GetYaxis()->SetTitle("Counts");
  double stripWidth = 0.125;
  int halfOfBinAng = ((double)(histRightBoundAng - histLeftBoundAng))/histBinCountAng/2.;
  auto hist_diffTheta = new TH1D ("Theta_reco-Theta_sim", "Theta_reco-Theta_sim", histBinCountAng, 
                                                                                  histLeftBoundAng, 
                                                                                  histRightBoundAng);
  hist_diffTheta->GetXaxis()->SetTitle("Theta_reco-Theta_sim, [rad]");
  hist_diffTheta->GetYaxis()->SetTitle("Counts");
  auto hist_diffPhi = new TH1D ("Phi_reco-Phi_sim", "Phi_reco-Phi_sim", histBinCountAng, 
                                                              histLeftBoundAng, 
                                                              histRightBoundAng);
  hist_diffPhi->GetXaxis()->SetTitle("Phi_reco-Phi_sim, [rad]");
  hist_diffPhi->GetYaxis()->SetTitle("Counts");
  auto track_sim = new TClonesArray("ERBeamDetTrack", 1000);
  auto track_reco = new TClonesArray("ERBeamDetTrack", 1000);
  tree_sim->SetBranchAddress("BeamDetMCTrack", &track_sim);
  tree_track->SetBranchAddress("BeamDetTrack", &track_reco);

  for (unsigned i = 0; i < tree_track->GetEntriesFast(); i++) {
    tree_sim->GetEntry(i);
    tree_track->GetEntry(i);
    // cout << "track_sim->GetEntriesFast() " << track_sim->GetEntriesFast() << endl;
    // cout << "track_reco->GetEntriesFast() " << track_reco->GetEntriesFast() << endl;
    if (track_reco->GetEntriesFast() && track_sim->GetEntriesFast()) {
      double diffX = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetX() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetX();
      int binNb = hist_diffX->GetXaxis()->FindBin(diffX + halfOfBinCoords);
      hist_diffX->AddBinContent(binNb);

      double diffPxPz = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetPxPz() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetPxPz();
      binNb = hist_diffPxPz->GetXaxis()->FindBin(diffPxPz + halfOfBinCoords);
      hist_diffPxPz->AddBinContent(binNb);

      double diffX_Spread = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetX() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetX() 
                    + randGen->Uniform(-0.5, 0.5) * stripWidth;
      
      binNb = hist_diffX_Spread->GetXaxis()->FindBin(diffX_Spread + halfOfBinCoords);
      hist_diffX_Spread->AddBinContent(binNb);

      double diffY_Spread = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetY() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetY()
                    + randGen->Uniform(-0.5, 0.5) * stripWidth;
      binNb = hist_diffY_Spread->GetXaxis()->FindBin(diffY_Spread + halfOfBinCoords);
      hist_diffY_Spread->AddBinContent(binNb);


      double diffTheta = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetVertex().Theta() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetVertex().Theta();
      binNb = hist_diffTheta->GetXaxis()->FindBin(diffTheta + halfOfBinAng);
      hist_diffTheta->AddBinContent(binNb);

      double diffPhi = ((ERBeamDetTrack*)track_sim->At(0))->GetTargetVertex().Phi() 
                  - ((ERBeamDetTrack*)track_reco->At(0))->GetTargetVertex().Phi();
      binNb = hist_diffPhi->GetXaxis()->FindBin(diffPhi + halfOfBinAng);
      hist_diffPhi->AddBinContent(binNb);
    }
  }
  // gStyle->SetImageScaling(3.);
  gROOT->ForceStyle();
  gStyle->SetOptStat(1001101);  
  auto canv = new TCanvas();
  canv->Divide(2, 1);
  canv->cd(1);
  hist_diffX->GetXaxis()->SetRange(hist_diffX->GetXaxis()->FindBin(-1.9), 
                                   hist_diffX->GetXaxis()->FindBin(1.9));
  hist_diffX->SaveAs(TString("hist_") + hist_diffX->GetName() + ".root");
  hist_diffX->Draw("");

  canv->cd(2);
  hist_diffPxPz->GetXaxis()->SetRange(hist_diffPxPz->GetXaxis()->FindBin(-0.2), 
                                   hist_diffPxPz->GetXaxis()->FindBin(0.2));
  hist_diffPxPz->SaveAs(TString("hist_") + hist_diffPxPz->GetName() + ".root");
  hist_diffPxPz->Draw("");

  // canv->cd(3);
  // hist_diffTheta->GetXaxis()->SetRange(hist_diffTheta->GetXaxis()->FindBin(-1.9), 
  //                                  hist_diffTheta->GetXaxis()->FindBin(1.9));
  // hist_diffTheta->Draw("");
  // canv->cd(4);
  // hist_diffPhi->GetXaxis()->SetRange(hist_diffPhi->GetXaxis()->FindBin(-1.9), 
  //                                  hist_diffPhi->GetXaxis()->FindBin(1.9));
  // hist_diffPhi->Draw("");

  // canv->SaveAs(TString("compareCanvas") + fileInd + ".png");
   canv->SetCanvasSize(1860, 986);
   TImage *img = TImage::Create();
   img->FromPad(canv);
   img->Scale(1860, 986);
   img->WriteImage(TString("compareCanvas") + fileInd + ".png");
  // auto canv_spread = new TCanvas();
  // canv_spread->Divide(2, 2);
  // canv_spread->cd(1);
  // hist_diffX->Draw("");
  // canv_spread->cd(2);
  // hist_diffPxPz->Draw("");
  // canv_spread->cd(3);
  // hist_diffX_Spread->GetXaxis()->SetRange(0, histBinCountCoords);
  // hist_diffX_Spread->Draw("");
  // canv_spread->cd(4);
  // hist_diffY_Spread->GetXaxis()->SetRange(0, histBinCountCoords);
  // hist_diffY_Spread->Draw("");


}
