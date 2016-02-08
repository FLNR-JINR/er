struct PIDGraph {
  double ly[100000];
  double el[100000];
  int pid;
  int nb;
};


void LYvsEloss(){
  TFile *sim = new TFile("sim250.root");
	TFile *out = new TFile("out.root", "recreate");
  
  TTree *T = (TTree*)sim->Get("cbmsim");
	TClonesArray *points = new TClonesArray("ERNeuRadPoint");
	T->SetBranchAddress("NeuRadPoint",&points);
  
  Long64_t nEvents = T->GetEntries();
  
  const double bin_min = 0.8;
	const double bin_max = 1.;
	const double bin = 0.001;
	const int nbins = int((bin_max-bin_min)/bin); 
	
	TH1F* hPointsLYvsEL= new TH1F("500MeV. Point ly vs eloss",
			"Neutron. EKin=500MeV. Light Yield vs ELoss. 10000 events",nbins,bin_min,bin_max);
  hPointsLYvsEL->GetXaxis()->SetTitle("LY vs ELoss");
  TString str = "Counts/"; str += bin;
  hPointsLYvsEL->GetYaxis()->SetTitle( str.Data());
  
  Int_t pids[100];
  Int_t pid_nb = 0;
  
  //Определяем какие есть PID
  for (Long64_t iEvent=0; iEvent < nEvents; iEvent++) {
		points->Clear();
		T->GetEntry(iEvent);
    Int_t pointsCount = points->GetEntriesFast();
    for (int iPoint = 0; iPoint < pointsCount; iPoint++){
      ERNeuRadPoint *point = (ERNeuRadPoint*)points->At(iPoint);
      Int_t pid = point->GetPID();
      //Ищем есть ли у нас уже такой PID
      bool found = false;
      for (int iPid =0; iPid < pid_nb; iPid++){
        if(pids[iPid] == pid)
          found = true;
      }
      if (!found)
        pids[pid_nb++] = pid;
    }
  }
  
  for (int i =0; i < pid_nb; i++){
    cout << pids[i] << endl;
  }
  
  //Инициализируем графики под каждый PID;
  PIDGraph* grs = new PIDGraph[100];
  for (int iPid = 0; iPid < pid_nb; iPid++){
    grs[iPid].pid = pids[iPid];    
  }
  

  //Заполняем графики точками
  for (Long64_t iEvent=0; iEvent < nEvents; iEvent++) {
		points->Clear();
		T->GetEntry(iEvent);
    Int_t pointsCount = points->GetEntriesFast();
    for (int iPoint = 0; iPoint < pointsCount; iPoint++){
      ERNeuRadPoint *point = (ERNeuRadPoint*)points->At(iPoint);
      //находим куда нужно добавить точку
      int pid = point->GetPID();
      for(int iPid = 0; iPid < pid_nb; iPid++){
        if (pid == grs[iPid].pid){
          //Добавляем точку к графику
          grs[iPid].ly[grs[iPid].nb] = point->GetLightYield();
          grs[iPid].el[grs[iPid].nb] = point->GetEnergyLoss();
          grs[iPid].nb++;
          
          break;
        }
      }
    }
  }
  //Создаем мультиграф
  TMultiGraph *mg = new TMultiGraph();
  TLegend *l =new TLegend(0.6,0.7,0.99,0.9);
  l->SetTextFont(62);
  l->SetTextSize(0.03868472);
  l->SetMargin(0.4);
  l->SetFillColor(0);
  cout << "pid_nb=" << pid_nb << endl;
  int ic = 0;
  for (int iPid = 0; iPid < pid_nb; iPid++){
    if (grs[iPid].nb < 100)
      continue;
    TGraph* gr = new TGraph(grs[iPid].nb, grs[iPid].el, grs[iPid].ly);
    TString t;
    cout << grs[iPid].nb;
    t.Form("pid = %d (%d)",grs[iPid].pid, grs[iPid].nb);
    gr->SetTitle(t.Data());
    if (ic+2 == 5) ic++;
    gr->SetMarkerColor(ic+2);
    ic++;
    mg->Add(gr);
  }
  TCanvas *c1 = new TCanvas("c1","Neutron. EKin=500MeV. Light Yield vs ELoss. 10000 events",10,10,1800,1000);
  
  mg->Draw("AP");
  mg->GetXaxis()->SetTitle("ELoss [GeV]");
  mg->GetXaxis()->SetRangeUser(0.,0.03);
  mg->GetYaxis()->SetRangeUser(0.,0.02);
  mg->GetYaxis()->SetTitle("Light Yield [GeV]");
  //Добавляем легенду
  l->AddEntry(mg);
  l = c1->BuildLegend();
  l->SetBBoxY1(100);
  TIter next(l->GetListOfPrimitives());
  TLegendEntry *entry;
  ic = 0;
  while (( entry = (TLegendEntry *)next() )){
    entry->SetOption("p");
    if (ic+2 == 5) ic++;
    entry->SetTextColor(ic+2);
    //entry->SetTextSize(3);
    
    ic++;
  }

  //l->SetEntryOption("p");
  gPad->Modified();
  c1->SaveAs("LYvsELoss250MeV.png");
  out->cd();
  mg->Write();
  /*for (int iPid=0; iPid < pid_nb; iPid++){
    c1->Clear();
    TGraph* gr = new TGraph(grs[iPid].nb, grs[iPid].el, grs[iPid].ly);
    gr->Draw("AP");
    gr->GetXaxis()->SetTitle("ELoss [GeV]");
    gr->GetYaxis()->SetTitle("Light Yield [GeV]");
    
    gPad->Modified();
    TString file;
    file.Form("LYvsELoss250MeV%d.png", grs[iPid].pid);
    c1->SaveAs(file.Data());
  }
  */
}
