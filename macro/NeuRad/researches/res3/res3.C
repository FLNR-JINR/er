/*
"продвинутый" режим

Пишутся только те пиксели (не важно, что происходит с другого конца файбера!), где интеграл сигнала в 5 нс преодолел среднее значение 2 фотоэлектрона.

Суммарное значение сигналов с обоих концов должно быть выше переменного порога (4, 10, 20, 50 фотоэлектронов)

Ищется тот файбер, где пришел самый первый сигнал в том фэу, который выше по пучку (с той стороны, откуда летят нейтроны)

Далее гистограммируется расстояние в плоскости XY между точкой входа нейтрона и центром этого первого файбера.
*/

void res3(int eKin, TString pngdir, TString pngname, int pe, int sumpe, Double_t thEdep,TString fsimName, TString fdigiName){
  
  
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
  
  Long_t nEvent = T->GetEntries();
  
  TString title;
  title.Form("XY distance, neutron, Ekin=%d MeV, %d events, pixel threshold %d pe, module threshold %d pe", eKin,nEvent, pe, sumpe);
  TH1F* hdxy = new TH1F("hdxy", title.Data(),400,0.,4.);
  title.Form("Counts/%.3f",4./400);
  hdxy->GetXaxis()->SetTitle("XY disance [cm]");
  hdxy->GetYaxis()->SetTitle(title.Data());
  
  title.Form("XY distance less 6mm, neutron, Ekin=%d MeV, %d events, pixel threshold %d pe, module threshold %d pe",eKin,nEvent, pe, sumpe);
  TH1F* hdxyLess6 = new TH1F("hdxyLess6", title.Data(),400,0.,4.);
  title.Form("Counts/%.3f",0.4/400);
  hdxyLess6->GetXaxis()->SetTitle("XY disance [cm]");
  hdxyLess6->GetYaxis()->SetTitle(title.Data());
   
  int signals[64][2], jsig[64];
  double fiberEdep[64];
  Int_t thFb[64][2];
  Double_t modA = 0.;
  Double_t sumModule = 0.;
  Int_t thFbN = 0;
  int bsum_pe=0;
  int fsum_pe=0;
  FairLink link;
  for (int iEvent = 0; iEvent < nEvent; iEvent++){
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
        
        if (signal->GetFirstInteg(5.) > fiberThreshold || signals[signal->FiberIndex()][0] != -1){
          fb[signal->FiberIndex()]++;
          signals[signal->FiberIndex()][jsig[signal->FiberIndex()]++] = iSignal;
        }        
    }    
    //Определяемся с количеством прошедших порог
    
    thFbN = 0;
    cout << sumModule << " " <<sumModuleThreshold << endl;
    for (int i = 0; i<64; i++){
      if (fb[i] >1 && sumModule > sumModuleThreshold){//Загорелось хотя бы с одной стороны и суммарный в сигнал в модуле
        ERNeuRadPMTSignal* sig1 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[i][0]);
        ERNeuRadPMTSignal* sig2 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[i][1]);
        thFb[thFbN][0] = signals[i][0]; //Сохраняем номера сигналов
        thFb[thFbN++][1] = signals[i][1];
        }
    }
    
    if (thFbN == 0)
      continue;
    
    //Определяемся какой передний фэу загорелся раньше
    double tmin = 10000.;
    int iFind = -1;
    for(int i = 0; i < thFbN; i++){
      ERNeuRadPMTSignal* sig = (ERNeuRadPMTSignal*)pmtSignals->At(thFb[i][0]);
      if (sig->GetSignalPoints().begin()->first < tmin){
        iFind = i;
        tmin = sig->GetSignalPoints().begin()->first;
      }
    }
    
    //для самого первого зажженного, пишем его дистанцию
    ERNeuRadPMTSignal* sig = (ERNeuRadPMTSignal*)pmtSignals->At(thFb[iFind][0]);
    
    //Индекс первого загоревшегося файбера с перевалом порога
    int iFirstFiber = sig->FiberIndex();
    
    double dist = TMath::Sqrt( (step->GetX()-fb_x[iFirstFiber])*(step->GetX()-fb_x[iFirstFiber]) +
                              (step->GetY()-fb_y[iFirstFiber])*(step->GetY()-fb_y[iFirstFiber]));
    hdxy->Fill(dist);
      if (dist > 1.2)
         cout << "iEvent" << iEvent << endl;
    if (dist < 0.6){
        hdxyLess6->Fill(dist);
    } 
    
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