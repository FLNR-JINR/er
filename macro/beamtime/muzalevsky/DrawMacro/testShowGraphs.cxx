#include <TTree.h>
#include <TGraph.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include "TStyle.h"

void testShowGraphs()
{
	using std::cout;
	using std::endl;

	gStyle->SetCanvasDefH(900);
	gStyle->SetCanvasDefW(1500);

	const char *foldername = "7_8";
	const char *ext = ".gif";

	//gSystem->Load("../libData.so");

	const Long64_t kFirstEvent = 1;

	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TFile fr("../../NeuRad/oscill/tests/analyze.root");
	TCanvas *c1 = new TCanvas("c1","test",10,10,1000,600);
//	c1->Divide(4,4);
	TTree *tr = (TTree*)fr.Get("cbmsim");

	AEvent *revent = new AEvent(1000);
	tr->SetBranchAddress("Ach1.",&revent);
	Int_t nevents = tr->GetEntries();
	TGraph *gr[16];
	 // TGraph *gr;
	// Double_t t10[16];
	// Double_t t90[16];

	//loop over events
	// for (Long64_t i = 0; i <nevents; i++) {

	// 	

	// 	gr->Draw("AL*");
	// 	c1->Update();
	// 	delete gr;
		
	// }//for over events

//	for (Int_t k = 0; k < 16; k++){
	 { Int_t k=0;
		tr->GetEntry(k+kFirstEvent);
		gr[k] = new TGraph(*revent->GetGraphSignal());
	//	c1->cd(k+1);
		// gr[k]->GetXaxis()->SetRangeUser(130, 175);
		// gr[k]->GetXaxis()->SetTitle("Time [ns]");
		// gr[k]->GetXaxis()->CenterTitle();
		// gr[k]->GetYaxis()->SetTitle("Signal [V]");
		// gr[k]->GetYaxis()->CenterTitle();
		gr[k]->Draw("AL*");
		// cout << t10[k] << "\t" << t90[k] << endl;
		// f1->SetRange(t10[k], t90[k]);
		// gr[k]->Fit(f1, "RQ");
	}
c1->Print("shape.png");
	//c1->Print(Form("../macros/picsDRS4/file%s/signals%s", foldername, ext));
//	c1->cd(2);
//	tr->Draw("gAmp.Draw()","","goff",1,124);
}
