void PMTSignalsViewerTS(){
  TFile fdigi("digiTS.root");
  TFile fsim("sim.root");

  TTree* digitree = (TTree*) fdigi.Get("cbmsim");
  TTree* Tsim = (TTree*) fsim.Get("cbmsim");
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignalLP",1000);
  TBranch* digi_trbr = digitree->GetBranch("NeuRadPMTSignal");
  digi_trbr->SetAddress(&pmtSignals);
  TClonesArray* points = new TClonesArray("ERNeuRadPoint",100);
  Tsim->SetBranchAddress("NeuRadPoint",&points);
  cerr << Tsim->GetEntries() << endl;

//for (int iEvent = 0; iEvent < Tsim->GetEntries(); iEvent++){  
  int iEvent = 1;
  digi_trbr->GetEntry(iEvent);

  Tsim->GetEntry(iEvent);
  
  TCanvas *c1 = new TCanvas("NeuRad PMTSignal","NeuRad PMTSignal",200,10,1000,500);
  //c1->Divide(2,2,0,0);
  int pe_count = 0;
  int realSignals = 2;

  double time[100000];
  double U[100000];
  int nb = 0;

  cout << "Signals Count = " << pmtSignals->GetEntriesFast() << endl;
  //for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast()/2; iSignal++){
  int iSignal = 0;
  ERNeuRadPMTSignalLP* signal = (ERNeuRadPMTSignalLP*)pmtSignals->At(iSignal);
  cerr << signal->Exist() << endl;
  //if (!signal->Exist())
  // /  continue;

  //c1->cd(3);
  gPad->SetLeftMargin(0.1);
  TMultiGraph *mg = new TMultiGraph();
  cout << iSignal << endl;
  map<double, double> sPointsMap = signal->GetSignalPoints();
  
  for (map<double,double>::iterator it = sPointsMap.begin(); it != sPointsMap.end(); it++){

    time[nb] = it->first;
    U[nb] = it->second;        
    nb++;
  }
  cout << "I = " << signal->GetFirstInteg(100.) << endl;
  TGraph* gr = new TGraph(nb,time, U);
  gr->SetLineWidth(5);
  mg->Add(gr);
  //Обрабатываем сигналы от отдельных FiberPoint
  TGraph** gr_sigs = new TGraph*[signal->GetNumberOfSeparatesSignals()];
  for (int issig = 0; issig < signal->GetNumberOfSeparatesSignals(); issig++){
        map<double, double> ssPointsMap = signal->GetSeparateSignal(issig);
        nb = 0;
        for (map<double,double>::iterator sit = ssPointsMap.begin(); sit != ssPointsMap.end(); sit++){
          time[nb] = sit->first;
          U[nb] = sit->second;
          pe_count += U[nb]/5.;
          nb++;
        }
        gr_sigs[issig] = new TGraph(nb,time,U);
        gr_sigs[issig]->SetLineColorAlpha(3, 1.);
        mg->Add(gr_sigs[issig]);
  }
  mg->Draw("AL");
  mg->GetXaxis()->SetTitle("Time [ns]");
  mg->GetYaxis()->SetTitle("U [mV]");
  cerr << pe_count << endl;
  gPad->Modified();
  /*  Int_t jSignal = iSignal+pmtSignals->GetEntriesFast()/2;
  /* ERNeuRadPMTSignalLP* signal2 = (ERNeuRadPMTSignalLP*)pmtSignals->At(jSignal);
    
    if (!signal2->Exist())
      continue;

    c1->cd(4);
    gPad->SetLeftMargin(0.1);
    mg = new TMultiGraph();
    sPointsMap = signal2->GetSignalPoints();
    nb = 0;
    for (map<double,double>::iterator it = sPointsMap.begin(); it != sPointsMap.end(); it++){
      time[nb] = it->first;
      U[nb] = it->second;        
      nb++;
    }
    gr = new TGraph(nb,time, U);
    gr->SetLineWidth(5);
    mg->Add(gr);
    mg->Draw("AL");
    mg->GetXaxis()->SetTitle("Time [ns]");
    mg->GetYaxis()->SetTitle("U [mV]");
    gPad->Modified();*/
  //}
//}

/*
    c1->cd(1);
    TPaveText *pt = new TPaveText(.05,.1,.95,.8);
    TString textLine;
    textLine.Form("Fiber index: %d", signal->FiberIndex());
    pt->AddText(textLine);
    
    pt->AddText("Points:");
    Double_t Z1 = -100.;
    bool good = false;
    bool start = true;
    for (int iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++){
        ERNeuRadPoint* p = (ERNeuRadPoint*)points->At(iPoint);

        if (p->GetFiberInBundleNb() == signal->FiberIndex()){
          textLine.Form("z=%f,ed=%f,ly=%f,t=%f", p->GetZ(),p->GetEnergyLoss(),p->GetLightYield(), p->GetTime());
          pt->AddText(textLine);
          if (start){
            Z1 = p->GetZ();
            start = false;
          }
          if ((p->GetZ() - Z1) > 10.)
            good = true;
        }
      }
      pt->Draw();
      c1->cd(2);
      TPaveText *pt2 = new TPaveText(.05,.1,.95,.8);
      textLine.Form("Front pe count: %d", signal->GetNumberOfSeparatesSignals());
      pt2->AddText(textLine);
      textLine.Form("Fornt Integral: %f", signal->GetFirstInteg(50));
      pt2->AddText(textLine);
      textLine.Form("Back pe count: %d", signal2->GetNumberOfSeparatesSignals());
      pt2->AddText(textLine);
      textLine.Form("Back Integral: %f", signal2->GetFirstInteg(50));
      pt2->AddText(textLine);
      pt2->Draw();
      textLine.Form("%devSignalsMap%d.png", iEvent, signal->FiberIndex());
      if (good)
        c1->SaveAs(textLine);
    }*/
}
