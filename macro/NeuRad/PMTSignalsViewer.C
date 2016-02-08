struct Signal {
  double x[100000];
  double y[100000];
  int nb;
};

void PMTSignalsViewer(){
  TString fdigiName = "digi.root";

  TFile fdigi(fdigiName);
  TFile fsim("sim.root");


  TTree* digitree = (TTree*) fdigi.Get("cbmsim");
  TTree* Tsim = (TTree*) fsim.Get("cbmsim");
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",1000);
  TBranch* digi_trbr = digitree->GetBranch("NeuRadPMTSignal");
  digi_trbr->SetAddress(&pmtSignals);
   TClonesArray* points = new TClonesArray("ERNeuRadPoint",100);
  Tsim->SetBranchAddress("NeuRadPoint",&points);
for (int iEvent = 0; iEvent < Tsim->GetEntries(); iEvent++){	
  
  digi_trbr->GetEntry(iEvent);

 
  Tsim->GetEntry(iEvent);

  Signal signal_mas[100];
  TGraph* signals_gr[100];
  //Signal separateSignals_mas[1000][100];
  //TGraph* separateSignals_gr[1000][100];
  
  TCanvas *c1 = new TCanvas("NeuRad PMTSignal","NeuRad PMTSignal",200,10,1000,500);
  c1->Divide(2,2,0,0);
  int realSignals = 2;
  cout << "Signals Count" << pmtSignals->GetEntriesFast() << endl;
  for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast()/2; iSignal++){

    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
    
    if (!signal->Exist())
      continue;

    c1->cd(3);
    gPad->SetLeftMargin(0.1);
    TMultiGraph *mg = new TMultiGraph();
    cout << iSignal << endl;
    map<double, double> sPointsMap = signal->GetSignalPoints();
    signal_mas[iSignal].nb = 0;
    for (map<double,double>::iterator it = sPointsMap.begin(); it != sPointsMap.end(); it++){
      //Обрабатываем суммарный сигнал
      signal_mas[iSignal].x[signal_mas[iSignal].nb] = it->first;
      signal_mas[iSignal].y[signal_mas[iSignal].nb] = it->second;
      /*cout <<  iSignal << " " << signal_mas[iSignal].nb <<  " " << 
                signal_mas[iSignal].x[signal_mas[iSignal].nb] << " " <<
                signal_mas[iSignal].y[signal_mas[iSignal].nb] << endl;
       */         
      signal_mas[iSignal].nb++;
    }
    TGraph* gr = signals_gr[iSignal];
    gr = new TGraph(signal_mas[iSignal].nb,signal_mas[iSignal].x, signal_mas[iSignal].y);
    //gr->Draw("AL*");
    gr->SetLineWidth(5);
    mg->Add(gr);
      //Обрабатываем сигналы от отдельных FiberPoint
      /*for (int issig = 0; issig < signal->GetNumberOfSeparatesSignals(); issig++){
        map<double, double> ssPointsMap = signal->GetSeparateSignal(issig);
        separateSignals_mas[iSignal][issig].nb = 0;
        
        for (map<double,double>::iterator sit = ssPointsMap.begin(); sit != ssPointsMap.end(); sit++){
          Int_t nb = separateSignals_mas[iSignal][issig].nb;
          separateSignals_mas[iSignal][issig].x[nb] = sit->first;
          separateSignals_mas[iSignal][issig].y[nb] = sit->second;
          cout <<  iSignal << " " << issig << " " << separateSignals_mas[iSignal][issig].nb
                <<  " " << 
                separateSignals_mas[iSignal][issig].x[nb] << " " <<
                separateSignals_mas[iSignal][issig].y[nb]<< endl;
          separateSignals_mas[iSignal][issig].nb++;
        }
        separateSignals_gr[iSignal][issig] = new TGraph(separateSignals_mas[iSignal][issig].nb,separateSignals_mas[iSignal][issig].x,
                            separateSignals_mas[iSignal][issig].y);
        ///gr1->Draw("AL*");
        separateSignals_gr[iSignal][issig]->SetLineColorAlpha(3, 1.);
        mg->Add(separateSignals_gr[iSignal][issig]);
        cout<<separateSignals_mas[iSignal][issig].nb<<endl;
      }*/
      //TString title = "NeuRad PMT Signal in fiber number " + iSignal;
      //mg->SetTitle(title);
      mg->Draw("AL");
      mg->GetXaxis()->SetTitle("Time [ns]");
      mg->GetYaxis()->SetTitle("U [mV]");
      gPad->Modified();
      Int_t jSignal = iSignal+pmtSignals->GetEntriesFast()/2;
	ERNeuRadPMTSignal* signal2 = (ERNeuRadPMTSignal*)pmtSignals->At(jSignal);
    
    if (!signal2->Exist())
      continue;

    c1->cd(4);
    gPad->SetLeftMargin(0.1);
    TMultiGraph *mg = new TMultiGraph();
    map<double, double> sPointsMap = signal2->GetSignalPoints();
    signal_mas[jSignal].nb = 0;
    for (map<double,double>::iterator it = sPointsMap.begin(); it != sPointsMap.end(); it++){
      //Обрабатываем суммарный сигнал
      signal_mas[jSignal].x[signal_mas[jSignal].nb] = it->first;
      signal_mas[jSignal].y[signal_mas[jSignal].nb] = it->second;        
      signal_mas[jSignal].nb++;
    }
    TGraph* gr = signals_gr[jSignal];
    gr = new TGraph(signal_mas[jSignal].nb,signal_mas[jSignal].x, signal_mas[jSignal].y);
    gr->SetLineWidth(5);
    mg->Add(gr);
      mg->Draw("AL");
      mg->GetXaxis()->SetTitle("Time [ns]");
      mg->GetYaxis()->SetTitle("U [mV]");
      gPad->Modified();

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
  }
}
}
