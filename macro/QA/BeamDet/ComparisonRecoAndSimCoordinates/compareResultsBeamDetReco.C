void compareResultsBeamDetReco () {
  TString simFileName = "sim_digi.root";
  TString recoFileName = "track.root";
  auto file_sim = TFile::Open(simFileName.Data());
  auto file_track = TFile::Open(recoFileName.Data());
  auto tree_sim = (TTree*)file_sim->Get("er");
  auto tree_track = (TTree*)file_track->Get("er");

  int histLeftBoundCoords = -2;
  int histRightBoundCoords = 2;
  int histBinCountCoords = 120*4;
  int histLeftBoundAng = -2;
  int histRightBoundAng = 2;
  int histBinCountAng = 120*50;
  int halfOfBinCoords = ((double)(histRightBoundCoords - histLeftBoundCoords))/histBinCountCoords/2.;
  int halfOfBinAng = ((double)(histRightBoundAng - histLeftBoundAng))/histBinCountAng/2.;

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

  auto track_sim = new TClonesArray("ERBeamDetTargetPoint", 1000);
  auto track_reco = new TClonesArray("ERBeamDetTrack", 1000);
  tree_sim->SetBranchAddress("BeamDetTargetPoint", &track_sim);
  tree_track->SetBranchAddress("BeamDetTrack", &track_reco);

  unsigned maxEventNb = min(tree_sim->GetEntriesFast(), tree_track->GetEntriesFast());
  for (unsigned i = 0; i < maxEventNb; i++) {
    tree_sim->GetEntry(i);
    tree_track->GetEntry(i);
    if (track_reco->GetEntriesFast() && track_sim->GetEntriesFast()) {
      double simTargetX = ((ERBeamDetTargetPoint*)track_sim->At(0))->GetXIn();
      double recoTargetX = ((ERBeamDetTrack*)track_reco->At(0))->GetTargetX();
      double diffX = simTargetX - recoTargetX;
      int binNb = hist_diffX->GetXaxis()->FindBin(diffX + halfOfBinCoords);
      hist_diffX->AddBinContent(binNb);

      double simPxPz = ((ERBeamDetTargetPoint*)track_sim->At(0))->GetPxIn()
                     / ((ERBeamDetTargetPoint*)track_sim->At(0))->GetPzIn();
      double recoPxPz = ((ERBeamDetTrack*)track_reco->At(0))->GetVector().Px()
                      / ((ERBeamDetTrack*)track_reco->At(0))->GetVector().Pz();
      double diffPxPz = simPxPz - recoPxPz; 
      binNb = hist_diffPxPz->GetXaxis()->FindBin(diffPxPz + halfOfBinAng);
      hist_diffPxPz->AddBinContent(binNb);
    }
  }

  gROOT->ForceStyle();
  gStyle->SetOptStat(1001101);  
  auto canv = new TCanvas();
  canv->Divide(2, 1);
  canv->cd(1);
  hist_diffX->GetXaxis()->SetRange(hist_diffX->GetXaxis()->FindBin(-0.2), 
                                   hist_diffX->GetXaxis()->FindBin(0.2));
  hist_diffX->SaveAs(TString("hist_") + hist_diffX->GetName() + ".root");
  hist_diffX->Draw("");

  canv->cd(2);
  hist_diffPxPz->GetXaxis()->SetRange(hist_diffPxPz->GetXaxis()->FindBin(-0.02), 
                                   hist_diffPxPz->GetXaxis()->FindBin(0.02));
  hist_diffPxPz->SaveAs(TString("hist_") + hist_diffPxPz->GetName() + ".root");
  hist_diffPxPz->Draw("");

  canv->SetCanvasSize(1920, 1080);
  TImage *img = TImage::Create();
  img->FromPad(canv);
  img->Scale(1920, 1080);
  img->WriteImage(TString("compareCanvas") + ".png");
}
