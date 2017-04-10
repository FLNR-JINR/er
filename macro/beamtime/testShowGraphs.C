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

//	gSystem->Load("../libData.so");

	const Long64_t kFirstEvent = 110;
	TFile fr("~/er/macro/beamtime/analyze.root");

	TTree *tr = (TTree*)fr.Get("cbmsim");

	AEvent *revent = new AEvent();
	tr->SetBranchAddress("Ach1.",&revent);

	TGraph *gr[13];
	TGraph *grcfd[13];


	//loop over events
	for (Long64_t i = 0; i < 13; i++) {
		gr[i] = 0;
		grcfd[i] = 0;
		tr->GetEntry(i+kFirstEvent);
		gr[i] = new TGraph(*revent->GetGraphSignal());
		grcfd[i] = new TGraph(*revent->GetGraphCFD());
	}//for over events

	TCanvas *c1 = new TCanvas("c1","Signal shape",10,10,1000,600);

	c1->Divide(2,2);
	for (Int_t k = 0; k < 4; k++) {
		c1->cd(k+1);
		//gr[k]->GetXaxis()->SetRangeUser(130, 175);
		gr[k]->GetXaxis()->SetRangeUser(135, 145);
		gr[k]->GetXaxis()->SetTitle("Time [ns]");
		gr[k]->GetXaxis()->CenterTitle();
		gr[k]->GetYaxis()->SetTitle("Signal [V]");
		gr[k]->GetYaxis()->CenterTitle();
		gr[k]->Draw("AL*");

	}

	TCanvas *c2 = new TCanvas("c2","CFD Signal shape",10,10,1000,600);

	c2->Divide(2,2);
	for (Int_t k = 0; k < 4; k++) {
		c2->cd(k+1);

		grcfd[k]->GetXaxis()->SetRangeUser(130,175);
		grcfd[k]->Draw("AL*");

	}
}
