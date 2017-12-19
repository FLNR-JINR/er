#include "TH1F.h"

void SignalFormNEW(	const char *foldername = "report",	const char *ext = ".gif")
{
	//gSystem->Load("../libData.so");
	using std::cout;
	using std::endl;

	TFile *f = new TFile("/home/muzalevsky/work/dataER/simNeuRad/analyze.root");
	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TTree *tr = (TTree*)f->Get("cbmsim");

	const Int_t noBranches = 2;
	Double_t ZeroTime[4],mh[4],RisTime[4],decayT[4];
	Int_t iZero[4],PosZero[4],deltaT[4],Eh[4],Bh[4]; 
	TString bName;
	ERNeuRadAEvent *aevent[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent[j] = new ERNeuRadAEvent(1000);	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j+1);
		tr->SetBranchAddress(bName.Data(), &aevent[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	TString hname,Chname;
		TH1F *hist[2];
		for (Int_t i = 0; i < 2; i++) {
			hname.Form("hist%d",i);
			Chname.Form("Integral form of the signal SIM");
			hist[i] = new TH1F(hname.Data(), Chname.Data(), 1000, 0, 200);
		}

		TF1 *fit1 = new TF1("fit1","-[0]*exp(-x*[1])");
			//fit1->SetRange(-15,-5);
			fit1->SetParName(1,"tD");

			Long64_t nEntries = tr->GetEntries();

			//loop over events
			for (Int_t j = 0; j < nEntries; j++) {
				//cout<<"found event number "<<j<<endl;
				tr->GetEntry(j);
				for(Int_t k=0;k<2;k++) { // loop for channels
					ZeroTime[k] = aevent[k]->GetfLED(); // get fCFD for channel №k
					iZero[k] = ZeroTime[k]*10;
					if(j==0) { PosZero[k] = iZero[k]; }
					deltaT[k] = iZero[k] - PosZero[k];
				}

					for(Int_t i = 0; i<1000; i++){
						if( ((i+deltaT[0])>-1) && ((i+deltaT[0])<1000))	hist[0]->AddBinContent(i,aevent[0]->GetOnefAmpPos(i+deltaT[0]));
						if( ((i+deltaT[1])>-1) && ((i+deltaT[1])<1000))	hist[1]->AddBinContent(i,aevent[1]->GetOnefAmpPos(i+deltaT[1]));
					}
			}

		TCanvas *c1 = new TCanvas("c1","testSIM",10,10,1000,600);
			//c1->Divide(2,2);
			Int_t i=0;
			//for(Int_t i=0;i<2;i++)
			{
				//c1->cd(i+1);
				hist[i]->GetYaxis()->SetTitle("Signal [V]");
				hist[i]->GetYaxis()->CenterTitle();

				hist[i]->GetXaxis()->SetTitle("Time [ns]");
				hist[i]->GetXaxis()->CenterTitle();

				//hist[i]->GetXaxis()->SetRangeUser(75, 135);

				hist[i]->Draw();
				c1->Update();
			}






	TFile *f1 = new TFile("/home/muzalevsky/work/dataER/simNeuRad/analyzeEXP.root");
	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TTree *tr1 = (TTree*)f1->Get("cbmsim");

	ERNeuRadAEvent *aevent1[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent1[j] = new ERNeuRadAEvent(1000);	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j+1);
		tr1->SetBranchAddress(bName.Data(), &aevent[j]);	//read the tree tr with raw data and fill array revent with raw data
	}

		TH1F *hist1[2];
		for (Int_t i = 0; i < 2; i++) {
			hname.Form("hist%d",i);
			Chname.Form("Integral form of the signal EXP");
			hist1[i] = new TH1F(hname.Data(), Chname.Data(), 1000, 0, 200);
		}
		nEntries = tr1->GetEntries();

			//loop over events
			for (Int_t j = 0; j < nEntries; j++) {
				//cout<<"found event number "<<j<<endl;
				tr1->GetEntry(j);
				for(Int_t k=0;k<2;k++) { // loop for channels
					ZeroTime[k] = aevent1[k]->GetfLED(); // get fCFD for channel №k
					iZero[k] = ZeroTime[k]*10;
					if(j==0) { PosZero[k] = iZero[k]; }
					deltaT[k] = iZero[k] - PosZero[k];
				}

					for(Int_t i = 0; i<1000; i++){
						if( ((i+deltaT[0])>-1) && ((i+deltaT[0])<1000))	hist1[0]->AddBinContent(i, -700 + 1.5*10000*aevent[0]->GetOnefAmpPos(i+deltaT[0]));
						if( ((i+deltaT[1])>-1) && ((i+deltaT[1])<1000))	hist1[1]->AddBinContent(i, -700 + 1.5*10000*aevent[1]->GetOnefAmpPos(i+deltaT[1]));
					}
			}


		//TCanvas *c2 = new TCanvas("c2","testEXP",10,10,1000,600);
			i=0;
			//for(Int_t i=0;i<2;i++)
			{
				//c1->cd(i+1);
				hist1[i]->GetYaxis()->SetTitle("Signal [V]");
				hist1[i]->GetYaxis()->CenterTitle();

				hist1[i]->GetXaxis()->SetTitle("Time [ns]");
				hist1[i]->GetXaxis()->CenterTitle();
				hist1[i]->SetLineColor(kRed);
				hist1[i]->Draw("SAME");

				//c2->Update();
			}


}
