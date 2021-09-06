auto gLegend = new TLegend(0.1,0.7,0.48,0.9);
void addLineMultiGraph(TMultiGraph *multyGraph, const float *x, const float *y, 
                       int colors, int markerStyle, int pointsCount,  TString name) {
  
  auto gr = new TGraph(pointsCount, x, y);
  gr->SetLineColor(colors);
  gr->SetMarkerColor(colors);
  gr->SetLineWidth(2);
  gr->SetMarkerSize(1.5);
  gr->SetMarkerStyle(markerStyle/*21*/);
  gr->SetName(name);

  multyGraph->Add(gr);
  multyGraph->Draw("APL");

  gLegend->AddEntry(gr, name, "p");
}

void fit_eta() {
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  // --------------------- Calculator initialization --------------------------
  ERElossCalculatorUtils *calculator = ERElossCalculatorUtils::Instance();
  calculator->Init();
  // --------------------------------------------------------------------------
  const unsigned pid = 1000020040;
  const TString material = "silicon";
  const float number_of_steps = 1e4; // number of integration steps
  // --------------------------------------------------------------------------
  std::vector<float> energies = {4.7844, 6.0024, 7.6869}; // [MeV] 
  std::vector<float> thicknesses = {0., 1., 2., 3., 4., 5., 6.}; // [um]
  // ------------- Create graph -----------------------------------------------
  auto mgEta = new TMultiGraph();
  TString mgTitle = "d(eta)";
  mgEta->SetTitle(mgTitle);
  // -------- Evaluate energies after dead layer ------------------------------
  std::vector<std::vector<float>> energies_after_dead_layer;
  for (const auto &thickness: thicknesses) {
    std::vector<float> eneries_lost;
    for (const auto &energy: energies) {
      const float eloss = calculator->IntegralEloss(pid, energy, material, 
                                                    thickness, number_of_steps, false);
      eneries_lost.push_back((energy - eloss) * 1e3 /*to [KeV]*/);
    }
    energies_after_dead_layer.push_back(eneries_lost);
  }
  // --------- Evaluate `eta` parameter ---------------------------------------
  std::vector<float> eta(thicknesses.size());
  for (int i = 0; i < energies_after_dead_layer.size(); i++) {
    eta[i] = (energies_after_dead_layer[i][2] - energies_after_dead_layer[i][1]) 
           / (energies_after_dead_layer[i][2] - energies_after_dead_layer[i][0]); 
  }
  // --------- Add lines on graph ---------------------------------------------
  addLineMultiGraph(mgEta, &eta[0], &thicknesses[0], 
                    kBlue,  22, eta.size(), "EmCalculator");
  gLegend->Draw();
  mgEta->GetXaxis()->SetTitle("eta = (E3-E2)/(E3-E1),\n [1]");
  mgEta->GetYaxis()->SetTitle("d, [um]");
  // --------- Fit ------------------------------------------------------------
  TFitResultPtr fitRes = mgEta->Fit("pol2","S");
  cout << "Fit params " << fitRes->Parameter(0) 
       << ", " << fitRes->Parameter(1) 
       << ", " << fitRes->Parameter(2) << endl;
}
