void SumEdepAndLightYield(){
	
	TFile *sim = new TFile("sim.root");
	TFile *out = new TFile("out.root", "recreate");
	
	//Read root tree from file 
	TTree *T = (TTree*)sim->Get("cbmsim");
	TClonesArray *points = new TClonesArray("ERNeuRadPoint");
	T->SetBranchAddress("NeuRadPoint",&points);

	const double bin_min = 0.;
	const double bin_max = 100.;
	const double bin = 0.1;
	const int nbins = int((bin_max-bin_min)/bin); 
	
	TH1F* hPointsEdep = new TH1F("Neutron. EKin=500MeV. Sum Energy deposit","Neutron. EKin=500MeV. Sum Energy deposit of all fibers in event. Cut = 100 KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsEdep->GetXaxis()->SetTitle("Sum Edep [MeV] ");
    hPointsEdep->GetYaxis()->SetTitle(TString.Form("Counts/%f MeV", bin));
	
	TH1F* hPointsLYeild = new TH1F("500MeV neutron. Sum light yeild.","Neutron. EKin=500MeV. Sum light yeild of all fibers in event. Cut Sum Edep = 100 KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsLYeild->GetXaxis()->SetTitle("Sum Light Yield [MeV] ");
    hPointsLYeild->GetYaxis()->SetTitle(TString.Form("Counts/%f MeV", bin));
	
	Long64_t nEvents = T->GetEntries();
	for (Long64_t iEvent=0; iEvent < nEvents; iEvent++) {
		points->Clear();
		T->GetEntry(iEvent);
		
		Int_t nTracks = points->GetEntriesFast();
		
		Double_t sumEPerFibers[64];
		Double_t sumLPerFibers[64];
		for (Int_t ifiber = 0; ifiber < 64; ifiber++){
			sumEPerFibers[ifiber] = 0;
			sumLPerFibers[ifiber] = 0;
		}
	
		for (Int_t i=0;i<nTracks;i++) {
			ERNeuRadPoint *point = (ERNeuRadPoint*)points->At(i);
			sumEPerFibers[(Int_t)point->GetFiberInBundleNb()]+= point->GetEnergyLoss()/1000.; //MeV
			sumLPerFibers[(Int_t)point->GetFiberInBundleNb()]+= point->GetLightYield();  	  //MeV
		}
		
		for (Int_t ifiber = 0; ifiber < 64; ifiber++){
			if (sumEPerFibers[ifiber] > 0.1){ //threshold
				hPointsEdep->Fill(sumEPerFibers[ifiber]);
				hPointsLYeild->Fill(sumLPerFibers[ifiber]);
			}				
		}
  }
  out->cd();
  hPointsEdep->Write();
  hPointsLYeild->Write();
}