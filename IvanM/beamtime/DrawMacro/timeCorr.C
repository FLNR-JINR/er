#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"

void timeCorr(

	const Bool_t can1 = 0, const Bool_t cane1 = 0,
	const Bool_t canLED = 1, const Bool_t canCFD = 1,
	const Bool_t canMID = 1, const Bool_t can10 = 1,
	const Bool_t canToT = 1, const Bool_t can2 = 1 ) // tLEd and CFD for colollinated and NONcollimated data

{
	//TString foldername = "30_06
	Double_t rMin;
	Double_t rMax;
	TF1 *g1 = new TF1("g1","gaus",0,100);
	using std::cout;
	using std::endl;

	TFile *f = new TFile("/home/ivanm/er/IvanM/NeuRad/expData/dataDSR4/drs4.analize.root");
	TTree *t = (TTree*)f->Get("cbmsim");

	TFile *f1 = new TFile("/home/ivanm/er/IvanM/NeuRad/oscill/data/analize1.root");
	TTree *t1 = (TTree*)f1->Get("cbmsim");

	gStyle->SetOptFit();
	if (can1) { 
		TCanvas *c1 = new TCanvas("c5", "Changing CFD parameters expDATA");
		c1->Divide(2,2);
		c1->cd(1);

		TH1F *h1 = new TH1F("h1", "tau_CFD",100, 0., 20.);
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","","");
		h1->GetXaxis()->SetRangeUser(0, 20);
		h1->GetXaxis()->SetTitle("tauCFD [ns]");
		h1->GetXaxis()->CenterTitle();

		c1->cd(2);
		TH1F *h2 = new TH1F("h2", "tau_LED ",100, 0., 20.);
		t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","","");
		h2->GetXaxis()->SetRangeUser(0, 20);
		h2->GetXaxis()->SetTitle("tauLED [ns]");
		h2->GetXaxis()->CenterTitle();

		c1->cd(3);
		TH1F *h4 = new TH1F("h4", "tau_50 percent",100, 0., 20.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> h4","","");
		h4->GetXaxis()->SetRangeUser(0, 20);
		h4->GetXaxis()->SetTitle("tau50 [ns]");
		h4->GetXaxis()->CenterTitle();
		c1->cd(4);

		TH1F *h3 = new TH1F("h3", "tau_10 percent",100, 0., 20.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h3","","");
		h3->GetXaxis()->SetRangeUser(0, 20);
		h3->GetXaxis()->SetTitle("tau10 [ns]");
		h3->GetXaxis()->CenterTitle();
	}
	if (cane1) { 
		TCanvas *ce1 = new TCanvas("ce1", "Changing CFD parameters sim");
		ce1->Divide(2,2);
		ce1->cd(1);

		TH1F *he1 = new TH1F("he1", "tau_CFD",100, -10., 10.);
		// t1->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> he1","Ach1.fTimeCFD - Ach2.fTimeCFD >-1 && Ach1.fTimeCFD - Ach2.fTimeCFD < 1","");
		t1->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD ","","");
		he1->GetXaxis()->SetRangeUser(-1, 1);
		he1->GetXaxis()->SetTitle("tauCFD [ns]");
		he1->GetXaxis()->CenterTitle();

		ce1->cd(2);
		TH1F *he2 = new TH1F("he2", "tau_LED ",100, -10., 10.);
		t1->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> he2","","");
		he2->GetXaxis()->SetRangeUser(-10, 10);
		he2->GetXaxis()->SetTitle("tauLED [ns]");
		he2->GetXaxis()->CenterTitle();

		ce1->cd(3);
		TH1F *he4 = new TH1F("he4", "tau_50 percent",100, -10., 10.);

		t1->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> he4","","");
		he4->GetXaxis()->SetRangeUser(-10, 10);
		he4->GetXaxis()->SetTitle("tau50 [ns]");
		he4->GetXaxis()->CenterTitle();
		ce1->cd(4);

		TH1F *he3 = new TH1F("he3", "tau_10 percent",100, -10., 10.);
		t1->Draw("Ach1.fTime10 - Ach2.fTime10 >> he3","","");
		he3->GetXaxis()->SetRangeUser(-10, 10);
		he3->GetXaxis()->SetTitle("tau10 [ns]");
		he3->GetXaxis()->CenterTitle();
	}


// Long64_t Draw(const char* varexp, const TCut& selection, Option_t* option = "", Long64_t nentries = 1000000000, Long64_t firstentry = 0)
	if (canLED) { 
		TCanvas *cled = new TCanvas("cled", "conparing LED");

		TH1F *hsim1 = new TH1F("hsim1", "tau_LED",100, -10., 10.);
		hsim1->SetLineColor(kRed);
		t1->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> hsim1","Ach1.fTimeLED - Ach2.fTimeLED>-10 && Ach1.fTimeLED - Ach2.fTimeLED<10","",8335,0);
		hsim1->GetXaxis()->SetRangeUser(-10, 10);
		hsim1->GetXaxis()->SetTitle("tauLEDsim [ns]");
		hsim1->GetXaxis()->CenterTitle();
//8133
		TH1F *hexp1 = new TH1F("hexp1", "tau_LEDexp",100, -10., 10.);
		t->Draw("Ach1.fTimeLED - Ach2.fTimeLED - 10.75 >> hexp1","(Ach1.fTimeLED - Ach2.fTimeLED - 10.75)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED-10.75)<10","SAME",9999,0);
		hexp1->GetXaxis()->SetRangeUser(-10, 10.);
		hexp1->GetXaxis()->SetTitle("tauLEDexp [ns]");
		hexp1->GetXaxis()->CenterTitle();

	}

	if (canCFD) { 
		TCanvas *ccfd = new TCanvas("ccfd", "conparing CFD");

		TH1F *hsimCFD = new TH1F("hsimCFD", "tau_CFDsim",100, -10., 10.);
		hsimCFD->SetLineColor(kRed);
		t1->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> hsimCFD","Ach1.fTimeCFD - Ach2.fTimeCFD>-10 && Ach1.fTimeCFD - Ach2.fTimeCFD<10","",7156,0);
		hsimCFD->GetXaxis()->SetRangeUser(-10, 10);
		hsimCFD->GetXaxis()->SetTitle("tauCFDsim [ns]");
		hsimCFD->GetXaxis()->CenterTitle();
//6987
		TH1F *hexpCFD = new TH1F("hexpCFD", "tau_CFDexp",100, -10., 10.);
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD - 10.75 >> hexpCFD","(Ach1.fTimeCFD - Ach2.fTimeCFD - 10.75)>-10 && (Ach1.fTimeCFD - Ach2.fTimeCFD-10.75)<10","SAME",9999,0);
		hexpCFD->GetXaxis()->SetRangeUser(-10, 10.);
		hexpCFD->GetXaxis()->SetTitle("tauCFDexp [ns]");
		hexpCFD->GetXaxis()->CenterTitle();
	}

	if (canMID) { 
		TCanvas *cMid = new TCanvas("cMid", "conparing Mid");

		TH1F *hsimMid = new TH1F("hsimMid", "tau_Midsim",100, -10., 10.);
		hsimMid->SetLineColor(kRed);
		t1->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> hsimMid","Ach1.fTimeMid - Ach2.fTimeMid>-10 && Ach1.fTimeMid - Ach2.fTimeMid<10","",8350,0);
		hsimMid->GetXaxis()->SetRangeUser(-10, 10);
		hsimMid->GetXaxis()->SetTitle("tauMidsim [ns]");
		hsimMid->GetXaxis()->CenterTitle();
//8209
		TH1F *hexpMid = new TH1F("hexpMid", "tau_Midexp",100, -10., 10.);
		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid - 10.75 >> hexpMid","(Ach1.fTimeMid - Ach2.fTimeMid - 10.75)>-10 && (Ach1.fTimeMid - Ach2.fTimeMid-10.75)<10","SAME",9999,0);
		hexpMid->GetXaxis()->SetRangeUser(-10, 10.);
		hexpMid->GetXaxis()->SetTitle("tauMidexp [ns]");
		hexpMid->GetXaxis()->CenterTitle();
	}

	if (can10) { 
		TCanvas *c10 = new TCanvas("c10", "conparing 10");

		TH1F *hsim10 = new TH1F("hsim10", "tau_10sim",100, -10., 10.);
		hsim10->SetLineColor(kRed);
		t1->Draw("Ach1.fTime10 - Ach2.fTime10 >> hsim10","Ach1.fTime10 - Ach2.fTime10>-10 && Ach1.fTime10 - Ach2.fTime10<10","",8332,0);
		hsim10->GetXaxis()->SetRangeUser(-10, 10);
		hsim10->GetXaxis()->SetTitle("tau10sim [ns]");
		hsim10->GetXaxis()->CenterTitle();
//8177
		TH1F *hexp10 = new TH1F("hexp10", "tau_10exp",100, -10., 10.);
		t->Draw("Ach1.fTime10 - Ach2.fTime10 - 10.75 >> hexp10","(Ach1.fTime10 - Ach2.fTime10 - 10.75)>-10 && (Ach1.fTime10 - Ach2.fTime10-10.75)<10","SAME",9999,0);
		hexp10->GetXaxis()->SetRangeUser(-10, 10.);
		hexp10->GetXaxis()->SetTitle("tau10exp [ns]");
		hexp10->GetXaxis()->CenterTitle();
	}

	if (canToT) { 
		TCanvas *cToT = new TCanvas("cToT", "conparing triggered ToT");

		TH1F *hsimToT = new TH1F("hsimToT", "tau_ToTsim",100, 0., 10.);
		hsimToT->SetLineColor(kRed);
		t1->Draw("Ach1.fToT >> hsimToT","","",9999,0);
		hsimToT->GetXaxis()->SetRangeUser(0, 10);
		hsimToT->GetXaxis()->SetTitle("tauToTsim [ns]");
		hsimToT->GetXaxis()->CenterTitle();
//8177
		TH1F *hexpToT = new TH1F("hexpToT", "tau_ToTexp",100, 0., 10.);
		t->Draw("Ach1.fToT >> hexpToT","","SAME",9999,0);
		hexpToT->GetXaxis()->SetRangeUser(0, 10.);
		hexpToT->GetXaxis()->SetTitle("tauToTexp [ns]");
		hexpToT->GetXaxis()->CenterTitle();
	}

	if (can2) { 

			TCanvas *c2 = new TCanvas("c2", "time charge corr");
			c2->Divide(2,1);
			TH2F *hsim2 = new TH2F("hsim2", "tauLED vs CHARGE SIM ", 40, -10., 10, 40, 0 ,200);
			hsim2->SetLineColor(kRed);
			c2->cd(1);
			t1->Draw("Ach2.fAmpMax : Ach1.fTimeLED - Ach2.fTimeLED >> hsim2","","col");
			// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD  : Ach1.fChargeCFD + Ach2.fChargeCFD >> h11","","col");
			//x
			hsim2->GetYaxis()->SetTitle("Charge [kl]");
			hsim2->GetYaxis()->CenterTitle();
			//y
			hsim2->GetXaxis()->SetTitle("tau_LED [V]");
			hsim2->GetXaxis()->CenterTitle();

			TH2F *hexp2 = new TH2F("hexp2", "tauLED vs CHARGE EXP", 40, -10., 10, 40, 0 ,200);
			hexp2->SetLineColor(kBlue);
			c2->cd(2);
			t->Draw("Ach2.fAmpMax*1000 : Ach1.fTimeLED - Ach2.fTimeLED - 10. >> hexp2","","col");
			// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD  : Ach1.fChargeCFD + Ach2.fChargeCFD >> h11","","col");
			//x   Ach2.fChargeLED > 0.05*1e-10
			hexp2->GetYaxis()->SetTitle("Charge [kl]");
			hexp2->GetYaxis()->CenterTitle();
			//y
			hexp2->GetXaxis()->SetTitle("tau_LED [V]");
			hexp2->GetXaxis()->CenterTitle();
	}
return;
}
