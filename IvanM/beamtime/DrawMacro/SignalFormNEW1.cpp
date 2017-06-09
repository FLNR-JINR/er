#include "TH1F.h"

void SignalFormNEW1(	const char *foldername = "report",	const char *ext = ".gif")
{
	// gSystem->Load("../libData.so");
	using std::cout;
	using std::endl;

	TFile *f = new TFile("/home/ivanm/er/IvanM/NeuRad/oscill/data/analyze1.root");
	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TTree *tr = (TTree*)f->Get("cbmsim");


	TFile *f1 = new TFile("/home/ivanm/er/IvanM/NeuRad/expData/dataDSR4/drs4.analize.root");
	TTree *tr1 = (TTree*)f1->Get("cbmsim");


	const Int_t noBranches = 2;
	Double_t ZeroTime[4],mh[4],RisTime[4],decayT[4];
	Int_t iZero[4],PosZero[4],deltaT[4],Eh[4],Bh[4]; 
	TString bName;
	AEvent *aevent[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent[j] = new AEvent(1000);	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j+1);
		tr->SetBranchAddress(bName.Data(), &aevent[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	AEvent *aevent1[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent1[j] = new AEvent();	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j+1);
		tr1->SetBranchAddress(bName.Data(), &aevent1[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	TString hname,Chname,hname1,Chname1;
		TH1F *hist[2];
		TH1F *histo[2];
		for (Int_t i = 0; i < 2; i++) {
			hname.Form("hist%d",i);
			Chname.Form("Integral form of the signal",i);
			hname1.Form("histo%d",i);
			Chname1.Form("Integral form of the signalo",i);
			hist[i] = new TH1F(hname.Data(), Chname.Data(), 1000, 0, 200);
			histo[i] = new TH1F(hname1.Data(), Chname1.Data(), 1024, 0, 200);
		}

		TF1 *fit1 = new TF1("fit1","[0]*exp(-(x-[2])*[1])");
		TF1 *fit2 = new TF1("fit2","x*[0]*exp(-(x-[2])*[1])");
			Long64_t nEntries = tr->GetEntries();

			//loop over events
			for (Int_t j = 0; j < nEntries; j++) {
				//cout<<"found event number "<<j<<endl;
				tr->GetEntry(j);
				for(Int_t k=0;k<2;k++) { // loop for channels
					ZeroTime[k] = aevent[k]->GetfCFD(); // get fCFD for channel №k
					iZero[k] = ZeroTime[k]*10;
					if(j==0) { PosZero[k] = iZero[k]; }
					deltaT[k] = iZero[k] - PosZero[k];
				}

					for(Int_t i = 0; i<1000; i++){
						if( ((i+deltaT[0])>-1) && ((i+deltaT[0])<1000))	hist[0]->AddBinContent(i,aevent[0]->GetOnefAmpPos(i+deltaT[0]));
						if( ((i+deltaT[1])>-1) && ((i+deltaT[1])<1000))	hist[1]->AddBinContent(i,aevent[1]->GetOnefAmpPos(i+deltaT[1]));
						//if( ((i+deltaT[2])>-1) && ((i+deltaT[2])<1024))	hist[2]->AddBinContent(i,aevent[2]->GetOnefAmpPos(i+deltaT[2]));
						//if( ((i+deltaT[3])>-1) && ((i+deltaT[3])<1024))	hist[3]->AddBinContent(i,aevent[3]->GetOnefAmpPos(i+deltaT[3]));
					}
					// cout<<aevent[0]->GetOnefAmpPos(500+deltaT[0])<<" "<<500<<endl;
			}

			Long64_t nEntries1 = tr1->GetEntries();

			//loop over events
			for (Int_t j = 0; j < nEntries1; j++) {
				//cout<<"found event number "<<j<<endl;
				tr1->GetEntry(j);
				for(Int_t k=0;k<2;k++) { // loop for channels
					ZeroTime[k] = aevent1[k]->GetfCFD(); // get fCFD for channel №k
					iZero[k] = ZeroTime[k]*10;
					if(j==0) { PosZero[k] = iZero[k]; }
					deltaT[k] = iZero[k] - PosZero[k];
				}

					for(Int_t i = 0; i<1024; i++){
						if( ((i+deltaT[0])>-1) && ((i+deltaT[0])<1024))	histo[0]->AddBinContent(i,aevent1[0]->GetOnefAmpPos(i+deltaT[0]));
						if( ((i+deltaT[1])>-1) && ((i+deltaT[1])<1024))	histo[1]->AddBinContent(i,aevent1[1]->GetOnefAmpPos(i+deltaT[1]));
					}
					// cout<<aevent[0]->GetOnefAmpPos(500+deltaT[0])<<" "<<500<<endl;
			}

		TCanvas *c1 = new TCanvas("c1","test",10,10,1000,600);
		c1->Divide(1,2);
		i=0;
			// for(Int_t i=0;i<2;i++)
			{
				c1->cd(i+1);
				hist[1]->GetYaxis()->SetTitle("Signal [V]");
				hist[1]->GetYaxis()->CenterTitle();

				hist[1]->GetXaxis()->SetTitle("Time [ns]");
				hist[1]->GetXaxis()->CenterTitle();

				hist[1]->GetXaxis()->SetRangeUser(10, 65);
				// histo[1]->GetXaxis()->SetRangeUser(120, 175);

				hist[1]->Draw();
				c1->cd(i+2);
				histo[1]->Draw();
				c1->Update();
			}

}
