auto gLegend = new TLegend(0.1,0.7,0.48,0.9);

void addLineMultiGraph(TMultiGraph *multyGraph, const Double_t *x, const Double_t *y,
                       const int colors, const int markerStyle, 
                       const int pointsCount, const TString& name) {
  
  auto gr = new TGraph(pointsCount, x, y);
  gr->SetLineColor(colors);
  gr->SetMarkerColor(colors);
  gr->SetLineWidth(2);
  gr->SetMarkerSize(1.5);
  gr->SetMarkerStyle(markerStyle/*21*/);
  gr->SetName(name);

  multyGraph->Add(gr);
  multyGraph->Draw("APL");
  // multyGraph->Draw("AP");

  gLegend->AddEntry(gr, name,"l");
}


void compareLines () {
  gLegend->SetHeader("Legend","C"); // option "C" allows to center the header
  vector<double> elossHighE = { /*compute*/
    0.906094, 1.39092, 1.90176, 2.44373, 3.02392, 3.65266, 4.34617, 5.13268
  };  
  std::vector<double> thickCalc_dE = {8, 12, 16, 20, 24, 28, 32, 36}; // [um]

  auto canv = new TCanvas();
  auto mgLowE = new TMultiGraph();
  mgLowE->SetTitle("Alpha Start E = 7.6869 [Mev]");
  
  addLineMultiGraph(mgLowE, &elossHighE[0], &thickCalc_dE[0], kBlue, 23, elossHighE.size(), "EmCalculator");

  mgLowE->GetXaxis()->SetTitle("Eloss,\n [MeV]");
  mgLowE->GetYaxis()->SetTitle("d, [um]");
  TFitResultPtr fitRes = mgLowE->Fit("pol3","S");
  gLegend->Draw();
}
