/*
   Ёкспонента высвечивани€
*/

void res6(){
  
  TFile fdigi("../../sim/digi.root");
  TFile fsim("../../sim/sim.root");
  
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
  
  Long_t nEvent = T->GetEntries(); 
  
  FairLink link;
  TH1F* hly = new TH1F("dfgdfgdfg","sdfsdfsdf",50,0.,50.);
  hly->GetXaxis()->SetTitle("time");
  hly->GetYaxis()->SetTitle("pe_count");

  for (int iEvent = 0; iEvent < 1000/*nEvent*/; iEvent++)
   {

      T->GetEntry(iEvent);
      Tsim->GetEntry(iEvent);
      for (int ifpoint = 0; ifpoint < fpoints->GetEntriesFast(); ifpoint++){
         ERNeuRadFiberPoint* fp = (ERNeuRadFiberPoint*)fpoints->At(ifpoint);
         ERNeuRadPoint* p = (ERNeuRadPoint*)points->At(fp->GetLink(0).GetIndex());
         int bin = (int)(fp->LYTime()-p->GetTime());
         hly->SetBinContent(bin, hly->GetBinContent(bin) + fp->PhotoElCount());
      }

  }
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,500);
   c1->Modified();
   c1->Update();
   hly->Draw();
   c1->SaveAs("exp.png");
  c1->SetLogy();
  c1->SetGrid();
   c1->Modified();
   c1->Update();
   c1->SaveAs("exp.png");
   c1->Modified();
   c1->Update();
}