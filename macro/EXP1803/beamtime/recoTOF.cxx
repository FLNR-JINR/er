#if !defined(__CLING__)


#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TRandom3.h"

#include "~/Acculinna/ER/beamtime/data/ERBeamTimeEventHeader.h"

#include "~/Acculinna/ER/BeamDet/data/ERBeamDetTOFDigi.h"


#include <iostream>

using std::cout;
using std::endl;

#endif

void Reset();


void TOF(TClonesArray *dataF3, TClonesArray *dataF5);

Int_t trigger;
Double_t dE;
Double_t ToF;
Double_t tF3, tF5;

void recoTOF() {

	TString inFile =  "he8_10_0010.Digi.root";
	TString outFile = "he8_10_0010.TOF.reco.root";

	TString inPath =  "~/Acculinna/ER/showBeam_VC/analysed/";
	TString outPath = "~/Acculinna/ER/showBeam_VC/reco/";

	//////////////////////////
	//Input file
	//////////////////////////
	TFile *fr = new TFile(inPath + inFile);
	TTree *tr = (TTree*)fr->Get("er");

	cout << "Founding " << tr->GetEntries() << " entries" << endl;

	ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();

	
	TClonesArray *v_F3 = new TClonesArray("ERBeamDetTOFDigi");
	TClonesArray *v_F5 = new TClonesArray("ERBeamDetTOFDigi");
	
	//tr->Print();
	tr->SetBranchAddress("EventHeader.", &header);

	printf("Yey! %i\n", tr->SetBranchAddress("BeamDetToFDigi1", &v_F3));
	printf("Yey! %i\n", tr->SetBranchAddress("BeamDetToFDigi2", &v_F5));
	
	//////////////////////////
	//output file
	//////////////////////////
	TFile *fw = new TFile(outPath + outFile, "RECREATE");
	TTree *tw = new TTree("reco", "Selected reconstructed variables");

//	Int_t trigger;
	tw->Branch("trigger", &trigger, "trigger/I");
	tw->Branch("dE",  &dE,  "dE/D");
	tw->Branch("ToF", &ToF, "ToF/D");
	tw->Branch("tF3", &tF3, "tF3/D");
	tw->Branch("tF5", &tF5, "tF5/D");

	Long64_t noEntries = tr->GetEntries();


	//todo: first event is triggered by 1 for some reason
	for(Long64_t nentry = 1; nentry < noEntries; nentry++) {

		if(nentry%100000==0) cout << "#Event " << nentry << "#" << endl;

		Reset();

		tr->GetEntry(nentry);

	    trigger = header->GetTrigger();

	    TOF(v_F3, v_F5);

	    tw->Fill();

	}
	/*
	TCanvas *c1 = new TCanvas("c1", "PID - dE vs ToF", 1000, 1000);
	c1->Divide(1,2);

	c1->cd(1);
	tw->Draw("dE:ToF>>(1000, 0, 1000, 5000, 0, 10000)", "", "col");

	c1->cd(2);
	tw->Draw("trigger", "");
	c1->Update();*/  // I would like to plot PID picture here 
	fw->cd();
	tw->Write();
	fw->Close();



}

void Reset() {
	trigger = 0;

}



void TOF(TClonesArray *dataF3, TClonesArray *dataF5) {

	//cout << "F3 " << dataF3->GetEntriesFast() << endl;
	//cout << "F5 " << dataF5->GetEntriesFast() << endl;
	
	Int_t F3 = 0;
	Int_t F5 = 0;
	
	if (dataF3)
	{
		F3 = dataF3->GetEntriesFast();

	}
	if (dataF5)
	{
		F5 = dataF5->GetEntriesFast();
	}

	if (F5 == 1) {
		Double_t dEbeam;
			for (Int_t i = 0; i < 1; i++) {
				//printf("dE 's here \t%i\n",i );
				dEbeam = ((ERBeamDetTOFDigi*)dataF5->At(i))->GetEdep();
				//cout << " dE = " << dEbeam << endl;
				
				dE = dEbeam;
			}

			
	}

	Double_t TIME = 0.;
	const Double_t timeCal = 0.125;
	const Double_t constTOF = 89.165;
	
	if (F3 == 1 && F5 == 1) {
	
		Float_t timeF3, timeF5;
		for (Int_t i = 0; i < 1; i++) {
				//printf("hi\t%i\n",i );
				timeF3 = ((ERBeamDetTOFDigi*)dataF3->At(i))->GetTime();
				//cout<< " Time F3 = " << timeF3 <<endl;
				tF3 = timeF3;
			}
	
		
		for (Int_t i = 0; i < 1; i++) {
				//printf("helllllllloo\t%i\n",i );
				timeF5 = ((ERBeamDetTOFDigi*)dataF5->At(i))->GetTime();
			    //cout << " Time F5 = " << timeF5 <<endl;
			    tF5 = timeF5;
			}
			
			TIME = (timeF5 - timeF3) + constTOF; // I'm not sure in Digi file 
			// do we take time caliration or not? That's why I didn't put timeCal here
			//cout << " TOF = " << TIME << endl;
			ToF = TIME;
	
	}	
	
}
