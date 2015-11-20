struct Signal {
  double x[1000];
  double y[1000];
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
  TGraph* signals_gr[1000];
  Signal separateSignals_mas[1000][100];
  TGraph* separateSignals_gr[1000][100];
  
  for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
    
    if (!signal->Exist())
      continue;
    
    map<double, double> sPointsMap = signal->GetSignalPoints();
    signal_mas[iSignal].nb = 0;
    for (map<double,double>::iterator it = sPointsMap.begin(); it != sPointsMap.end(); it++){
      //Обрабатываем суммарный сигнал
      signal_mas[iSignal].x[signal_mas[iSignal].nb] = it->first;
      signal_mas[iSignal].y[signal_mas[iSignal].nb] = it->second;
      cout <<  iSignal << " " << signal_mas[iSignal].nb <<  " " << 
                signal_mas[iSignal].x[signal_mas[iSignal].nb] << " " <<
                signal_mas[iSignal].y[signal_mas[iSignal].nb] << endl;  
      signal_mas[iSignal].nb++;
      TGraph* gr = signals_gr[iSignal];
      gr = new TGraph(signal_mas[iSignal].nb,signal_mas[iSignal].x, signal_mas[iSignal].y);
      TString title = "NeuRad PMT Signal in fiber number " + iSignal;
      gr->SetTitle(title);
      gr->GetXaxis()->SetTitle("Time [ns]");
      gr->GetYaxis()->SetTitle("U [mV]");
      gr->Draw("AL*");
      //Обрабатываем сигналы от отдельных FiberPoint
      for (int issig = 0; issig < signal->GetNumberOfSeparatesSignals(); issig++){
        map<double, double> ssPointsMap = signal->GetSeparateSignal(issig);
        for (map<double,double>::iterator sit = sPointsMap.begin(); sit != sPointsMap.end(); sit++){
          int nb = separateSignals_mas[iSignal][issig].nb;
          separateSignals_mas[iSignal][issig].x[nb] = sit->first;
          separateSignals_mas[iSignal][issig].y[nb] = sit->second;
        }
      }
    } 
  }
  
  //TCanvas *c1 = new TCanvas("NeuRad PMTSignal","NeuRad PMTSignal",200,10,700,500);
  //c1->Divide(2);
 //c1-cd(1);
  
}