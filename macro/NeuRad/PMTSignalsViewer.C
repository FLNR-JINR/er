void PMTSignalsViewer(){
  TString fdigiName = "digi.root";

  TFile fdigi(fdigiName);

  TTree* digitree = (TTree*) fdigi.Get("cbmsim");
  
  TClonesArray* pmtSignals = new TClonesArray("ERNeuRadPMTSignalF",1000);
  TBranch* digi_trbr = digitree->GetBranch("NeuRadPMTSignal");
  digi_trbr->SetAddress(&pmtSignals);
  Int_t iEvent = 1;
  //for (int iEvent = 0; iEvent < digitree->GetEntries(); iEvent++){ 
    digitree->GetEntry(iEvent);

    TCanvas *c1 = new TCanvas("NeuRad PMTSignal","NeuRad PMTSignal",200,10,1000,500);
    // /c1->Divide(2,2,0,0);
    //for (int iSignal = 0; iSignal < pmtSignals->GetEntriesFast()/2; iSignal++){
      
      //c1->cd(3);
    if(pmtSignals->GetEntriesFast()){
        int iSignal = 0;
        ERNeuRadPMTSignalF* signal = (ERNeuRadPMTSignalF*)pmtSignals->At(iSignal);
        cerr << signal->Exist() << endl;
        Float_t* resultSignal = signal->ResultSignal()->GetArray();
      

        Int_t pointsCount = (signal->GetFinishTime()-signal->GetStartTime())/signal->dT();
        //cerr << signal->GetFinishTime() << " " << signal->GetStartTime() << " " << signal->dT() << " " << pointsCount << endl;
        Float_t *times = new Float_t[pointsCount];

        for (int i =0; i < pointsCount; i++){
          times[i] = signal->GetStartTime() + i*signal->dT();
        }
        
        TGraph* gr1 = new TGraph(pointsCount,times,resultSignal);
        for (int i = 0; i < pointsCount; i++){
          cout << times[i] << " " << resultSignal[i] << endl;
        }
        gr1->Draw("AL*");
      }
      //}
        //gPad->Modified();
/*
        ERNeuRadPMTSignalF* signal2 = (ERNeuRadPMTSignalF*)pmtSignals->At(iSignal+1);
      if (!signal2->Exist())
          continue;
        Double_t *resultSignal2 = signal2->ResultSignal();
        Int_t pointsCount2 = (signal2->GetFinishTime()-signal2->GetStartTime())/signal2->dT();
        Double_t *times2 = new Double_t[pointsCount2];

        for (int i =0; i < pointsCount; i++){
          times2[i] = signal2->GetStartTime() + i*signal2->dT();
        }

        TGraph* gr2 = new TGraph(pointsCount2,times2,resultSignal2);
        c1->cd(4);
        gr2->Draw("AL");
        gPad->Modified();

        c1->SaveAs("signal");
        */
    //}
}