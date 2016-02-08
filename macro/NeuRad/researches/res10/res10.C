/*
"многофайберный" режим в геометрии v2
*/

void res10(int eKin, TString pngdir, TString pngname, int pe, int sumpe, Double_t thEdep,TString fsimName, TString fparName, TString fdigiName){
  // -----   run   -------------------------------------------  
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(fsimName);
  fRun->SetOutputFile("out.root");
  ERNeuRadDigitizer* digitizer = new ERNeuRadDigitizer(1);
  fRun->AddTask(digitizer);
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(fparName.Data());
  rtdb->setFirstInput(parIo1);
  fRun->Init();
  // ---------------------------------------------------------
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  // ---------------------------------------------------------
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
  TH1F* hdxy = new TH1F("hdxy", title.Data(),200,0.,20.);
  title.Form("Counts/%.3f",4./400);
  hdxy->GetXaxis()->SetTitle("XY disance [cm]");
  hdxy->GetYaxis()->SetTitle(title.Data());
  
  title.Form("XY distance less 6mm, neutron, Ekin=%d MeV, %d events, pixel threshold %d pe, module threshold %d pe",eKin,nEvent, pe, sumpe);
  TH1F* hdxyLess6 = new TH1F("hdxyLess6", title.Data(),100.,0.,1.);
  title.Form("Counts/%.3f",1./100);
  hdxyLess6->GetXaxis()->SetTitle("XY disance [cm]");
  hdxyLess6->GetYaxis()->SetTitle(title.Data());
  
  int fb[49][64]; 
  int signals[49][64][2], jsig[49][64];
  double fiberEdep[49][64];
  Int_t thFb[1000][2];
  Int_t thBn[1000];
  Double_t modA = 0.;
  Double_t sumModule[49] = 0.;
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
    
	
	for (int j = 0; j < 49; j++){
		for (int i = 0; i<64; i++){
		  fb[j][i] = 0;
		  jsig[j][i] = 0;
		  signals[j][i][0] = -1;
		  signals[j][i][1] = -1;
		  fiberEdep[j][i] = 0.;
		  thFb[i][0] = -1;
		  thFb[i][1] = -1;
		}
		thBn[j] = -1;
		sumModule[j] = 0.;
	}
    //Сохраняем сигналы файбера, который хотябы с одной стороны перешел макс порог
    Long_t sigCount = pmtSignals->GetEntriesFast();
    for (int iSignal = 0; iSignal < sigCount; iSignal++){
        ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)pmtSignals->At(iSignal);
        if (!signal->Exist())
          continue;
	  
        int  bundle  = signal->BundleIndex();
        int fiber = signal->FiberIndex();
        
		sumModule[bundle] +=signal->GetFirstInteg(50.);
		
        if (signal->GetFirstInteg(5.) > fiberThreshold || signals[bundle][fiber][0] != -1){
          fb[bundle][fiber]++;
          signals[bundle][fiber][jsig[bundle][fiber]++] = iSignal;
        }        
    }    
    //Определяемся с количеством прошедших порог
    
    thFbN = 0;
	for (int j = 0; j < 49; j++){
		for (int i = 0; i<64; i++){
		  if (fb[j][i] >1 && sumModule[j] > sumModuleThreshold){//Загорелось хотя бы с одной стороны и суммарный в сигнал в модуле
			ERNeuRadPMTSignal* sig1 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[j][i][0]);
			ERNeuRadPMTSignal* sig2 = (ERNeuRadPMTSignal*)pmtSignals->At(signals[j][i][1]);
			thBn[thFbN] = j; 
			thFb[thFbN][0] = signals[j][i][0]; //Сохраняем номера сигналов
			thFb[thFbN++][1] = signals[j][i][1];
			}
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
	int iFirstBundle = thBn[iFind];
    cout << iFirstBundle << " " << iFirstFiber << " " << setup->FiberX(iFirstBundle, iFirstFiber) << " " << setup->FiberY(iFirstBundle, iFirstFiber) << endl;

    double dist = TMath::Sqrt( (step->GetX()-setup->FiberX(iFirstBundle, iFirstFiber))*(step->GetX()-setup->FiberX(iFirstBundle, iFirstFiber))+
                              (step->GetY()-setup->FiberY(iFirstBundle, iFirstFiber))*(step->GetY()-setup->FiberY(iFirstBundle, iFirstFiber)));
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