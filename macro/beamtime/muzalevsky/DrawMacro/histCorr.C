#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"

void histCorr(

	const Bool_t can1 = 1, const Bool_t can2 = 0,
	const Bool_t can3 = 0, const Bool_t can4 = 0,
	const Bool_t can5 = 0, const Bool_t can6 = 0) // tLEd and CFD for colollinated and NONcollimated data

{
	//TString foldername = "30_06
	Double_t rMin;
	Double_t rMax;
	TF1 *g1 = new TF1("g1","gaus",0,100);
	using std::cout;
	using std::endl;
	//gSystem->Load("../libData.so");
	TFile *f = new TFile("/store/ivan/simGSIJULY/analyze11.root");
	TTree *t = (TTree*)f->Get("cbmsim");



	// TFile *f1 = new TFile("../myData/analyze1.root");
	// TTree *t1 = (TTree*)f1->Get("cbmsim");

		gStyle->SetOptFit();
	if (can1) { // t1 - коллимированные данные!!!
		TCanvas *c1 = new TCanvas("c5", "TAU");
		c1->Divide(2,2);
		c1->cd(1);

		g1->SetParLimits(0,1e+2,1e+4);
		g1->SetParLimits(1,-2,2);
		g1->SetParLimits(2,0.1,10);

		// t->Draw("Ach1.fToT"); return;

		TH1F *h1 = new TH1F("h1", "tau_CFD ",70, -3., 3.);
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 3 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -3","");
//	        t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD ");
//		h1->GetXaxis()->SetRangeUser(-10, 15);
		h1->GetXaxis()->SetTitle("tauCFD [ns]");
		h1->GetXaxis()->CenterTitle();
		g1->SetRange(-0.69,0.66);
		h1->Fit("g1","R");

		c1->cd(2);
		TH1F *h2 = new TH1F("h2", "tau_10 ",100, -3., 3.);

		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h2","(Ach1.fTime10 - Ach2.fTime10)>-3 && (Ach1.fTime10 - Ach2.fTime10)<3","");
 
//		h2->GetXaxis()->SetRangeUser(-10, 10);
		h2->GetXaxis()->SetTitle("fTime10 [ns]");
		h2->GetXaxis()->CenterTitle();
		g1->SetRange(-0.72,0.72);
		h2->Fit("g1","R");

		c1->cd(3);
		TH1F *h3 = new TH1F("h3", "tau_MID ",100, -3., 3.);

		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> h3","(Ach1.fTimeMid - Ach2.fTimeMid)>-5 && (Ach1.fTimeMid - Ach2.fTimeMid)<5 ","");
// t->Draw("Ach1.fTimeMid - Ach2.fTimeMid");	
//	h3->GetXaxis()->SetRangeUser(-10, 10);
		h3->GetXaxis()->SetTitle("fTimeMid [ns]");
		h3->GetXaxis()->CenterTitle();
		g1->SetRange(-0.96,0.96);
		h3->Fit(g1,"R");
		c1->cd(4);
		TH1F *h4 = new TH1F("h4", "tau_90 ",100, -3., 3.);

		t->Draw("Ach1.fTime90 - Ach2.fTime90 >> h4","(Ach1.fTime90 - Ach2.fTime90)>-3 && (Ach1.fTime90 - Ach2.fTime90)<3","");
//t->Draw("Ach1.fTime10 - Ach2.fTime10"); 	
//	h4->GetXaxis()->SetRangeUser(-5, 10);
		h4->GetXaxis()->SetTitle("fTime90 [ns]");
		h4->GetXaxis()->CenterTitle();
		g1->SetRange(-1.2,1.4);
		h4->Fit(g1,"R");
		c1->Print("tau.png");
	}

	if (can3) {
		TCanvas *c3 = new TCanvas("c3", "Correlation pictures: Integrals and amplitudes");
		c3->Divide(2,2);


		TH2F *h10 = new TH2F("h10", "tau_led vs chargeLed ", 60, -5 ,5, 60, 30, 400);
		c3->cd(1);
		t->Draw(" Ach1.fAmpMax +  Ach2.fAmpMax : Ach1.fTimeLED - Ach2.fTimeLED >> h10","","col");
		//x
		h10->GetYaxis()->SetTitle("Charge [kl]");
		h10->GetYaxis()->CenterTitle();
		//y
		h10->GetXaxis()->SetTitle("tauLED [ns]");
		h10->GetXaxis()->CenterTitle();

		TH2F *h11 = new TH2F("h11", "tau_CFD vs chargeCFD ", 60, -5 ,5, 60, 30, 400);
		c3->cd(2);
		t->Draw(" Ach1.fAmpMax +  Ach2.fAmpMax  : Ach1.fTimeCFD - Ach2.fTimeCFD >> h11","","col");
		//x
		h11->GetYaxis()->SetTitle("Charge [kl]");
		h11->GetYaxis()->CenterTitle();
		//y
		h11->GetXaxis()->SetTitle("tauLED [ns]");
		h11->GetXaxis()->CenterTitle();

		c3->cd(3);
		TH2F *h12 = new TH2F("h12","tau_MID vs chargeMID", 60, -5, 5, 60, 0, 400);
		t->Draw("Ach1.fAmpMax + Ach2.fAmpMax : Ach1.fTimeMid - Ach2.fTimeMid >> h12","Ach1.fAmpMax + Ach2.fAmpMax >0 && Ach1.fAmpMax + Ach2.fAmpMax<400 & Ach1.fTimeMid - Ach2.fTimeMid>-5 & Ach1.fTimeMid - Ach2.fTimeMid<5","col");

		c3->cd(4);
		TH2F *h13 = new TH2F("h13","tau_90 vx summ amp", 60, -5, 5, 60, 0, 400);
		t->Draw("Ach1.fAmpMax + Ach2.fAmpMax : Ach1.fTime90 - Ach2.fTime90 >> h13", "Ach1.fAmpMax + Ach2.fAmpMax >0 && Ach1.fAmpMax + Ach2.fAmpMax<400 & Ach1.fTime90 - Ach2.fTime90>-5 && Ach1.fTime90 - Ach2.fTime90<5 ","col");

		//c3->Print("ampcorr.png");
	}

}
