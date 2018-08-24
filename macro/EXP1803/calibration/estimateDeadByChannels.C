auto mg  = new TMultiGraph();
std::vector<TGraph*> gr_dVect;

void addLineMultiGraph(const Double_t *x, const Double_t *y, int colors, int pointsCount) {
  gr_dVect.push_back(new TGraph(pointsCount, x, y));
  gr_dVect.back()->SetLineColor(colors);
  gr_dVect.back()->SetMarkerColor(colors);
  gr_dVect.back()->SetLineWidth(2);
  gr_dVect.back()->SetMarkerSize(1.1);
  gr_dVect.back()->SetMarkerStyle(21);
  mg->Add(gr_dVect.back());
  mg->Draw("APL");

}

void estimateDeadByChannels() {
  vector<vector<Double_t>> geantEdep = { {4784.4000, 6002.4000, 7686.9000},
                                         {4481.5893, 5740.8789, 7465.1015},
                                         {4167.1732, 5473.8416, 7241.6313},
                                         {4000.7082, 5334.8245, 7127.4266},
                                         {3910.9122, 5257.3666, 7065.1708}
                                       };
  vector<vector<Double_t>> liseEdep  = { {4784.4000, 6002.4000, 7686.9000},
                                         {4479.8000, 5742.4000, 7464.3000},
                                         {4160.2000, 5471.9000, 7236.5000},
                                         {3994.8000, 5332.7000, 7120.7000},
                                         {3901.2000, 5253.6000, 7055.3000}
                                       };
  vector<vector<Double_t>> expChan   = { {206.399, 265.370, 346.331}, /*TSpectrum - dir*/
                                         {206.500, 265.800, 346.100}, /*Mean - dir*/
                                         {201.402, 260.373, 342.333}, /*TSpectrum - rot*/
                                         {201.000, 260.800, 342.200}  /*Mean - rot*/
                                       }; 

  TCanvas* eLdepCanvas = new TCanvas("edepCanvas");

  vector<Double_t> deadThickPoints = {0., 2., 4., 5., 5.56};
  vector<Double_t> deadValue(5);
  vector<vector<Double_t>> expPointTSpec = {{(expChan[0][2] - expChan[0][1]) / (expChan[0][2] - expChan[0][0])},
                                            {(expChan[1][2] - expChan[1][1]) / (expChan[1][2] - expChan[1][0])},
                                            {(expChan[2][2] - expChan[2][1]) / (expChan[2][2] - expChan[2][0])},
                                            {(expChan[3][2] - expChan[3][1]) / (expChan[3][2] - expChan[3][0])}
                                           };
  vector<vector<Double_t>> expDeadPoint = {{1.},
                                   {2.},
                                   {3.},
                                   {4.}};
  vector<Double_t> expPointMean(1);

  // expPointTSpec[0] = (expChan[0][2] - expChan[0][1]) / (expChan[0][2] - expChan[0][0]);
  // expPointMean[0]  = (expChan[1][2] - expChan[1][1]) / (expChan[1][2] - expChan[1][0]); 

  addLineMultiGraph(&expDeadPoint[0][0], &expPointTSpec[0][0], kRed, 1);
  addLineMultiGraph(&expDeadPoint[1][0], &expPointTSpec[1][0], kGreen, 1);
  addLineMultiGraph(&expDeadPoint[2][0], &expPointTSpec[2][0], kBlue, 1);
  addLineMultiGraph(&expDeadPoint[3][0], &expPointTSpec[3][0], kPink, 1);

  for (int i = 0; i < 5; i++) {
    deadValue[i] = (liseEdep[i][2] - liseEdep[i][1]) / (liseEdep[i][2] - liseEdep[i][0]); 
  }
  addLineMultiGraph(&deadThickPoints[0], &deadValue[0], kBlack, 5);
  TLatex* latLiseLine = new TLatex(deadThickPoints[4], deadValue[4], "#splitline{Lise}{line}");

  for (int i = 0; i < 5; i++) {
    deadValue[i] = (geantEdep[i][2] - geantEdep[i][1]) / (geantEdep[i][2] - geantEdep[i][0]); 
  }
  addLineMultiGraph(&deadThickPoints[0], &deadValue[0], kOrange, 5);
  TLatex* latGeantLine = new TLatex(deadThickPoints[4], deadValue[4], "#splitline{Geant}{line}");
  
  TLatex* latDirTSpec = new TLatex(expDeadPoint[0][0], expPointTSpec[0][0], "#splitline{TSpectrum}{direct}");
  TLatex* latDirMean  = new TLatex(expDeadPoint[1][0], expPointTSpec[1][0], "#splitline{Mean}{direct}");
  TLatex* latRotTSpec = new TLatex(expDeadPoint[2][0], expPointTSpec[2][0], "#splitline{TSpectrum}{rot}");
  TLatex* latRotMean  = new TLatex(expDeadPoint[3][0], expPointTSpec[3][0], "#splitline{Mean}{rot}");
  
  TLine *lineDirTSpec  = new TLine(0., expPointTSpec[0][0], 6., expPointTSpec[0][0]);
  TLine *lineDirMean   = new TLine(0., expPointTSpec[1][0], 6., expPointTSpec[1][0]);
  TLine *lineRotTSpec  = new TLine(0., expPointTSpec[2][0], 6., expPointTSpec[2][0]);
  TLine *lineRotMean   = new TLine(0., expPointTSpec[3][0], 6., expPointTSpec[3][0]);
  
  latDirTSpec->Draw();
  latDirMean->Draw();
  latRotTSpec->Draw();
  latRotMean->Draw();
  latGeantLine->Draw();
  latLiseLine->Draw();

  lineDirTSpec->Draw();
  lineDirMean->Draw();
  lineRotTSpec->Draw();
  lineRotMean->Draw();
  // TCanvas *c1 = new TCanvas("c1");
  // TH1F h("h","test",100,-4,4);
  // h.FillRandom("gaus",10000);
  // h->Draw();
  // Float_t ymax = h.GetMaximum();
  // line->SetLineColor(kRed);
  // line->Draw();

}
