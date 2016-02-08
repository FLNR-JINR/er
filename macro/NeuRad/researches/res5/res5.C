/*
   Вывод для 25-го события 
*/

void res5(int eKin, TString pngdir, TString pngname, int pe, int sumpe, Double_t thEdep,TString fsimName, TString fdigiName){
  
  
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
  

  int signals[64][2], jsig[64];
  double fiberEdep[64];
  Int_t thFb[64][2];
  Double_t modA = 0.;
  Double_t sumModule = 0.;
  Int_t thFbN = 0;
  int bsum_pe=0;
  int fsum_pe=0;
  FairLink link;
  //for (int iEvent = 0; iEvent < 100/* nEvent*/; iEvent++){
      int iEvent = 25;
      T->GetEntry(iEvent);
      Tsim->GetEntry(iEvent);
   cout << "LYTime\tAnodeTime\tPhotoElCount\t" << endl;
   double min = 999999;
   for (int ifpoint = 0; ifpoint < fpoints->GetEntriesFast(); ifpoint++){
      ERNeuRadFiberPoint* p = (ERNeuRadFiberPoint*)fpoints->At(ifpoint);
      if ( p->AnodeTime() < min) min =  p->AnodeTime();
      cout <<p->LYTime() << " " << p->AnodeTime()  << " "<< p->PhotoElCount();
      for (int iLink = 0; iLink < p->GetNLinks();iLink++){
          cout << " " << p->GetLink(iLink).GetIndex();
      }
      cout << endl;
   }
      cout << "min = " << min << endl;
  //}
}