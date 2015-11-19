struct Signal {
  double x[100];
  double y[100];
  int nb;
};

void PMTSignalsViewer(){
  int nEvent = 0;
	
  TString fdigiName = "digi.root";

  TFile fdigi(fdigiName);
  
  TTree* digitree = (TTree*) fdigi.Get("cbmsim");
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",1000);
  TBranch* digi_trbr = digitree->GetBranch("NeuRadPMTSignal");
  digi_trbr->SetAddress(&pmtSignals);
  digi_trbr->GetEntry(nEvent);

  Signal signal_mas[1000];
 
  for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
    map<double, double> sPointsMap = signal->GetSignalPoints();
    map<double,double>::iterator it;
    signal_mas[iSignal].nb = 0;
    for (it = sPointsMap.begin(); it != sPointsMap.end(); it++){
      signal_mas[iSignal].x[signal_mas[iSignal].nb] = it->first;
      signal_mas[iSignal].y[signal_mas[iSignal].nb] = it->second;
      cout <<  iSignal << " " << signal_mas[iSignal].nb <<  " " << 
                signal_mas[iSignal].x[signal_mas[iSignal].nb] << " " <<
                signal_mas[iSignal].y[signal_mas[iSignal].nb] << endl;  
      signal_mas[iSignal].nb++;
      
    } 
  }
  
  TCanvas *c1 = new TCanvas("NeuRad PMTSignal","NeuRad PMTSignal",200,10,700,500);
  TGraph* gr = new TGraph(signal_mas[54].nb,signal_mas[54].x, signal_mas[54].y);
  gr->SetTitle("NeuRad PMT Signal in fiber number 54");
  gr->GetXaxis()->SetTitle("Time [ns]");
  gr->GetYaxis()->SetTitle("U [mV]");
  gr->Draw("AL*");
}