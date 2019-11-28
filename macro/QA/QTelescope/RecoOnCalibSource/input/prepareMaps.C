void prepareMaps () {
  auto file = TFile::Open("map_left.root");
  auto histFullPassive = (TH2D*)file->Get(file->GetListOfKeys()->At(0)->GetName());
  auto histSens = new TH2D("map_sens", "map_sens", 16, 0, 16,
                                                   16, 0, 16);
  auto histDeadFront = new TH2D("map_deadFront", "map_deadFront", 
                                 16, 0, 16,
                                 16, 0, 16);
  auto histDeadBack = new TH2D("map_deadBack", "map_deadBack", 
                                16, 0, 16,
                                16, 0, 16);
  for (int i = 1; i <= 16; i++) {
    for (int j = 1; j <= 16; j++) {
      double content = histFullPassive->GetBinContent(i, j);
      if (content < 15. || content > 30) {
        content = 22e-4;
      } else {
        content = (content - 1.99) * 1e-4;
      }
      cout << content << endl;
      double backDead = 0.7e-4;
      double frontDead = 0.5e-4;
      if (i == 7 && j == 1) {
        // backDead = 0.62e-4;
        backDead = 0.7e-4;
      }
      if (i == 7 && j == 7) {
        // backDead = 0.54e-4;
        backDead = 0.68e-4;
      }
      histDeadFront->SetBinContent(i, j, frontDead);
      histDeadBack->SetBinContent(i, j, backDead);
      histSens->SetBinContent(i, j, content - backDead - frontDead);
    }
  }
  auto canv = new TCanvas(); 
  histSens->Draw("col&&text");
  auto canv1 = new TCanvas(); 
  histDeadFront->Draw("col&&text");
  auto canv2 = new TCanvas(); 
  histDeadBack->Draw("col&&text");
  histSens->SaveAs(TString(histSens->GetName()) + ".root");
  histDeadFront->SaveAs(TString(histDeadFront->GetName()) + ".root");
  histDeadBack->SaveAs(TString(histDeadBack->GetName()) + ".root"); 
}