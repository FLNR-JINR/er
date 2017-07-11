#include <TTree.h>
#include <TGraph.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include "TStyle.h"
#include "TArrayF.h"

Double_t Func(Double_t time, Double_t amplitude, Float_t shift){
        //Аналитическая функция одноэлектронного сигнала
        return 8.*amplitude*(time-shift)*(time-shift)*TMath::Exp(-(time-shift)/0.45);
}

void testShowPE()
{
	using std::cout;
	using std::endl;

	gStyle->SetCanvasDefH(900);
	gStyle->SetCanvasDefW(1500);

	const Long64_t kFirstEvent = 2;

	TFile fr("/store/ivan/tests/analyze.root");
	TCanvas *c1 = new TCanvas("c1","test",10,10,1000,600);
//	c1->Divide(4,4);
	TTree *tr = (TTree*)fr.Get("cbmsim");
	AEvent *revent = new AEvent(1000);
	tr->SetBranchAddress("Ach1.",&revent);
	Int_t nevents = tr->GetEntries();
	TGraph *gr[16];
	Int_t PECount;
	Double_t StartTime;
	Float_t fPEamp[1000],fPEtime[1000];
	TArrayF fPEAmps, fPETimes;
	//for (Int_t k = 0; k < 16; k++){
	Int_t k=0;
		tr->GetEntry(k+kFirstEvent);
		fPEAmps = revent->GetPEAmps();
		fPETimes = revent->GetPETimes();

		gr[k] = new TGraph(*revent->GetGraphSignal());
		PECount = fPEAmps.GetSize(); cout<<endl<<PECount<<" number of photoelectrons "<<endl;
		
		Double_t **sigfunc = new Double_t *[PECount];
		Double_t **timefunc = new Double_t *[PECount];
		for(Int_t i=0; i< PECount; i++) {
			sigfunc[i] = new Double_t [40];
			timefunc[i] = new Double_t [40];
		}
		for(i=0; i<PECount; i++) {
			for(Int_t j=0;j<40;j++) {
				sigfunc[i][j] = Func(fPETimes[i] + j*0.1,fPEAmps[i],fPETimes[i]);
				timefunc[i][j] = fPETimes[i] + j*0.1;
			}		
		}
		TGraph **pe = new TGraph *[PECount];
		for(i=0; i< PECount; i++) {
			pe[i] = new TGraph(40,timefunc[i],sigfunc[i]);
		}		
		//c1->cd(k+1);
		
		gr[k]->GetXaxis()->SetRangeUser(5, 25);

		// gr[k]->GetXaxis()->SetTitle("Time [ns]");
		// gr[k]->GetXaxis()->CenterTitle();
		// gr[k]->GetYaxis()->SetTitle("Signal [V]");
		// gr[k]->GetYaxis()->CenterTitle();
		gr[k]->Draw("AL*");
	        for(i=0; i< PECount; i++) { pe[i]->SetLineColor(3); pe[i]->Draw("SAME");}
		// cout << t10[k] << "\t" << t90[k] << endl;
		// f1->SetRange(t10[k], t90[k]);
		// gr[k]->Fit(f1, "RQ");
	//}
	//c1->Print("shape.png");
	//c1->Print(Form("../macros/picsDRS4/file%s/signals%s", foldername, ext));
//	c1->cd(2);
//	tr->Draw("gAmp.Draw()","","goff",1,124);
}
