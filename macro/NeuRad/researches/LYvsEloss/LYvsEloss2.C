//Разбивка протонных событий

struct ProtonPointNbGr{
  double ly[100000];
  double el[100000];
  int nb;
  int pnb;//number of proton point in event
};


void LYvsEloss2(){
  TFile *sim = new TFile("sim.root");
	TFile *out = new TFile("out.root", "recreate");
  
  TTree *T = (TTree*)sim->Get("cbmsim");
	TClonesArray *points = new TClonesArray("ERNeuRadPoint");
	T->SetBranchAddress("NeuRadPoint",&points);
  
  Long64_t nEvents = 100;//T->GetEntries();
 	
  ProtonPointNbGr* grs = new ProtonPointNbGr[3];
  Int_t grs_nb = 0;
  for (Long64_t iEvent=0; iEvent < nEvents; iEvent++) {
		points->Clear();
		T->GetEntry(iEvent);
    Int_t pointsCount = points->GetEntriesFast();
    Int_t protonsPointsCount = 0;
    ERNeuRadPoint* firstPoint, *lastPoint; 
    for (int iPoint = 0; iPoint < pointsCount; iPoint++){
      ERNeuRadPoint *point = (ERNeuRadPoint*)points->At(iPoint);
      //находим куда нужно добавить точку
      int pid = point->GetPID();
      if (pid == 2212){
        protonsPointsCount++;
        if (protonsPointsCount == 1)
          firstPoint = point;
        else 
          lastPoint = point;
      }
    }
    if (protonsPointsCount == 1){
      Int_t nb = grs[0].nb;
      grs[0].ly[nb] = firstPoint->GetLightYield();
      grs[0].el[nb] = firstPoint->GetEnergyLoss();
      grs[0].nb++;
    }
    else if (protonsPointsCount > 1){
      Int_t nb = grs[1].nb;
      grs[1].ly[nb] = firstPoint->GetLightYield();
      grs[1].el[nb] = firstPoint->GetEnergyLoss();
      grs[1].nb++;
      nb = grs[2].nb;
      if (TMath::Abs(lastPoint->GetLightYield()-lastPoint->GetEnergyLoss())/lastPoint->GetEnergyLoss() < 0.1){
        cout << iEvent << " " <<  lastPoint->GetEnergyLoss() << " " << lastPoint->GetLightYield() << endl;
        grs[2].ly[nb] = lastPoint->GetLightYield();
        grs[2].el[nb] = lastPoint->GetEnergyLoss();
        grs[2].nb++;
      }
    }
  }
  //Создаем мультиграф
  TMultiGraph *mg = new TMultiGraph();
  
  TGraph* gr = new TGraph(grs[0].nb, grs[0].el, grs[0].ly);
  TString title;
  title.Form("One fibers events points (%d)", grs[0].nb);
  gr->SetTitle(title.Data());
  gr->SetMarkerColor(2);
  mg->Add(gr);
  
  TGraph* gr1 = new TGraph(grs[1].nb, grs[1].el, grs[1].ly);
  title.Form("Many fibers events first points (%d)", grs[1].nb);
  gr1->SetTitle(title.Data());
  gr1->SetMarkerColor(3);
  mg->Add(gr1);
  
  TGraph* gr2 = new TGraph(grs[2].nb, grs[2].el, grs[2].ly);
  title.Form("Many fibers events last points (%d)", grs[2].nb);
  gr2->SetTitle(title.Data());
  gr2->SetMarkerColor(4);
  gr2->SetMarkerSize(5);
  mg->Add(gr2);
  
  TCanvas *c1 = new TCanvas("c1","Neutron. EKin=500MeV. Light Yield vs ELoss. 10000 events",10,10,1000,1000);
  
  mg->Draw("AP");
  mg->GetXaxis()->SetTitle("ELoss [GeV]");
  mg->GetXaxis()->SetRangeUser(0.,0.02);
  mg->GetYaxis()->SetRangeUser(0.,0.02);
  mg->GetYaxis()->SetTitle("Light Yield [GeV]");
  //Добавляем легенду
  TLegend* l->AddEntry(mg);
  l = c1->BuildLegend();
  //l->SetBBoxY1(100);
  TIter next(l->GetListOfPrimitives());
  TLegendEntry *entry;
  ic = 0;
  while (( entry = (TLegendEntry *)next() )){
    entry->SetOption("p");
    entry->SetTextColor(ic+2);
    //entry->SetTextSize(3);
    ic++;
  }

  //l->SetEntryOption("p");
  gPad->Modified();
  //c1->SaveAs("MFProtonsEventsLYvsELoss250MeV.png");
  //out->cd();
  //mg->Write();
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
