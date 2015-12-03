/*
"однофайберный" режим. На каждом пикселе каждого ФЭУ ставится порог на интеграл импульса за промежуток 5 нс. 

Учитываются только те волокна, где с обоих концов преодолен этот порог. Далее отбираются только события, где порог преодолен только в одном файбере 
(но с двух сторон). Гистограммируется расстояние в плоскости XY между точкой входа нейтрона и центром единтсвенноо сработавшего файбера.

Значения порога должны соответствовать средним значениям 2, 5, 12, 30 фотоэлектронов. Мы не считаем штуки фотоэлектронов, а смотри интеграл сигнала с учетом всех разбросов!

Обязательно записывать в углу гистораммы сколько событий из миллиона (или хотя бы 100 000) в нее попали - это будет указание на эффективность.
*/

void res1(){
  TH1F* hdxy = new TH1F("hdxy", "XY disance, One fiber events, neutron, Ekin=250 MeV, 1000000 events",50,0.,10*0.6);
  hdxy->GetXaxis()->SetTitle("XY disance [cm]");
  hdxy->GetYaxis()->SetTitle("Counts/");
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
  
  TString fdigiName = "digi.root";
  TString fsimName = "sim.root";
  
  TFile fdigi(fdigiName);
  TFile fsim(fsimName);
  
  TTree* Tsim = (TTree*) fsim.Get("cbmsim");
  TTree* T = (TTree*) fdigi.Get("cbmsim");
  
  TClonesArray* steps = new TClonesArray("ERNeuRadStep",10000);
  Tsim->SetBranchAddress("NeuRadStep",&steps);
   
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignal",1000);
  T->SetBranchAddress("NeuRadPMTSignal",&pmtSignals);
  
  Double_t pe = 5.; //5,12,30
  
  Double_t fiberThreshold = pe*0.5*(0.1 + 1.3)*5.;
  Int_t oneFbEv = 0;
  for (int iEvent = 0; iEvent < T->GetEntries(); iEvent++){
    
    T->GetEntry(iEvent);
    Tsim->GetEntry(iEvent);
    for (int i = 0; i<64; i++)
      fb[i] = 0;
    
    for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast(); iSignal++){
        ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
        if (!signal->Exist())
          continue;
        if (signal->GetMaxInteg(5., 0.1) > fiberThreshold)
          fb[signal->FiberIndex()]++;
    }
    
    ERNeuRadStep* step = (ERNeuRadStep*)steps->At(0);
    cout << step->GetX() << " " << step->GetY() << endl;
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
    }
  }
  cout << "oneFbEv = " << oneFbEv << endl;
  hdxy->Draw();
}