/*
"однофайберный" режим. На каждом пикселе каждого ФЭУ ставится порог на интеграл импульса за промежуток 5 нс. 

Учитываются только те волокна, где с обоих концов преодолен этот порог. Далее отбираются только события, где порог преодолен только в одном файбере 
(но с двух сторон). Гистограммируется расстояние в плоскости XY между точкой входа нейтрона и центром единтсвенноо сработавшего файбера.

Значения порога должны соответствовать средним значениям 2, 5, 12, 30 фотоэлектронов. Мы не считаем штуки фотоэлектронов, а смотри интеграл сигнала с учетом всех разбросов!

Обязательно записывать в углу гистораммы сколько событий из миллиона (или хотя бы 100 000) в нее попали - это будет указание на эффективность.
*/

void res1(TString pngdir, TString pngname, int pe,TString fsimName, TString fdigiName){
  
  
  //Координаты центров файберов. все это уйдет в NeuRadSetup
  int fb[64];
  double fb_x[64];
  double fb_y[64];
  Double_t fiber_X = 0.6;   //cm
  Double_t fiber_Y = 0.6;   //cm 
  fiber_X /= 2.;
  fiber_Y /= 2.;
  Int_t fibers_in_boundle_X_Nb = 8;
  Int_t fibers_in_boundle_Y_Nb = 8;
  Double_t boundle_X = fiber_X * fibers_in_boundle_X_Nb;
  Double_t boundle_Y = fiber_Y * fibers_in_boundle_Y_Nb;
  Int_t i_fiber = 0;
  for (Int_t i_Y_fiber = 0; i_Y_fiber < 8; i_Y_fiber++){
    for (Int_t i_X_fiber = 0; i_X_fiber < 8; i_X_fiber++){
      fb_x[i_fiber] = boundle_X - fiber_X*2*(i_X_fiber)-fiber_X;
      fb_y[i_fiber] = boundle_Y - fiber_Y*2*(i_Y_fiber)-fiber_Y;
      i_fiber++;
    }
  }
  
  //TString fdigiName = "digi500MeV100000ev.root";
  //TString fsimName = "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000ev.root";
  TFile *out = new TFile("out.root");
  
  TFile fdigi(fdigiName);
  TFile fsim(fsimName);
  
  TTree* Tsim = (TTree*) fsim.Get("cbmsim");
  TTree* T = (TTree*) fdigi.Get("cbmsim");
  
  TClonesArray* steps = new TClonesArray("ERNeuRadStep",10000);
  Tsim->SetBranchAddress("NeuRadStep",&steps);
   
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",1000);
  T->SetBranchAddress("NeuRadPMTSignal",&pmtSignals);
  
  TClonesArray* fpoints = new TClonesArray("ERNeuRadFiberPoint",1000);
  T->SetBranchAddress("NeuRadFiberPoint",&fpoints);
  
  Double_t fiberThreshold = pe*0.5*(0.1 + 1.3)*5.;
  Int_t oneFbEv = 0;
  
  TString title;
  title.Form("XY distance, One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hdxy = new TH1F("hdxy", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%f",10*0.6/200);
  hdxy->GetXaxis()->SetTitle("XY disance [cm]");
  hdxy->GetYaxis()->SetTitle(title.Data());
  
  title.Form("X distance, One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hdx = new TH1F("hdx", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%f",10*0.6/200);
  hdx->GetXaxis()->SetTitle("X disance [cm]");
  hdx->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Y distance, One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hdy = new TH1F("hdy", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%f",10*0.6/200);
  hdy->GetXaxis()->SetTitle("Y disance [cm]");
  hdy->GetYaxis()->SetTitle(title.Data());
  
  /* Амплитуды однофайберных событий в фотоэлектронах */
  title.Form("Front fiber A in [pe], One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hfApe = new TH1F("hfApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hfApe->GetXaxis()->SetTitle("Front fiber A [pe]");
  hfApe->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Back fiber A in [pe], One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hbApe = new TH1F("hbApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hbApe->GetXaxis()->SetTitle("Back fiber A [pe]");
  hbApe->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Sum Fiber A in [pe], One fiber events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hApe = new TH1F("hApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hApe->GetXaxis()->SetTitle("Sum Fiber A [pe]");
  hApe->GetYaxis()->SetTitle(title.Data());  
  
  title.Form("Sum Module A in [pe], all events, neutron, Ekin=500 MeV, %d events, fiber threshold %d pe",T->GetEntries(), pe);
  TH1F* hAmod = new TH1F("hAmod", title.Data(),500,0.,5000);
  title.Form("Counts/%d",5000/500);
  hAmod->GetXaxis()->SetTitle("Sum Module A [pe]");
  hAmod->GetYaxis()->SetTitle(title.Data());  
  
  
  for (int iEvent = 0; iEvent < 100/*T->GetEntries()*/; iEvent++){
    //cout << iEvent << endl;
    Double_t modA = 0.;
    T->GetEntry(iEvent);
    Tsim->GetEntry(iEvent);
    int signals[64][2], jsig[64];
    for (int i = 0; i<64; i++){
      fb[i] = 0;
      jsig[i] = 0;
      signals[i][0] = 0;
      signals[i][1] = 0;
    }
    
    for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast(); iSignal++){
        ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
        if (!signal->Exist())
          continue;
        
        for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
          ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(signal->GetLink(iLink).GetIndex());
          modA += fpoint->PhotoElCount();
        }
        
        if (signal->GetFirstInteg(25.) > fiberThreshold){
          fb[signal->FiberIndex()]++;
          signals[signal->FiberIndex()][jsig[signal->FiberIndex()]++] = iSignal;
        }
    }
    hAmod->(modA);
    if (steps->GetEntriesFast() == 0)
      continue;
    ERNeuRadStep* step = (ERNeuRadStep*)steps->At(0);
    //Определяемся с количеством прошедших порог
    Int_t thFb[100];
    Int_t thFbN = 0;
    
    for (int i = 0; i<64; i++){
      if (fb[i] == 2)
        thFb[thFbN++] = i;
    }
    
    //Работаем только с однофайберными
    if (thFbN == 1){
      oneFbEv++;
      hdxy->Fill(TMath::Sqrt( (step->GetX()-fb_x[thFb[0]])*(step->GetX()-fb_x[thFb[0]]) +
                              (step->GetY()-fb_y[thFb[0]])*(step->GetY()-fb_y[thFb[0]])) );
      hdx->Fill(TMath::Abs(step->GetX()-fb_x[thFb[0]]));
      hdy->Fill(TMath::Abs(step->GetY()-fb_y[thFb[0]]));
      ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(signals[thFb[0]][0]);
      int fsum_pe=0;
      for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
        ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(signal->GetLink(iLink).GetIndex());
        fsum_pe += fpoint->PhotoElCount();
      }
      hfApe->Fill(fsum_pe);
      signal = (ERNeuRadPMTSignal*)pmtSignals->At(signals[thFb[0]][1]);
      int bsum_pe=0;
      for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
        ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(signal->GetLink(iLink).GetIndex());
        bsum_pe += fpoint->PhotoElCount();
      }
      hbApe->Fill(bsum_pe);
      hApe->Fill(fsum_pe+bsum_pe);
    }
    
  }
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,500);
  c1->SetLogy();
  c1->SetGrid();
  
  hdxy->Draw();
  gSystem->ProcessEvents();
  title.Form("distXY_%s_%dev_%dpe.png",pngname.Data(), T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdx->Draw();
  gSystem->ProcessEvents();
  title.Form("distX_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdy->Draw();
  gSystem->ProcessEvents();
  title.Form("distY_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hfApe->Draw();
  gSystem->ProcessEvents();
  title.Form("frontFiberA[pe]_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hbApe->Draw();
  gSystem->ProcessEvents();
  title.Form("backFiberA[pe]_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hApe->Draw();
  gSystem->ProcessEvents();
  title.Form("sumFiberA[pe]_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hAmod->Draw();
  gSystem->ProcessEvents();
  title.Form("moduleA[pe]_%s_%dev_%dpe.png",pngname.Data(),T->GetEntries(), pe);
  c1->SaveAs(title.Data());
  
  
  
}