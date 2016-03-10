/*
	Построение основных характеристик симуляции. ELoss в CsI и LaBr
*/

void res1(){
	TString simFile = "~/er/macro/Gadast/sim.root";
	TFile fsim(simFile);
  	TTree* Tsim = (TTree*) fsim.Get("cbmsim");
  	TClonesArray* CsIPoints = new TClonesArray("ERGadastCsIPoint",1000);
  	TClonesArray* LaBrPoints = new TClonesArray("ERGadastLaBrPoint",1000);
  	Tsim->SetBranchAddress("GadastCsIPoint",&CsIPoints);
  	Tsim->SetBranchAddress("GadastLaBrPoint",&LaBrPoints);

  	Long_t nEvent = Tsim->GetEntries();
  	
  	TH1F* hEdepLaBr = new TH1F("hEdepLaBr", "Gadast LaBr energy deposit",1000,0.,0.005);
  	hEdepLaBr->GetXaxis()->SetTitle("Edep [GeV]");

  	TH1F* hEdepCsI = new TH1F("hEdepCsI", "Gadast CsI energy deposit",1000,0.,0.005);
  	hEdepCsI->GetXaxis()->SetTitle("Edep [GeV]");

  	for (int iEvent = 0; iEvent < nEvent; iEvent++){
  		double EdepCsI = 0.;
  		double EdepLaBr = 0.;
  		int wall, block, cell;
  		Tsim->GetEntry(iEvent);

  		for (int iPoint=0; iPoint < CsIPoints->GetEntriesFast();  iPoint++){
  			ERGadastCsIPoint* csipoint = (ERGadastCsIPoint*)CsIPoints->At(iPoint);
  			if (iPoint == 0){
  				wall = csipoint->GetWall();
  				block = csipoint->GetBlock();
  				cell = csipoint->GetCell();
  			}
  			//if (csipoint->GetWall() == wall && csipoint->GetBlock() == block && csipoint->GetCell() == cell)
  				EdepCsI += csipoint->GetEnergyLoss();
  		}
  		for (int iPoint=0; iPoint < LaBrPoints->GetEntriesFast();  iPoint++){
  			ERGadastLaBrPoint* labrpoint = (ERGadastLaBrPoint*)LaBrPoints->At(iPoint);
  			EdepLaBr += labrpoint->GetEnergyLoss();
  		}
  		if (EdepCsI > 0)
  			hEdepCsI->Fill(EdepCsI);
  		if (EdepLaBr > 0)
  			hEdepLaBr->Fill(EdepLaBr);
  	}
 	TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,500);
  	hEdepCsI->Draw();
  	gSystem->ProcessEvents();
  	c1->SaveAs("ELossCsI.png");

	c1->Clear();
  	hEdepLaBr->Draw();
  	gSystem->ProcessEvents();
  	c1->SaveAs("ELossLaBr.png");
}