#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"

void histCorr1(

	const Bool_t can1 = 0, const Bool_t can2 = 0, const Bool_t can11 = 1,
	const Bool_t can3 = 0, const Bool_t can4 = 0,
	const Bool_t can5 = 0, const Bool_t can6 = 0,	
	const Bool_t can12 = 0,	const Bool_t can123 = 0) // tLEd and CFD for colollinated and NONcollimated data

{
	//TString foldername = "30_06
	Double_t rMin;
	Double_t rMax;
	TF1 *g1 = new TF1("g1","gaus",0,100);
	using std::cout;
	using std::endl;
	//gSystem->Load("../libData.so");
	// TFile *f = new TFile("/home/ivanm/er/IvanM/NeuRad/oscill/data/analyze1.root");
	// TTree *t = (TTree*)f->Get("cbmsim");

	TFile fr1("/home/ivanm/er/IvanM/NeuRad/expData/dataDSR4/drs4.analize.root");
	TTree *t1 = (TTree*)fr1.Get("cbmsim");
		gStyle->SetOptFit();
	if (can1) { // t1 - коллимированные данные!!!
		TCanvas *c1 = new TCanvas("c5", "Changing CFD parameters");
		c1->Divide(2,2);
		c1->cd(1);

		g1->SetParLimits(0,1e+2,1e+4);
		g1->SetParLimits(1,-100,100);
		g1->SetParLimits(2,0.1,10);

		TH1F *h1 = new TH1F("h1", "tau_CFD",100, -5., 5.);
		// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 15 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -10","");
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1 ","","");
		h1->GetXaxis()->SetRangeUser(-5, 5);
		h1->GetXaxis()->SetTitle("tauCFD [ns]");
		h1->GetXaxis()->CenterTitle();

		c1->cd(2);
		//t->Draw("Ach1.fToT","Ach1.fToT>1");
		// return;
		TH1F *h2 = new TH1F("h2", "tau_LED ",100, -5., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","","");
		h2->GetXaxis()->SetRangeUser(-5, 5);
		h2->GetXaxis()->SetTitle("tauLED [ns]");
		h2->GetXaxis()->CenterTitle();

		c1->cd(3);
		TH1F *h4 = new TH1F("h4", "tau_50 percent",100, -5., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> h4","","");
		h4->GetXaxis()->SetRangeUser(-5, 5);
		h4->GetXaxis()->SetTitle("tau50 [ns]");
		h4->GetXaxis()->CenterTitle();
		c1->cd(4);

		TH1F *h3 = new TH1F("h3", "tau_10 percent",100, -5., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h3","","");
		h3->GetXaxis()->SetRangeUser(-5, 5);
		h3->GetXaxis()->SetTitle("tau10 [ns]");
		h3->GetXaxis()->CenterTitle();
	}

		if (can11) {
		TCanvas *ce1 = new TCanvas("ce1", "Changing CFD parameters");
		ce1->Divide(2,2);
		ce1->cd(1);

		TH1F *he1 = new TH1F("he1", "tau_CFD",100, -5., 5.);
		t1->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> he1 ","","");
		he1->GetXaxis()->SetRangeUser(-5, 5);
		he1->GetXaxis()->SetTitle("tauCFD [ns]");
		he1->GetXaxis()->CenterTitle();

		ce1->cd(2);

		TH1F *he2 = new TH1F("he2", "tau_LED ",100, -5., 5.);
		t1->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> he2","","");
		he2->GetXaxis()->SetRangeUser(-5, 5);
		he2->GetXaxis()->SetTitle("tauLED [ns]");
		he2->GetXaxis()->CenterTitle();

		ce1->cd(3);
		TH1F *he4 = new TH1F("he4", "tau_50 percent",100, -5., 5.);

		t1->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> he4","","");
		he4->GetXaxis()->SetRangeUser(-5, 5);
		he4->GetXaxis()->SetTitle("tau50 [ns]");
		he4->GetXaxis()->CenterTitle();
		ce1->cd(4);

		TH1F *he3 = new TH1F("h3", "tau_10 percent",100, -5., 5.);

		t1->Draw("Ach1.fTime10 - Ach2.fTime10 >> he3","","");
		he3->GetXaxis()->SetRangeUser(-5, 5);
		he3->GetXaxis()->SetTitle("tau10 [ns]");
		he3->GetXaxis()->CenterTitle();
	}

if (can123) { // t1 - коллимированные данные!!!
		TCanvas *c12 = new TCanvas("c12", "Changing CFD parameters");
		c12->Divide(2,2);
		c12->cd(1);

		g1->SetParLimits(0,1e+2,1e+4);
		g1->SetParLimits(1,-100,100);
		g1->SetParLimits(2,0.1,10);

		TH1F *h111 = new TH1F("h111", "tau_CFD",100, -5., 5.);
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h111 ","Ach1.fTimeCFD - Ach2.fTimeCFD != 0","");
		h111->GetXaxis()->SetRangeUser(-5, 5);
		h111->GetXaxis()->SetTitle("tauCFD [ns]");
		h111->GetXaxis()->CenterTitle();

		c12->cd(2);

		TH1F *h211 = new TH1F("h211", "tau_LED ",100, -5., 5.);

		t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h211","Ach1.fTimeLED - Ach2.fTimeLED !=0","");
		h211->GetXaxis()->SetRangeUser(-5, 5);
		h211->GetXaxis()->SetTitle("tauLED [ns]");
		h211->GetXaxis()->CenterTitle();

		c12->cd(3);
		t->Draw("Ach1.fTimeCFD");
		c12->cd(4);
		t->Draw("Ach2.fTimeCFD");
		return;

		TH1F *h411 = new TH1F("h411", "tau_50 percent",100, -5., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTimeMid - Ach2.fTimeMid >> h411","Ach1.fTimeMid - Ach2.fTimeMid!=0","");
		h411->GetXaxis()->SetRangeUser(-5, 5);
		h411->GetXaxis()->SetTitle("tau50 [ns]");
		h411->GetXaxis()->CenterTitle();
		c12->cd(4);

		TH1F *h311 = new TH1F("h311", "tau_10 percent",100, -5., 5.);

		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h311","Ach1.fTime10 - Ach2.fTime10!=0","");
		h311->GetXaxis()->SetRangeUser(-5, 5);
		h311->GetXaxis()->SetTitle("tau10 [ns]");
		h311->GetXaxis()->CenterTitle();

	}



	if (can12) {
			TCanvas *c2 = new TCanvas("c2", "Changing CFD parameters");
			c2->Divide(1,2);
			TH2F *h11 = new TH2F("h11", "tauCFD vs CHARGE ", 100, -10, 10, 100, 0 ,6e-8);
			c2->cd(1);
			t->Draw("Ach2.fChargeCFD : Ach1.fTimeCFD - Ach2.fTimeCFD  >> h11","Ach1.fTimeCFD - Ach2.fTimeCFD !=0","col");
			// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD  : Ach1.fChargeCFD + Ach2.fChargeCFD >> h11","","col");
			//x
			h11->GetYaxis()->SetTitle("Summ_Charge [kl]");
			h11->GetYaxis()->CenterTitle();
			//y
			h11->GetXaxis()->SetTitle("tau_CFD [V]");
			h11->GetXaxis()->CenterTitle();
			c2->cd(2);
			t->Draw("Ach2.fChargeCFD : Ach1.fTimeCFD - Ach2.fTimeCFD  >> h11","","col");
		
			// c2->cd(2);
			// TH1F *h13 = new TH1F("h13", "tau_CFD",100, -5., 5.);
			// // t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 15 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -10","");
			//  t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h13 ");
			// h13->GetXaxis()->SetRangeUser(-5, 5);
			// h13->GetXaxis()->SetTitle("tauCFD [ns]");
			// h13->GetXaxis()->CenterTitle();

			// c2->cd(3);
			// TH2F *h12 = new TH2F("h12", "tauCFD vs SUMM CHARGE after CORR", 100, -10, 10, 100, 0 ,1e-7);
			// t->Draw("Ach1.fChargeCFD + Ach2.fChargeCFD : (Ach1.fTimeCFD - Ach2.fTimeCFD) - 2*0.02*1e-9/(Ach1.fAmpMax+Ach2.fAmpMax)  >> h12","","col");
			// c2->cd(4);
			// TH1F *h14 = new TH1F("h14", "tau_CFD after CORR",100, -5., 5.);
			// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD - 2*0.02*1e-9/(Ach1.fAmpMax+Ach2.fAmpMax) >> h14 ");
		// t1->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h3","(Ach1.fTimeCFD - Ach2.fTimeCFD)>-10 && (Ach1.fTimeCFD - Ach2.fTimeCFD)<15","");
	}


	if (can2) { // t1 - коллимированные данные!!!
		TCanvas *c3 = new TCanvas("c5", "Changing CFD parameters");
		c3->Divide(2,2);
		c3->cd(1);

		TH1F *h21 = new TH1F("h21", "tau_CFD",100, -5., 5.);
		// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 15 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -10","");
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h21 ");
		h21->GetXaxis()->SetRangeUser(-5, 5);
		h21->GetXaxis()->SetTitle("tauCFD [ns]");
		h21->GetXaxis()->CenterTitle();
		// g1->SetRange(2.8,6.5);
		// h1->Fit("g1","R");
		//cout<<"sigma= "<<g1->GetParameter(2)<<endl

		c3->cd(2);
		TH1F *h22 = new TH1F("h22", "TOT ",100, 0., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fToT >> h22");
		// h2->GetXaxis()->SetRangeUser(-5, 5);
		h22->GetXaxis()->SetTitle("ToT [ns]");
		h22->GetXaxis()->CenterTitle();
// Double_t AEvent::GetT_10() {
// 		return fTime10;
// }

// Double_t AEvent::GetT_90() {
// 		return fTime90;
// }
		c3->cd(3);
		TH1F *h23 = new TH1F("h23", "tau_CFD",100, -5., 5.);
		// t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h1","(Ach1.fTimeCFD - Ach2.fTimeCFD) < 15 && (Ach1.fTimeCFD - Ach2.fTimeCFD) > -10","");
		t->Draw("Ach1.fTimeCFD - Ach2.fTimeCFD >> h23 ","Ach1.fToT>1","");
		h23->GetXaxis()->SetRangeUser(-5, 5);
		h23->GetXaxis()->SetTitle("tauCFD [ns]");
		h23->GetXaxis()->CenterTitle();
return;
		TH1F *h3 = new TH1F("h3", "tau_10 percent",100, -5., 5.);

		// t->Draw("Ach1.fTimeLED - Ach2.fTimeLED >> h2","(Ach1.fTimeLED - Ach2.fTimeLED)>-10 && (Ach1.fTimeLED - Ach2.fTimeLED)<15","");
		t->Draw("Ach1.fTime10 - Ach2.fTime10 >> h3","Ach1.ToT","");
		h3->GetXaxis()->SetRangeUser(-5, 5);
		h3->GetXaxis()->SetTitle("tau10 [ns]");
		h3->GetXaxis()->CenterTitle();
}

	// if (can3) {
	// 	TCanvas *c3 = new TCanvas("c3", "Correlation pictures: Integrals and amplitudes");
	// 	c3->Divide(2,2);

	// 	TH2F *h9 = new TH2F("h9", "tau_led vs chargeLed KOLLIMATED", 200, 0 ,5e-10, 200, 5, 15);
	// 	c3->cd(1);
	// 	t1->Draw("Ach0.fTimeLED - Ach1.fTimeLED : Ach0.fChargeLED >> h9","","col");
	// 	//x
	// 	h9->GetXaxis()->SetTitle("Charge [kl]");
	// 	h9->GetXaxis()->CenterTitle();
	// 	//y
	// 	h9->GetYaxis()->SetTitle("tauLED [ns]");
	// 	h9->GetYaxis()->CenterTitle();

	// 	TH2F *h10 = new TH2F("h10", "tau_led vs chargeLed NONKOLLIMATED", 200, 0 ,8e-10, 200, -10, 5);
	// 	c3->cd(3);
	// 	t->Draw("Ach0.fTimeLED - Ach3.fTimeLED : Ach0.fChargeLED >> h10","","col");
	// 	//x
	// 	h10->GetXaxis()->SetTitle("Charge [kl]");
	// 	h10->GetXaxis()->CenterTitle();
	// 	//y
	// 	h10->GetYaxis()->SetTitle("tauLED [ns]");
	// 	h10->GetYaxis()->CenterTitle();

	// 	c3->Update();

	// 	TH2F *h11 = new TH2F("h11", "tauLED vs summCharge COLLIMATED", 200, 0 ,1e-9, 200, 5, 15);
	// 	c3->cd(2);
	// 	t1->Draw("Ach0.fTimeLED - Ach1.fTimeLED  : Ach0.fChargeLED + Ach1.fChargeLED >> h11","","col");
	// 	//x
	// 	h11->GetXaxis()->SetTitle("Summ_Charge [kl]");
	// 	h11->GetXaxis()->CenterTitle();
	// 	//y
	// 	h11->GetYaxis()->SetTitle("tau_LED [V]");
	// 	h11->GetYaxis()->CenterTitle();

	// 	c3->Update();
	// 	TH2F *h12 = new TH2F("h12", "tau_led vs summCharge NONKOLLIMATED", 200, 0 ,8e-10, 200, -10, 5);
	// 	c3->cd(4);
	// 	t->Draw("Ach0.fTimeLED - Ach3.fTimeLED : Ach0.fChargeLED + Ach3.fChargeLED >> h12","","col");
	// 	//x
	// 	h12->GetXaxis()->SetTitle("Summ_Charge [kl]");
	// 	h12->GetXaxis()->CenterTitle();
	// 	//y
	// 	h12->GetYaxis()->SetTitle("tauLED [ns]");
	// 	h12->GetYaxis()->CenterTitle();
	// 	c3->Update();


	// 	c3->Print(Form("../macros/picsDRS4/%s/tauVSCHARGE%s", foldername, ext));
	// }

// 	if (can4) {

// 		gStyle->SetOptFit();

// 		TCanvas *c4 = new TCanvas("c4", "time rising edge and full charge");
// 		c4->Divide(2,2);
// 		c4->cd(1);
// 		TH1F *h13 = new TH1F("h13", "Time front line crossing zero line trigg",300, 38., 52. );
// 		t->Draw("Ach0.fTimeFront >> h13","Ach0.fTimeFront > 38 && Ach0.fTimeFront < 52","");
// 		h13->GetXaxis()->SetRangeUser(38, 52);
// 		h13->GetXaxis()->SetTitle("TimeFront [ns]");
// 		h13->GetXaxis()->CenterTitle();
// 		g1->SetRange(49.35,49.9);
// 		h13->Fit("g1","R");

// 		c4->cd(2);
// 		TH1F *h141 = new TH1F("h141", "Time front line crossing zero line nontrigg",300, 38., 52. );
// 		t->Draw("Ach1.fTimeFront >> h141","Ach1.fTimeFront > 38 && Ach1.fTimeFront < 52 ","");
// 		h141->GetXaxis()->SetRangeUser(38, 52);
// 		h141->GetXaxis()->SetTitle("TimeFront [ns]");
// 		h141->GetXaxis()->CenterTitle();
// 		g1->SetRange(49.35,49.9);
// 		//h14->Fit("g1","R");

// 		c4->cd(3);
// 		TH1F *h16 = new TH1F("h14", "tau Front Time",300, -10, 10. );
// 		t->Draw("Ach0.fTimeFront - Ach1.fTimeFront");
// 		h16->GetXaxis()->SetRangeUser(38, 52);
// 		h16->GetXaxis()->SetTitle("tau Front time [ns]");
// 		h16->GetXaxis()->CenterTitle();
// 		g1->SetRange(49.35,49.9);
// 		//h14->Fit("g1","R");

// 		c4->cd(4);
// 		TH1F *h15 = new TH1F("h15", "summCharge",300, 0., 0.4. );
// 		t->Draw("Ach0.fChargeLED + Ach1.fChargeLED>>h15","Ach0.fChargeLED + Ach1.fChargeLED<0.4","");
// 		h15->GetXaxis()->SetRangeUser(0, 0.4);
// 		h15->GetXaxis()->SetTitle("FullCharge");
// 		h15->GetXaxis()->CenterTitle();
// 		//h14->Fit("g1","R");
// 		c4->Print("../data/dataTektronix/GSItests/1000V_trigg10mv/30_06_10mv_35mv/pic/front&Charge.gif");

// 	}

// 	if (can5) {
// 		TCanvas *c5 = new TCanvas("c5", "Changing CFD parameters");
// 		c5->Divide(2,2);
// 		c5->cd(1);

// 		rMin[0]= 138.4; rMax[0] = 140.4;
// 		rMin[1]= 137; rMax[1] = 138.4;
// 		rMin[2]= 136.4; rMax[2] = 142.4;
// 		rMin[3]= 135.2; rMax[3] = 141.4;


// //		TF1 *gaus5 = new TF1("gaus5","TMath:gaus(0)");
// 		TF1 *g5 = new TF1("g5","gaus",135,140);
// 		g5->SetParLimits(0,1e+2,1e+4);
// 		g5->SetParLimits(1,10,1000);
// 		g5->SetParLimits(2,0.1,10);

// 		TH1F *h12 = new TH1F("h12", "tau_CFD triggered channel for NONcollimated data",1000, 0., 50);

// 		t->Draw("Ach0.fTimeCFD >> h12","","");
// 		//h12->GetXaxis()->SetRangeUser(132, 150);
// 		h12->GetXaxis()->SetTitle("tauCFD_{0} [ns]");
// 		h12->GetXaxis()->CenterTitle();
// 		g5->SetRange(rMin[0],rMax[0]);
// 		//h12->Fit("g5","R");
// 		cout<<"sigma= "<<g5->GetParameter(2)<<endl;

// 		c5->cd(2);
// 		TH1F *h13 = new TH1F("h13", "tau LED triggered channel for NONcollimated data",1000, 0., 50);

// 		t->Draw("Ach0.fTimeLED >> h13","","");
// 		//h13->GetXaxis()->SetRangeUser(132, 150);
// 		h13->GetXaxis()->SetTitle("tauLED_{0} [ns]");
// 		h13->GetXaxis()->CenterTitle();
// 		g5->SetRange(rMin[1],rMax[1]);
// 		//h13->Fit("g5","R");
// 		cout<<"sigma= "<<g5->GetParameter(2)<<endl;

// 		c5->cd(3);
// 		TH1F *h14 = new TH1F("h14", "tau_CFD nontriggered channel for NONcollimated data",1000	, 0., 50);

// 		t->Draw("Ach1.fTimeCFD >> h14","","");
// 		//h14->GetXaxis()->SetRangeUser(132, 150);
// 		h14->GetXaxis()->SetTitle("tauCFD_{0} [ns]");
// 		h14->GetXaxis()->CenterTitle();
// 		g5->SetRange(rMin[2],rMax[2]);
// 		//h14->Fit("g5","R");
// 		cout<<"sigma= "<<g5->GetParameter(2)<<endl;

// 		c5->cd(4);
// 		TH1F *h15 = new TH1F("h15", "tau LED nontriggered channel for NONcollimated data",1000	, 0., 50);

// 		t->Draw("Ach1.fTimeLED >> h15","","");
// 		//h15->GetXaxis()->SetRangeUser(132, 	150);
// 		h15->GetXaxis()->SetTitle("tauLED_{0} [ns]");
// 		h15->GetXaxis()->CenterTitle();
// 		g5->SetRange(rMin[3],rMax[3]);
// 		//h15->Fit("g5","R");
// 		cout<<"sigma= "<<g5->GetParameter(2)<<endl;
// 		c5->Update();

// 		//c5->Print(Form("../macros/picsDRS4/%s/NONcollimatedT%s", foldername, ext));
// 	}

// 	if (can6) {
// 		TCanvas *c6 = new TCanvas("c6", "Changing CFD parameters");
// 		c6->Divide(2,2);
// 		TF1 *g6 = new TF1("g6","gaus",135,140);
// 		//g6->SetParLimits(0,1e+2,1e+4);
// 		//g6->SetParLimits(1,10,1000);
// 		//g6->SetParLimits(2,0.1,10);

// 		rMin[0]= 138.8; rMax[0] = 140.2;
// 		rMin[1]= 137.4; rMax[1] = 138.4;
// 		rMin[2]= 125.6; rMax[2] = 131.6;
// 		rMin[3]= 125.8; rMax[3] = 131.4;

// 		c6->cd(1);

// 		TH1F *h16 = new TH1F("h16", "tau_CFD triggered channel for collimated data",1000, 0., 200);

// 		t1->Draw("Ach0.fTimeCFD >> h16","Ach0.fTimeCFD>132 && Ach0.fTimeCFD<142","");
// 		h16->GetXaxis()->SetRangeUser(120, 150);
// 		h16->GetXaxis()->SetTitle("tauCFD_{0} [ns]");
// 		h16->GetXaxis()->CenterTitle();
// 		g6->SetRange(rMin[0],rMax[0]);
// 		h16->Fit("g6","R");
// 		cout<<"sigma= "<<g6->GetParameter(2)<<endl;


// 		c6->cd(2);
// 		TH1F *h17 = new TH1F("h17", "tau LED triggered channel for collimated data",1000, 0., 200);

// 		t1->Draw("Ach0.fTimeLED >> h17","Ach0.fTimeLED>132 && Ach0.fTimeLED<142","");
// 		h17->GetXaxis()->SetRangeUser(120, 150);
// 		h17->GetXaxis()->SetTitle("tauLED_{0} [ns]");
// 		h17->GetXaxis()->CenterTitle();
// 		g6->SetRange(rMin[1],rMax[1]);
// 		h17->Fit("g6","R");
// 		cout<<"sigma= "<<g6->GetParameter(2)<<endl;

// 		c6->cd(3);

// 		TH1F *h18 = new TH1F("h18", "tau_CFD nontriggered channel for collimated data",1000	, 0., 200);

// 		t1->Draw("Ach1.fTimeCFD >> h18","Ach1.fTimeCFD > 120 && Ach1.fTimeCFD<180","");
// 		h18->GetXaxis()->SetRangeUser(120, 150);
// 		h18->GetXaxis()->SetTitle("tauCFD_{0} [ns]");
// 		h18->GetXaxis()->CenterTitle();
// 		g6->SetRange(rMin[2],rMax[2]);
// 		h18->Fit("g6","R");
// 		cout<<"sigma= "<<g6->GetParameter(2)<<endl;

// 		c6->cd(4);
// 		TH1F *h19 = new TH1F("h19", "tau LED nontriggered channel for collimated data",1000	, 0., 200);

// 		t1->Draw("Ach1.fTimeLED >> h19","Ach1.fTimeLED>120 && Ach1.fTimeLED<180","");
// 		h19->GetXaxis()->SetRangeUser(120, 	150);
// 		h19->GetXaxis()->SetTitle("tauLED_{0} [ns]");
// 		h19->GetXaxis()->CenterTitle();
// 		g6->SetRange(rMin[3],rMax[3]);
// 		h19->Fit("g6","R");
// 		cout<<"sigma= "<<g6->GetParameter(2)<<endl;

// 		c6->Update();
// 		c6->Print(Form("../macros/picsDRS4/%s/collimatedT%s", foldername, ext));
// 	}


}
