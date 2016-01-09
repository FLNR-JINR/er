/*
"однофайберный" режим.  На каждом пикселе каждого ФЭУ ставится порог на интеграл импульса за промежуток 5 нс. 

Учитываются только те волокна, где с обоих концов преодолен этот порог.   Далее отбираются только события, где порог преодолен только в одном файбере 
(но с двух сторон). Гистограммируется расстояние в плоскости XY между точкой входа нейтрона и центром единтсвенноо сработавшего файбера.

Значения порога должны соответствовать средним значениям 2, 5, 12, 30 фотоэлектронов. Мы не считаем штуки фотоэлектронов, а смотри интеграл сигнала с учетом всех разбросов!
*/

void res1(int eKin, TString pngdir, TString pngname, int pe, Double_t thEdep,TString fsimName, TString fdigiName){
  
  
  //Êîîðäèíàòû öåíòðîâ ôàéáåðîâ. âñå ýòî óéäåò â NeuRadSetup
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
  
  TClonesArray* steps = new TClonesArray("ERNeuRadStep",100);
  Tsim->SetBranchAddress("NeuRadStep",&steps);
  
  TClonesArray* points = new TClonesArray("ERNeuRadPoint",100);
  Tsim->SetBranchAddress("NeuRadPoint",&points);
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",100);
  T->SetBranchAddress("NeuRadPMTSignal",&pmtSignals);
  
  TClonesArray* fpoints = new TClonesArray("ERNeuRadFiberPoint",1000);
  T->SetBranchAddress("NeuRadFiberPoint",&fpoints);
  
  Double_t fiberThreshold = pe*0.5*(0.1 + 1.3)*5.;
  Int_t oneFbEv = 0;
  
  Long_t nEvent = T->GetEntries();
  
  TString title;
  title.Form("XY distance, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe", eKin,nEvent, pe);
  TH1F* hdxy = new TH1F("hdxy", title.Data(),400,0.,4.);
  title.Form("Counts/%.3f",4./400);
  hdxy->GetXaxis()->SetTitle("XY disance [cm]");
  hdxy->GetYaxis()->SetTitle(title.Data());
  
  title.Form("XY distance less 6mm, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hdxyLess6 = new TH1F("hdxyLess6", title.Data(),400,0.,4.);
  title.Form("Counts/%.3f",0.4/400);
  hdxyLess6->GetXaxis()->SetTitle("XY disance [cm]");
  hdxyLess6->GetYaxis()->SetTitle(title.Data());
  
  title.Form("X distance, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hdx = new TH1F("hdx", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%f",10*0.6/200);
  hdx->GetXaxis()->SetTitle("X disance [cm]");
  hdx->GetYaxis()->SetTitle(title.Data());
  
  title.Form("X distance less 6mm, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hdxLess6 = new TH1F("hdxLess6", title.Data(),100,0.,0.6);
  title.Form("Counts/%f",0.6/100);
  hdxLess6->GetXaxis()->SetTitle("X disance [cm]");
  hdxLess6->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Y distance, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hdy = new TH1F("hdy", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%f",10*0.6/200);
  hdy->GetXaxis()->SetTitle("Y disance [cm]");
  hdy->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Y distance less 6mm, One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hdyLess6 = new TH1F("hdyLess6", title.Data(),100,0.,0.6);
  title.Form("Counts/%f",0.6/100);
  hdyLess6->GetXaxis()->SetTitle("Y disance [cm]");
  hdyLess6->GetYaxis()->SetTitle(title.Data());
  
  /* Àìïëèòóäû îäíîôàéáåðíûõ ñîáûòèé â ôîòîýëåêòðîíàõ */
  title.Form("Front fiber A in [pe], One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hfApe = new TH1F("hfApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hfApe->GetXaxis()->SetTitle("Front fiber A [pe]");
  hfApe->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Back fiber A in [pe], One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hbApe = new TH1F("hbApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hbApe->GetXaxis()->SetTitle("Back fiber A [pe]");
  hbApe->GetYaxis()->SetTitle(title.Data());
  
  title.Form("Sum Fiber A in [pe], One fiber events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hApe = new TH1F("hApe", title.Data(),300,0.,3000);
  title.Form("Counts/%d",3000/300);
  hApe->GetXaxis()->SetTitle("Sum Fiber A [pe]");
  hApe->GetYaxis()->SetTitle(title.Data());  
  
  title.Form("Sum Module A in [pe], all events, neutron, Ekin=%d MeV, %d events, fiber threshold %d pe",eKin,nEvent, pe);
  TH1F* hAmod = new TH1F("hAmod", title.Data(),500,0.,5000);
  title.Form("Counts/%d",5000/500);
  hAmod->GetXaxis()->SetTitle("Sum Module A [pe]");
  hAmod->GetYaxis()->SetTitle(title.Data());  
  
  title.Form("X distance, One fiber events, neutron, Ekin=%d MeV, Edep>%f.3 KeV, %d events",eKin,thEdep, nEvent);
  TH1F* hdxEdep = new TH1F("hdxEdep", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%d",10*0.6/200);
  hdxEdep->GetXaxis()->SetTitle("X distance [cm]");
  hdxEdep->GetYaxis()->SetTitle(title.Data());  
  
  title.Form("Y distance, One fiber events, neutron, Ekin=%d MeV, Edep>%f.3 KeV, %d events",eKin,thEdep, nEvent);
  TH1F* hdyEdep = new TH1F("hdyEdep", title.Data(),200,0.,10*0.6);
  title.Form("Counts/%d",10*0.6/200);
  hdyEdep->GetXaxis()->SetTitle("Y distance [cm]");
  hdyEdep->GetYaxis()->SetTitle(title.Data());  
  
  
  int signals[64][2], jsig[64];
  double fiberEdep[64];
  Int_t thFb[100];
  Double_t modA = 0.;
  Int_t thFbN = 0;
  int bsum_pe=0;
  int fsum_pe=0;
  FairLink link;
  for (int iEvent = 0; iEvent < nEvent; iEvent++){
    //cout << iEvent << endl;
    modA = 0.;
    
    steps->Clear();
    pmtSignals->Clear();
    fpoints->Clear();
    
    T->GetEntry(iEvent);
    Tsim->GetEntry(iEvent);
    
    if (steps->GetEntriesFast() == 0)
      continue;
    ERNeuRadStep* step = (ERNeuRadStep*)steps->At(0);
    
    for (int i = 0; i<64; i++){
      fb[i] = 0;
      jsig[i] = 0;
      signals[i][0] = 0;
      signals[i][1] = 0;
      fiberEdep[i] = 0.;
    }
    
    for (int iPoint = 0; iPoint < points->GetEntriesFast(); iPoint++){
      ERNeuRadPoint* point = (ERNeuRadPoint*) points->At(iPoint);
      fiberEdep[(int)point->GetFiberInBundleNb()] += point->GetEnergyLoss();
    }
    
    int thFEdepC = 0, iThFEdep;
    for (int iFiber = 0; iFiber < 64; iFiber++){
      if (fiberEdep[iFiber] > thEdep *10e-6){
        iThFEdep = iFiber;
        thFEdepC++;
      }
    }
    
    if (thFEdepC == 1){
      hdxEdep->Fill(step->GetX()-fb_x[iThFEdep]);
      hdyEdep->Fill(step->GetY()-fb_y[iThFEdep]);
    }
    
    Long_t sigCount = pmtSignals->GetEntriesFast();
    for (int iSignal = 0; iSignal < sigCount; iSignal++){
        ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
        if (!signal->Exist())
          continue;
        
        for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
          link = signal->GetLink(iLink);
          ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(link.GetIndex());
          modA += fpoint->PhotoElCount();
        }
        
        if (signal->GetFirstInteg(100.) > fiberThreshold){
          fb[signal->FiberIndex()]++;
          signals[signal->FiberIndex()][jsig[signal->FiberIndex()]++] = iSignal;
        }        
    }
   
    hAmod->Fill(modA);
    
    //Îïðåäåëÿåìñÿ ñ êîëè÷åñòâîì ïðîøåäøèõ ïîðîã
    
    thFbN = 0;
    
    for (int i = 0; i<64; i++){
      if (fb[i] == 2) //Çàãîðåëîñü ñ äâóõ ñòîðîí
        thFb[thFbN++] = i;
    }
    
    //Ðàáîòàåì òîëüêî ñ îäíîôàéáåðíûìè
    if (thFbN == 1){
      oneFbEv++;
      
      double dist = TMath::Sqrt( (step->GetX()-fb_x[thFb[0]])*(step->GetX()-fb_x[thFb[0]]) +
                              (step->GetY()-fb_y[thFb[0]])*(step->GetY()-fb_y[thFb[0]]));
      hdxy->Fill(dist);
      if (dist < 0.6){
        hdxyLess6->Fill(dist);
      }
      hdx->Fill(TMath::Abs(step->GetX()-fb_x[thFb[0]]));
      hdxLess6->Fill(TMath::Abs(step->GetX()-fb_x[thFb[0]]));
      hdy->Fill(TMath::Abs(step->GetY()-fb_y[thFb[0]]));
      hdyLess6->Fill(TMath::Abs(step->GetY()-fb_y[thFb[0]]));
      ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(signals[thFb[0]][0]);
      fsum_pe=0;
      for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
        ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(signal->GetLink(iLink).GetIndex());
        fsum_pe += fpoint->PhotoElCount();
      }
      hfApe->Fill(fsum_pe);
      signal = (ERNeuRadPMTSignal*)pmtSignals->At(signals[thFb[0]][1]);
      bsum_pe=0;
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
  title.Form("%s/%d_MeV_distXY_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(), nEvent, pe);
  c1->SaveAs(title.Data());
  
  
  c1->Clear();
  hdxyLess6->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distXY_less6_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdx->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distX_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdxLess6->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distX_less6_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdy->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distY_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdyLess6->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distY_less6_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  
  c1->Clear();
  hfApe->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_frontFiberA[pe]_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hbApe->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_backFiberA[pe]_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hApe->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_sumFiberA[pe]_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hAmod->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_moduleA[pe]_%s_%dev_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdxEdep->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_dist_X_th%.2fKeV_%s_%dev.png", pngdir.Data(),eKin,thEdep,pngname.Data(),nEvent);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdyEdep->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_dist_Y_th%.2fKeV_%s_%dev.png", pngdir.Data(),eKin,thEdep,pngname.Data(),nEvent);
  c1->SaveAs(title.Data());
  
}