void Ana(){
  TFile *digi = new TFile("digi.root");
	TFile *out = new TFile("out.root", "recreate");
  
  TTree *T = (TTree*)digi->Get("cbmsim");
  
  TClonesArray *fpoints = new TClonesArray("ERNeuRadFiberPoint");
  T->SetBranchAddress("NeuRadFiberPoint",&fpoints);
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",1000);
  T->SetBranchAddress("NeuRadPMTSignal",&pmtSignals);
 
  TH1F* hOPEtime = new TH1F("OPE time", "OPE time",100,0.,20.);
  
  for (int iEvent = 0; iEvent < 5000; iEvent++){
    T->GetEntry(iEvent);
    
    for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast(); iSignal++){
      ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
      if (signal->Exist()){
        //Находим начало сигнала
       
        int fpindex = signal->GetLink(0).GetIndex();
        double startTime = ((ERNeuRadFiberPoint*) fpoints->At(fpindex))->AnodeTime();
         map<double, double> sPointsMap = signal->GetSignalPoints();
         
          cout << "! " <<  signal->GetNLinks() << " " << sPointsMap.begin()->first<< " " << startTime << endl;
        startTime =   sPointsMap.begin()->first;
        //Перебираем все файбер поинты и находи одноэлектронные
        int nope = 0;
        for (int ifp = 0; ifp < signal->GetNLinks(); ifp++){
          ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*) fpoints->At(signal->GetLink(ifp).GetIndex());
          if (fpoint->OnePE()){
            cout << fpoint->AnodeTime() - startTime << endl;
            hOPEtime->Fill(fpoint->AnodeTime() - startTime);
            
          }
        }
      }
    }
  }
  TCanvas *c1 = new TCanvas("c1","One pe anode time",10,10,1000,500);
  hOPEtime->SetTitle("One pe anode time, 500 MeV neutron, 5000ev");
  hOPEtime->GetXaxis()->SetTitle("[ns]");
  hOPEtime->Draw();
  c1->SaveAs("OnePEAnodeTime500MeV.png");
}