//Подбор параметров для распределения амплитуды одноэлектронного сигнала

void res7(){
	Double_t PMTGain = 5.0; //mV/pe
	Double_t ExcessNoiseFactor = 1.3;//

	TH1F* h = new TH1F("histo", "histo", 1000, 0., 10.);
	TRandom3* fRand = new TRandom3();
	for (int i = 0; i < 100000; i++){
		Double_t sigma = TMath::Sqrt(ExcessNoiseFactor-1)*PMTGain;
		h->Fill(TMath::Abs(fRand->Gaus(PMTGain, sigma)));
	}
	h->Draw();
}