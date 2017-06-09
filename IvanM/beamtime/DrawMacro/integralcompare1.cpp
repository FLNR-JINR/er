#include "TH1F.h"

void integralcompare1(	const char *foldername = "report",	const char *ext = ".gif")
{
	// gSystem->Load("../libData.so");
	using std::cout;
	using std::endl;

	TFile *fr = new TFile("/home/ivanm/er/IvanM/NeuRad/expData/dataDSR4/drs4.analize.root");
	TTree *tr = (TTree*)fr->Get("cbmsim");

	const Int_t noBranches = 2;
	Double_t ZeroTime[4],mh[4],RisTime[4],decayT[4];
	Int_t iZero[4],PosZero[4],deltaT[4],Eh[4],Bh[4]; 
	TString bName;
	AEvent *aevent[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent[j] = new AEvent();	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j+1);
		tr->SetBranchAddress(bName.Data(), &aevent[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	TString hname,Chname;
		TH1F *hist[2];
		for (Int_t i = 0; i < 2; i++) {
			hname.Form("hist%d",i);
			Chname.Form("Integral form of the signal",i);
			hist[i] = new TH1F(hname.Data(), Chname.Data(), 1024, 0, 200);
		}

		TF1 *fit1 = new TF1("fit1","[0]*exp(-(x+[2])*[1])");
			//fit1->SetRange(-15,-5);
			fit1->SetParName(1,"tD");

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

					for(Int_t i = 0; i<1024; i++){
						if( ((i+deltaT[0])>-1) && ((i+deltaT[0])<1024))	hist[0]->AddBinContent(i,aevent[0]->GetOnefAmpPos(i+deltaT[0]));
						if( ((i+deltaT[1])>-1) && ((i+deltaT[1])<1024))	hist[1]->AddBinContent(i,aevent[1]->GetOnefAmpPos(i+deltaT[1]));
						//if( ((i+deltaT[2])>-1) && ((i+deltaT[2])<1024))	hist[2]->AddBinContent(i,aevent[2]->GetOnefAmpPos(i+deltaT[2]));
						//if( ((i+deltaT[3])>-1) && ((i+deltaT[3])<1024))	hist[3]->AddBinContent(i,aevent[3]->GetOnefAmpPos(i+deltaT[3]));
					}
			}
		TCanvas *c4 = new TCanvas("c4","test",10,10,1000,600);
			c4->Divide(2,2);
			i=0;
			//for(Int_t i=0;i<2;i++)
			{
				c4->cd(1);
				// c1->cd(i+1);
				hist[i]->GetYaxis()->SetTitle("Signal [V]");
				hist[i]->GetYaxis()->CenterTitle();
				// hist[i]->GetXaxis()->SetRangeUser(120, 180);
				hist[i]->GetXaxis()->SetTitle("Time [ns]");
				hist[i]->GetXaxis()->CenterTitle();

				hist[i]->Draw();

				c4->Update();
				c4->cd(2);
				i=1;
				hist[i]->GetYaxis()->SetTitle("Signal [V]");
				hist[i]->GetYaxis()->CenterTitle();
				// hist[i]->GetXaxis()->SetRangeUser(120, 180);
				hist[i]->GetXaxis()->SetTitle("Time [ns]");
				hist[i]->GetXaxis()->CenterTitle();

				hist[i]->Draw();

				c4->Update();
			}


//////////////////////////////////////////
			///////////////////////////////
			/////////////////////////
			///////////////////////////////


	TFile *frs = new TFile("/home/ivanm/er/IvanM/NeuRad/oscill/data/analyze1.root");
	TTree *trs = (TTree*)frs->Get("cbmsim");

	const Int_t noBranches1 = 2;
	Double_t ZeroTime1[4],mh1[4],RisTime1[4],decayT1[4];
	Int_t iZero1[4],PosZero1[4],deltaT1[4],Eh1[4],Bh1[4]; 
	TString bName1;
	AEvent *aevent1[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches1; j++) {
		aevent1[j] = new AEvent(1000);	//each raw event element is of class RawEvent()
		bName1.Form("Ach%d.", j+1);
		trs->SetBranchAddress(bName1.Data(), &aevent1[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	TString hname1,Chname1;
		TH1F *histo[2];
		for (Int_t i = 0; i < 2; i++) {
			hname1.Form("histo%d",i);
			Chname1.Form("Integral form of the signal",i);
			histo[i] = new TH1F(hname1.Data(), Chname1.Data(), 1000, 0, 200);
		}

			Long64_t nEntries1 = trs->GetEntries();
			//loop over events
			for (Int_t j = 0; j < nEntries1; j++) {
				//cout<<"found event number "<<j<<endl;
				trs->GetEntry(j);
				for(Int_t k=0;k<2;k++) { // loop for channels
					ZeroTime1[k] = aevent1[k]->GetfCFD(); // get fCFD for channel №k
					iZero1[k] = ZeroTime1[k]*10;
					if(j==0) { PosZero1[k] = iZero1[k]; }
					deltaT1[k] = iZero1[k] - PosZero1[k];
				}

					for(Int_t i = 0; i<1000; i++){
						if( ((i+deltaT1[0])>-1) && ((i+deltaT1[0])<1000))	histo[0]->AddBinContent(i,aevent1[0]->GetOnefAmpPos(i+deltaT1[0]));
						if( ((i+deltaT1[1])>-1) && ((i+deltaT1[1])<1000))	histo[1]->AddBinContent(i,aevent1[1]->GetOnefAmpPos(i+deltaT1[1]));
						//if( ((i+deltaT[2])>-1) && ((i+deltaT[2])<1024))	hist[2]->AddBinContent(i,aevent[2]->GetOnefAmpPos(i+deltaT[2]));
						//if( ((i+deltaT[3])>-1) && ((i+deltaT[3])<1024))	hist[3]->AddBinContent(i,aevent[3]->GetOnefAmpPos(i+deltaT[3]));
					}
			}
			//c1->Divide(2,2);
			i=0;
			//for(Int_t i=0;i<2;i++)
			{
				c4->cd(3);
				histo[i]->GetYaxis()->SetTitle("Signal [V]");
				histo[i]->GetYaxis()->CenterTitle();

				histo[i]->GetXaxis()->SetTitle("Time [ns]");
				histo[i]->GetXaxis()->CenterTitle();
				// histo[i]->GetXaxis()->SetRangeUser(0, 60);
				histo[i]->Draw();
				i=1;
				c4->cd(4);
				histo[i]->GetYaxis()->SetTitle("Signal [V]");
				histo[i]->GetYaxis()->CenterTitle();

				histo[i]->GetXaxis()->SetTitle("Time [ns]");
				histo[i]->GetXaxis()->CenterTitle();
				// histo[i]->GetXaxis()->SetRangeUser(0, 60);
				histo[i]->Draw();
				c4->Update();
			}













}
