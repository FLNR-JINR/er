#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"

void histCorr(

	const Bool_t can1 = 1, const Bool_t can2 = 0,
	const Bool_t can3 = 1, const Bool_t can4 = 0,
	const Bool_t can5 = 0, const Bool_t can6 = 0) // tLEd and CFD for colollinated and NONcollimated data

{
	//TString foldername = "30_06
	Double_t rMin;
	Double_t rMax;
	TF1 *g1 = new TF1("g1","gaus",0,100);
	using std::cout;
	using std::endl;
	//gSystem->Load("../libData.so");
	TFile *f = new TFile("/store/ivan/simNeuRadJULY/simDEV/analyze.root");
	TTree *t = (TTree*)f->Get("cbmsim");



	// TFile *f1 = new TFile("../myData/analyze1.root");
	// TTree *t1 = (TTree*)f1->Get("cbmsim");

		gStyle->SetOptFit();
	if (can1) { // t1 - коллимированные данные!!!
		TCanvas *c1 = new TCanvas("c5", "Changing CFD parameters");
		c1->Divide(2,2);
		c1->cd(1);

		g1->SetParLimits(0,1e+2,1e+4);
		g1->SetParLimits(1,-100,100);
		g1->SetParLimits(2,0.1,10);

		// t->Draw("Ach1.fToT"); return;

		TH1F *h1 = new TH1F("h1", "tau_CFD ",40, -5., 5.);
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 5 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -5","");
//	        t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD ");
//		h1->GetXaxis()->SetRangeUser(-10, 15);
		h1->GetXaxis()->SetTitle("tauCFD [ns]");
		h1->GetXaxis()->CenterTitle();
		// h1->Fit("gaus");

		c1->cd(2);
		TH1F *h2 = new TH1F("h2", "tau_LED ",40, -5., 5.);

		t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-5 && (Ach1.fTimeLED - Ach2.fTimeLED)<5","");
 
//		h2->GetXaxis()->SetRangeUser(-10, 10);
		h2->GetXaxis()->SetTitle("tauLED [ns]");
		h2->GetXaxis()->CenterTitle();
		// h2->Fit("gaus");

		c1->cd(3);
		TH1F *h3 = new TH1F("h3", "tau_MID ",40, -5., 5.);

		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> h3","(Ach1.fTimeMid - Ach2.fTimeMid)>-5 && (Ach1.fTimeMid - Ach2.fTimeMid)<5 ","");
// t->Draw("Ach1.fTimeMid - Ach2.fTimeMid");	
//	h3->GetXaxis()->SetRangeUser(-10, 10);
		h3->GetXaxis()->SetTitle("fTimeMid [ns]");
		h3->GetXaxis()->CenterTitle();
		g1->SetRange(-1.5,1);
		h3->Fit(g1,"R");
		c1->cd(4);
		TH1F *h4 = new TH1F("h4", "tau_10 ",40, -5., 5.);

		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h4","(Ach1.fTime10 - Ach2.fTime10)>-5 && (Ach1.fTime10 - Ach2.fTime10)<5","");
//t->Draw("Ach1.fTime10 - Ach2.fTime10"); 	
//	h4->GetXaxis()->SetRangeUser(-5, 10);
		h4->GetXaxis()->SetTitle("fTime10 [ns]");
		h4->GetXaxis()->CenterTitle();
		c1->Print("tau.png");
		// h4->Fit("gaus");
	}

	if (can3) {
		TCanvas *c3 = new TCanvas("c3", "Correlation pictures: Integrals and amplitudes");
		c3->Divide(2,2);


		TH2F *h10 = new TH2F("h10", "tau_led vs chargeLed ", 40, -10 ,10, 40, 0, 2e-7);
		c3->cd(1);
		t->Draw(" Ach1.fChargeLED : Ach1.fTimeLED - Ach2.fTimeLED >> h10","","col");
		//x
		h10->GetYaxis()->SetTitle("Charge [kl]");
		h10->GetYaxis()->CenterTitle();
		//y
		h10->GetXaxis()->SetTitle("tauLED [ns]");
		h10->GetXaxis()->CenterTitle();

		TH2F *h11 = new TH2F("h11", "tau_CFD vs chargeCFD ", 40, -10 ,10, 40, 0, 2e-7);
		c3->cd(2);
		t->Draw(" Ach1.fChargeCFD : Ach1.fTimeCFD - Ach2.fTimeCFD >> h11","","col");
		//x
		h11->GetYaxis()->SetTitle("Charge [kl]");
		h11->GetYaxis()->CenterTitle();
		//y
		h11->GetXaxis()->SetTitle("tauLED [ns]");
		h11->GetXaxis()->CenterTitle();

		c3->cd(3);
		TH2F *h12 = new TH2F("h12","tau_MID vs chargeMID", 40, -10, 10, 40, 0, 2e-7);
		t->Draw("Ach1.fChargeLED : Ach1.fTimeMid - Ach2.fTimeMid >> h12","","col");

		c3->cd(4);
		TH2F *h13 = new TH2F("h13","tau_10 vx charge", 40, -10, 10, 40, 0, 2e-7);
		t->Draw("Ach1.fChargeLED : Ach1.fTime10 - Ach2.fTime10 >> h13", "","col");

		c3->Print("ampcorr.png");



	}

}
