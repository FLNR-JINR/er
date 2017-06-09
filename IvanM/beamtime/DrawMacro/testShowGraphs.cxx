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

	const Long64_t kFirstEvent = 32;
//	../data/dataDSR4/analysis_07_8.root
	TFile fr("/home/ivanm/er/IvanM/NeuRad/expData/dataDSR4/drs4.analize.root");
	TFile fr1("/home/ivanm/er/IvanM/NeuRad/oscill/data/analyze1.root");
	TTree *tr = (TTree*)fr.Get("cbmsim");
	TTree *tr1 = (TTree*)fr1.Get("cbmsim");

	AEvent *revent = new AEvent(1024);
	tr->SetBranchAddress("Ach1.",&revent);
	AEvent *revent1 = new AEvent(1000);
	tr1->SetBranchAddress("Ach1.",&revent1);

	TGraph *gr[16];
	TGraph *gr1[16];
	Double_t t10[16];
	Double_t t90[16];

	//loop over events
//	Int_t i=0;
	for (Long64_t i = 0; i < 9; i++) {
		gr[i] = 0;
		gr1[i] = 0;
		tr->GetEntry(i+kFirstEvent);
		tr1->GetEntry(i+kFirstEvent);
		gr[i] = new TGraph(*revent->GetGraphSignal());
		gr1[i] = new TGraph(*revent1->GetGraphSignal());
	}//for over events

	TCanvas *c1 = new TCanvas("c1","EXP Signal shape",10,10,1000,600);
	c1->Divide(3,3);
	TCanvas *c2 = new TCanvas("c2","SIM Signal shape",10,10,1000,600);
	c2->Divide(3,3);
// four event picture
	//Int_t k = 0;
	for (Int_t k = 0; k < 9; k++)
	{
		c1->cd(k+1);
		// gr[k]->GetYaxis()->SetRangeUser(-0.005, 0.01);
		// gr[k+8]->GetYaxis()->SetRangeUser(-0.005, 0.01);
		// gr[k]->GetXaxis()->SetRangeUser(45, 55);
		// gr[k]->GetXaxis()->SetRangeUser(130,170);
		gr[k]->GetXaxis()->SetTitle("Time [ns]");
		gr[k]->GetXaxis()->CenterTitle();
		gr[k]->GetYaxis()->SetTitle("Signal [V]");
		gr[k]->GetYaxis()->CenterTitle();
		gr[k]->Draw("AL*");
		// c1->cd(k+2);
		// gr[k+8]->Draw("AL*");
		//cout << t10[k] << "\t" << t90[k] << endl;
		//f1->SetRange(t10[k], t90[k]);
		//gr[k]->Fit(f1, "RQ");
	}

	for (k = 0; k < 9; k++)
	{
		c2->cd(k+1);
		gr1[k]->Draw("AL*");
		gr1[k]->GetXaxis()->SetRangeUser(0, 30);
		gr1[k]->GetXaxis()->SetTitle("Time [ns]");
		gr1[k]->GetXaxis()->CenterTitle();
		gr1[k]->GetYaxis()->SetTitle("Signal [V]");
		gr1[k]->GetYaxis()->CenterTitle();
	}

}