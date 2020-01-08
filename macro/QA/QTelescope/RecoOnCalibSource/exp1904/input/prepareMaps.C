void prepareMaps () {
  auto histSens = new TH2D("map_sens", "map_sens", 16, 0, 16,
                                                   16, 0, 16);
  vector<double> individStripThickness = { 2.11186, 2.10358, 2.0558, 2.11453, 
                                           1.98929, 2.09989, 2.10582, 1.93625, 
                                           2.20466, 2.39156, 2.59378, 2.14192, 
                                           2.16428, 2.2278, 1.91903, 2.17847};
  auto histDeadFront = new TH2D("map_deadFront", "map_deadFront", 
                                 16, 0, 16,
                                 16, 0, 16);
  auto histDeadBack = new TH2D("map_deadBack", "map_deadBack", 
                                16, 0, 16,
                                16, 0, 16);
  for (int i = 0; i < 16; i++) {
    double sensThickness = 1000 * 1e-4;
    double backDead = 2e-4;
    double frontDead = individStripThickness[i] * 1e-4;
    for (int j = 0; j < 16; j++) {
      histDeadFront->SetBinContent(i + 1, j + 1, frontDead);
      histDeadBack->SetBinContent(i + 1, j + 1, backDead);
      histSens->SetBinContent(i + 1, j + 1, sensThickness);
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