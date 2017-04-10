#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"

void walk_corr(
//01 means it is used for channels 0 and 1

	const char *filename = "analyze.root",	//considered non-collimated
//	const char *filename = "analysis_07_8_smooth.root",
//	const char *filename = "analysis_08_2_smooth.root",
	const Bool_t can1 = 1)

{
	using std::cout;
	using std::endl;
	//gSystem->Load("../libData.so");
	TFile *f = new TFile(Form("~/er/macro/beamtime/%s", filename));
	TTree *t = (TTree*)f->Get("cbmsim");

	if(can1){
		
		TCanvas *c1 = new TCanvas("c1", "Trying to correct the walk effect");

//pictures for latex report, LED
/*		c1->Divide(2,1);
		c1->cd(1);
		TH2F *h2 = new TH2F("h2", "time difference (LED method) vs charge of non-trigger channel ", 100, -12 ,5, 100, 0, 0.14e-9);
		t->Draw(" Ach1.fChargeLED : Ach0.fTimeLED - Ach1.fTimeLED >> h2","","col");
		//x
		h2->GetXaxis()->SetTitle("time  [ns]");
		h2->GetXaxis()->CenterTitle();
		//y
		h2->GetYaxis()->SetTitle("Q [C]");
		h2->GetYaxis()->CenterTitle();

//		c1->cd(2);
		TProfile *prof1 = new TProfile("prof1","Profile of y vs x",100, -12, 5, 0, 0.17e-9);
		prof1 = h2->ProfileX();
//		prof1->Fit("pol2","","",-6,0);
		prof1->Draw("same");
*/

//pictures for latex report, CFD
		c1->Divide(2,2);
		c1->cd(1);
		TH2F *h2 = new TH2F("h2", "time difference (CFD method) vs charge of non-trigger channel BEFORE CORRECTION", 100, -8 ,8, 100, 0, 0.25e-9);
		t->Draw(" Ach2.fChargeCFD : Ach1.fTimeCFD - Ach2.fTimeCFD  >> h2","","col");
		//x
		h2->GetXaxis()->SetTitle("time [ns]");
		h2->GetXaxis()->CenterTitle();
		//y
		h2->GetYaxis()->SetTitle("Q [C]");
		h2->GetYaxis()->CenterTitle();
		h2->SetMaximum(20);
		c1->Update();

		c1->cd(2);
		TH1F *h5 = new TH1F("h5", "time difference BEFORE CORRECTION", 20, -8, 8);
		t->Draw(" (Ach1.fTimeCFD - Ach2.fTimeCFD) >> h5","","");
		h5->GetXaxis()->SetTitle("Time difference [ns]");
		h5->GetXaxis()->CenterTitle();
		//h4->Fit("gaus","","",-3,1);
		h5->Fit("gaus","","",-2.5 ,1.5);
		gStyle->SetOptFit();

		c1->cd(3);
		TH2F *h3 = new TH2F("h3", "time difference (CFD method) vs charge of non-trigger channel AFTER CORRECTION", 100, -8 , 8, 100, 0, 0.25e-9);
		h3->SetMaximum(20);
		t->Draw("Ach2.fChargeCFD : (Ach1.fTimeCFD - Ach2.fTimeCFD) + 0.550*1.58e-11/Ach2.fChargeCFD >> h3","","col");

  		c1->cd(4);
		TH1F *h4 = new TH1F("h4", "time difference AFTER CORRECTION", 50, -8, 8);
		t->Draw(" (Ach1.fTimeCFD - Ach2.fTimeCFD) + 0.550*1.58e-11/Ach2.fChargeCFD >> h4","","");
		h4->GetXaxis()->SetTitle("Time difference [ns]");
		h4->GetXaxis()->CenterTitle();
		//h4->Fit("gaus","","",-3,1);
		h4->Fit("gaus","","",-2.5 ,1.5);
		gStyle->SetOptFit();

/*	 try to correct in a smart way by using trigonomerty failed
		c1->cd(5);
		TProfile *prof1 = h2->ProfileX();
		Float_t k, sina, cosa;
		prof1->Fit("pol1","","",-4,0);
		//k = pol1->GetParameter(1);
		//k = 4;
		cosa = sqrt(1/(1+k*k));
		sina = sqrt(1 - cosa*cosa);
		cout<<"k = "<<k<<" sin = "<<sina<<" cos = "<<cosa<<endl;
		prof1->Draw();

		c1->cd(6);
		TProfile *prof2 = h3->ProfileX();
		prof2->Draw();
*/



	}

}
