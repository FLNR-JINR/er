void FiredFibers(){
	
	TFile *sim = new TFile("sim.root");
	TFile *out = new TFile("out.root", "recreate");
	
	TTree *T = (TTree*)sim->Get("cbmsim");
	TClonesArray *points = new TClonesArray("ERNeuRadPoint");
	T->SetBranchAddress("NeuRadPoint",&points);
	
	Long64_t nEvents = T->GetEntries();
	
	const double bin_min = 0.;
	const double bin_max = 50.;
	const double bin = 0.1;
	const int nbins = int((bin_max-bin_min)/bin); 
	
	TH1F* hPointsEdep1Fiber = new TH1F("500MeV. Point edeps. 1Fiber.",
			"Neutron. EKin=500MeV. Sum Energy deposit in 1-fiber events. Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsEdep1Fiber->GetXaxis()->SetTitle("Sum Edep [MeV]");
    hPointsEdep1Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsEdep2Fiber = new TH1F("500MeV. Point edeps. 2Fiber.",
			"Neutron. EKin=500MeV. Sum Energy deposit in (2 and more)-fiber events.Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsEdep2Fiber->GetXaxis()->SetTitle("Sum Edep [MeV]");
    hPointsEdep2Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsLYield1Fiber = new TH1F("500MeV. Point light. 1Fiber.",
			"Neutron. EKin=500MeV. Sum light yield in 1-fiber events.Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsLYield1Fiber->GetXaxis()->SetTitle("Sum light yield [MeV]");
    hPointsLYield1Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsLYield2Fiber = new TH1F("500MeV. Point light. 2Fiber.",
			"Neutron. EKin=500MeV. Sum light yield in (2 and more)-fiber events.Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsLYield2Fiber->GetXaxis()->SetTitle("Sum light yield [MeV]");
    hPointsLYield2Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsEdepPerFiber1Fiber = new TH1F("500MeV. Point edeps per fiber. 1Fiber.",
			"Neutron. EKin=500MeV. Sum Energy deposit per fiber in 1-fiber events. Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsEdepPerFiber1Fiber->GetXaxis()->SetTitle("Sum Edep [MeV]");
    hPointsEdepPerFiber1Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsEdepPerFiber2Fiber = new TH1F("500MeV. Point edeps per fiber. 2Fiber.",
			"Neutron. EKin=500MeV. Sum Energy deposit per fiber in (2 and more)-fiber events. Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsEdepPerFiber2Fiber->GetXaxis()->SetTitle("Sum Edep [MeV]");
    hPointsEdepPerFiber2Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsLYieldPerFiber1Fiber = new TH1F("500MeV. Point light per fiber. 1Fiber.",
			"Neutron. EKin=500MeV. Sum light yield per fiber in 1-fiber events. Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsLYieldPerFiber1Fiber->GetXaxis()->SetTitle("Sum light yield [MeV]");
    hPointsLYieldPerFiber1Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	TH1F* hPointsLYieldPerFiber2Fiber = new TH1F("500MeV. Point light per fiber. 2Fiber.",
			"Neutron. EKin=500MeV. Sum light yield per fiber in (2 and more)-fiber events. Cut=100KeV. 10000 events",nbins,bin_min,bin_max);
	hPointsLYieldPerFiber2Fiber->GetXaxis()->SetTitle("Sum light yield [MeV]");
    hPointsLYieldPerFiber2Fiber->GetYaxis()->SetTitle("Counts/0.1MeV");
	
	
	TH1F* hEventsPerFiber = new TH1F("Neutron. EKin = 500MeV. Events per alight fibers count","Neutron. EKin = 500MeV. Events per alight fibers ",10,0,10);
	
	for (Long64_t iEvent=0; iEvent < nEvents; iEvent++) {
		points->Clear();
		T->GetEntry(iEvent);
		
		Int_t nTracks = points->GetEntriesFast();
		
		Double_t sumEPerFibers[64];
		Double_t sumLPerFibers[64];
		Double_t sumEdep = 0;
		Double_t sumLY = 0;
		for (Int_t ifiber = 0; ifiber < 64; ifiber++){
			sumEPerFibers[ifiber] = 0;
			sumLPerFibers[ifiber] = 0;
		}
	
		for (Int_t i=0;i<nTracks;i++) {
			ERNeuRadPoint *point = (ERNeuRadPoint*)points->At(i);
			sumEPerFibers[(Int_t)point->GetFiberInBundleNb()]+= point->GetEnergyLoss()/1000.;
			sumLPerFibers[(Int_t)point->GetFiberInBundleNb()]+= point->GetLightYield();
			sumEdep += sumEPerFibers[(Int_t)point->GetFiberInBundleNb()];
			sumLY 	+= sumLPerFibers[(Int_t)point->GetFiberInBundleNb()];
		}
		//Считаем заженные файберы
		int afCount = 0;
		for (Int_t ifiber = 0; ifiber < 64; ifiber++){
			if (sumEPerFibers[ifiber] > 0.0001){
				afCount++;
			}				
		}
		hEventsPerFiber->Fill(afCount);
		if (afCount == 1){
			if (sumEdep > 0.1){
				hPointsEdep1Fiber->Fill(sumEdep);
				hPointsLYield1Fiber->Fill(sumLY);
				for (Int_t ifiber = 0; ifiber < 64; ifiber++){
					if (sumEPerFibers[ifiber] > 0.1){
						hPointsEdepPerFiber1Fiber->Fill(sumEPerFibers[ifiber]);
						hPointsLYieldPerFiber1Fiber->Fill(sumLPerFibers[ifiber]);
					}
				}
			}
		}
		if (afCount >= 2){
			if (sumEdep > 0.1){
				hPointsEdep2Fiber->Fill(sumEdep);
				hPointsLYield2Fiber->Fill(sumLY);
				for (Int_t ifiber = 0; ifiber < 64; ifiber++){
					if (sumEPerFibers[ifiber] > 0.1){
						hPointsEdepPerFiber2Fiber->Fill(sumEPerFibers[ifiber]);
						hPointsLYieldPerFiber2Fiber->Fill(sumLPerFibers[ifiber]);
					}
				}
			}
		}
			
  }
  out->cd();
  hPointsEdep1Fiber->Write();
  hPointsEdep2Fiber->Write();
  hPointsLYield1Fiber->Write();
  hPointsLYield2Fiber->Write();
  hPointsEdepPerFiber1Fiber->Write();
  hPointsEdepPerFiber2Fiber->Write();
  hPointsLYieldPerFiber1Fiber->Write();
  hPointsLYieldPerFiber2Fiber->Write();
  hEventsPerFiber->Write();
}