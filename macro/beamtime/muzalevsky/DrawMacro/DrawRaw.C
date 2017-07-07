#include <TTree.h>
#include <TGraph.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include "TStyle.h"

void DrawRaw()
{
	using std::cout;
	using std::endl;

	gStyle->SetCanvasDefH(900);
	gStyle->SetCanvasDefW(1500);

	const char *foldername = "7_8";
	const char *ext = ".gif";

	//gSystem->Load("../libData.so");

	const Long64_t kFirstEvent = 9;

	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TFile fr("../myData/simRaw.root");
	TTree *tr = (TTree*)fr.Get("cbmsim");

	TCanvas *c1 = new TCanvas("c1","test",10,10,1000,600);
	c1->Divide(4,4);

	RawEvent *revent = new RawEvent(1000);
	tr->SetBranchAddress("ch1.",&revent);
	Int_t nevents = tr->GetEntries();
	TGraph *gr[16];//= new TGraph();
	for (Int_t i = 0; i < 16; i++) {
		gr[i] = new TGraph();
	}
	// tr->GetEntry(kFirstEvent);
	// Double_t Tt; Tt = revent->GetTime(0);
	// Double_t Aa; Aa = revent->GetAmp(0);
	// cout<<Tt<< " "<<Aa<< " moya stroka " <<endl;
	// gr[0]->SetPoint(0, revent->GetTime(0), revent->GetAmp(0));
	// return;
	//  // TGraph *gr;
	// Double_t t10[16];
	// Double_t t90[16];

	//loop over events
	// for (Long64_t i = 0; i <nevents; i++) {

	// 	

	// 	gr->Draw("AL*");
	// 	c1->Update();
	// 	delete gr;
		
	// }//for over events
	Int_t k=0;
	for (Int_t k = 0; k < 16; k++)
	{
		tr->GetEntry(k+kFirstEvent);
		// gr[k] = new TGraph(*revent->GetGraphSignal());
		for(Int_t j=0;j<1000;j++){
			gr[k]->SetPoint(j, revent->GetTime(j), revent->GetAmp(j));
		}	
		// return;
		c1->cd(k+1);
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
	//c1->Print(Form("../macros/picsDRS4/file%s/signals%s", foldername, ext));
//	c1->cd(2);
//	tr->Draw("gAmp.Draw()","","goff",1,124);
}
