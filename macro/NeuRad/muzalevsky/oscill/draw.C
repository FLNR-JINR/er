{
	TFile* f = new TFile("digi.root");
	TTree* t = (TTree*) f->Get("cbmsim");
	TClonesArray* signals = new TClonesArray("ERNeuRadPixelSignal",1000);
	t->SetBranchAddress("NeuRadPixelSignal", &signals);

	TH1F* h = new TH1F("delta","delta",100,0,5);
	for (Int_t iEvent = 0; iEvent < t->GetEntriesFast(); iEvent++){
		t->GetEntry(iEvent);
		float front = 0;
		float back = 0;
		for (Int_t iSignal = 0; iSignal < signals->GetEntriesFast(); iSignal++){
			ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*) signals->At(iSignal);

			if (signal->PixelNb() == 100){
				if (signal->Side() == 0)
					front = signal->StartTime();
				if (signal->Side() == 1)
					back = signal->StartTime();
			}
		}
		//cout << front << " " << back << endl;
		float delta = TMath::Abs(front-back);
		if (delta > 0 && front >0 && back >0)
			h->Fill(delta);
	}
	h->Draw();
}