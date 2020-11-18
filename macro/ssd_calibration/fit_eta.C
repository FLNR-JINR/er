// Macro is intended for fitting \eta parameter dependence

auto gLegend = new TLegend(0.1,0.7,0.48,0.9);
void addLineMultiGraph(TMultiGraph *multyGraph, const Double_t *x, const Double_t *y, 
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

void fit_eta () {
  auto canv = new TCanvas();
  gLegend->SetHeader("Legend","C"); // option "C" allows to center the header 
  vector<vector<Double_t>> liseEdep  = { /*[KeV]*/
    {4784.4, 6002.4, 7686.9},
    {4479.8, 5742.4, 7464.3},
    {4160.2, 5471.9, 7236.5},
    {3994.8, 5332.7, 7120.7},
    {3901.2, 5253.6, 7055.3}
  };
  // G4EmCalculator energy deposites calculated 
  // with dead layer thickness {0., 1., 2., 3., 4., 5., 6} [um]
  // Integration step 1e-5 [nm]
  vector<vector<Double_t>> emCalcEdep = { /*[KeV]*/
    {4784.40, 6002.40, 7686.90},
    {4634.73, 5873.25, 7577.70},
    {4482.01, 5742.23, 7467.42},
    {4326.04, 5609.24, 7356.02},
    {4166.59, 5474.19, 7243.46},
    {4003.37, 5336.96, 7129.70},
    {3836.11, 5197.45, 7014.70}
  };
  vector<Double_t> dead_reg6pts = {0., 1., 2., 3., 4., 5., 6};
  vector<Double_t> dead_lise = {0., 2., 4., 5., 5.67};
  vector<Double_t> eta_emCalcEdep(emCalcEdep.size());
  vector<Double_t> eta_lise(liseEdep.size());
  auto mgEta = new TMultiGraph();
  TString mgTitle = "d(eta)";
  mgEta->SetTitle(mgTitle);
  for (int i = 0; i < eta_emCalcEdep.size(); i++) {
    eta_emCalcEdep[i] = (emCalcEdep[i][2] - emCalcEdep[i][1]) 
                         / (emCalcEdep[i][2] - emCalcEdep[i][0]); 
  }
  for (int i = 0; i < eta_lise.size(); i++) {
    eta_lise[i] = (liseEdep[i][2] - liseEdep[i][1]) 
                   / (liseEdep[i][2] - liseEdep[i][0]); 
  }

  addLineMultiGraph(mgEta, &eta_emCalcEdep[0], &dead_reg6pts[0], 
                    kBlue,  22, eta_emCalcEdep.size(), "EmCalculator");
  addLineMultiGraph(mgEta, &eta_lise[0], &dead_lise[0],
                    kGreen,  23, dead_lise.size(), "Lise");
  gLegend->Draw();
  mgEta->GetXaxis()->SetTitle("eta = (E3-E2)/(E3-E1),\n [1]");
  mgEta->GetYaxis()->SetTitle("d, [um]");

  TFitResultPtr fitRes = mgEta->Fit("pol2","S");
  cout << "Fit params " << fitRes->Parameter(0) 
       << ", " << fitRes->Parameter(1) 
       << ", " << fitRes->Parameter(2) << endl;
}
