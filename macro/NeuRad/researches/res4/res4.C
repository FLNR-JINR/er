/*
Сколько у нас событий (и вообще файберов) когда на одном конце два и более одноэлектрона, а на другом - один или ноль?
*/

void res4(int eKin, TString pngdir, TString pngname, int pe, int sumpe, Double_t thEdep,TString fsimName, TString fdigiName){
  
  
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
  Double_t sumModuleThreshold = sumpe*0.5*(0.1 + 1.3)*5.;
  Double_t pe2fiberThreshold = 2.*0.5*(0.1 + 1.3)*5.;
  
  Long_t nEvent = T->GetEntries();
  
  TString title;  
  title.Form("fibers count/event, One of fiber pixel > 2pe Second < 2pe, neutron, Ekin=%d MeV, %d events", eKin,nEvent);
  TH1F* hdfcev = new TH1F("hdfcev", title.Data(),100,0.,10.);
  title.Form("Counts");
  hdfcev->GetXaxis()->SetTitle("fibers count/event");
  hdfcev->GetYaxis()->SetTitle(title.Data());
   
  int signals[64][2], jsig[64];
  double fiberEdep[64];
  Int_t thFb[64][2];
  Double_t modA = 0.;
  Double_t sumModule = 0.;
  Int_t thFbN = 0;
  int bsum_pe=0;
  int fsum_pe=0;
  FairLink link;
  for (int iEvent = 0; iEvent <  nEvent; iEvent++){
    //cout << iEvent << endl;
    modA = 0.;
    sumModule = 0.;
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
      signals[i][0] = -1;
      signals[i][1] = -1;
      fiberEdep[i] = 0.;
      thFb[i][0] = -1;
      thFb[i][1] = -1;
    }
    //Сохраняем сигналы файбера, который хотябы с одной стороны перешел макс порог
    Long_t sigCount = pmtSignals->GetEntriesFast();
    for (int iSignal = 0; iSignal < sigCount; iSignal++){
        ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
        if (!signal->Exist())
          continue;
        /*
        for (int iLink = 0; iLink < signal->GetNLinks();iLink++){
          link = signal->GetLink(iLink);
          ERNeuRadFiberPoint* fpoint = (ERNeuRadFiberPoint*)fpoints->At(link.GetIndex());
          modA += fpoint->PhotoElCount();
        }*/
        sumModule +=signal->GetFirstInteg(50.);
        
        if (signal->GetFirstInteg(5.) > pe2fiberThreshold || signals[signal->FiberIndex()][0] != -1){
          fb[signal->FiberIndex()]++;
          signals[signal->FiberIndex()][jsig[signal->FiberIndex()]++] = iSignal;
        }        
    }    
    //Определяемся с количеством прошедших порог
    
    thFbN = 0;
    Int_t fcount = 0;
    for (int i = 0; i<64; i++){
      if (fb[i] >1 && sumModule > sumModuleThreshold){//Загорелось хотя бы с одной стороны и суммарный в сигнал в модуле
        ERNeuRadPMTSignal* sig1 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[i][0]);
        ERNeuRadPMTSignal* sig2 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[i][1]);
        if (sig1->GetFirstInteg(5.) > pe2fiberThreshold && sig2->GetFirstInteg(5.) < pe2fiberThreshold)
          fcount++;
        }
        if (sig2->GetFirstInteg(5.) > pe2fiberThreshold && sig1->GetFirstInteg(5.) < pe2fiberThreshold)
          fcount++;
        }
    }
    hdfcev->Fill(fcount);
  }
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,500);
  c1->SetLogy();
  c1->SetGrid();
  
  hdxy->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distXY_%s_%dev_pth_%dpe_mth_%dpe.png",pngdir.Data(),eKin,pngname.Data(), nEvent, pe, sumpe);
  c1->SaveAs(title.Data());
  
  c1->Clear();
  hdxyLess6->Draw();
  gSystem->ProcessEvents();
  title.Form("%s/%d_MeV_distXY_less6_%s_%dev_pth_%dpe_mth_%dpe.png",pngdir.Data(),eKin,pngname.Data(),nEvent, pe, sumpe);
  c1->SaveAs(title.Data());
}