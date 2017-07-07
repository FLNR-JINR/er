void testShowCFD()
{

	gSystem->Load("../libData.so");

	const Long64_t kFirstEvent = 128;

//	TFile fr("../data/dataDSR4/analysis_08_2_CFD02_06.root");
	TFile fr("../data/dataDSR4/analysis_08_2.root");
	TTree *tr = (TTree*)fr.Get("atree");

	AEvent *revent = new AEvent();
	tr->SetBranchAddress("Ach0.",&revent);

	TGraph *gr[12];

	//loop over events
	for (Int_t i = 0; i < 12; i++) {
		gr[i] = 0;
		tr->GetEntry(i+kFirstEvent);
		gr[i] = new TGraph(*revent->GetGraphCFD());
	}//for over events


	TCanvas *c1 = new TCanvas("c1","CFD implementation",10,10,1000,600);
	c1->Divide(3,4);

	for (Int_t i = 0; i < 12; i++) {
		c1->cd(i+1);
		gr[i]->Draw("");
	}

//	c1->cd(2);
//	tr->Draw("gAmp.Draw()","","goff",1,124);
}
