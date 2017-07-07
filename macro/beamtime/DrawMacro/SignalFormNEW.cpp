#include "TH1F.h"

void SignalFormNEW(	const char *foldername = "report",	const char *ext = ".gif")
{
	gSystem->Load("../libData.so");
	using std::cout;
	using std::endl;

	TFile *f = new TFile("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	//TFile fr("../data/dataTektronix/GSItests/1000V_trigg40mv/30_60_10_50_GSI.root");
	TTree *tr = (TTree*)f->Get("atree");

	const Int_t noBranches = 2;
	Double_t ZeroTime[4],mh[4],RisTime[4],decayT[4];
	Int_t iZero[4],PosZero[4],deltaT[4],Eh[4],Bh[4]; 
	TString bName;
	AEvent *aevent[noBranches];	// pointer to the array (of RawEvent class) in which raw data for each channel will be put
	for (Int_t j = 0; j<noBranches; j++) {
		aevent[j] = new AEvent(1000);	//each raw event element is of class RawEvent()
		bName.Form("Ach%d.", j);
		tr->SetBranchAddress(bName.Data(), &aevent[j]);	//read the tree tr with raw data and fill array revent with raw data
		//cout << tr->SetBranchAddress(bName.Data(), &aevent[j]) << endl;	//read the tree tr with raw data and fill array revent with raw data
	}

	TString hname,Chname;
		TH1F *hist[2];
		for (Int_t i = 0; i < 2; i++) {
			hname.Form("hist%d",i);
			Chname.Form("Integral form of the signal",i);
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
						//if( ((i+deltaT[2])>-1) && ((i+deltaT[2])<1024))	hist[2]->AddBinContent(i,aevent[2]->GetOnefAmpPos(i+deltaT[2]));
						//if( ((i+deltaT[3])>-1) && ((i+deltaT[3])<1024))	hist[3]->AddBinContent(i,aevent[3]->GetOnefAmpPos(i+deltaT[3]));
					}
			}

	/*	for(Int_t j=0;j<2;j++) {
		//	j=0;
			mh[j] = hist[j]->GetBinContent(hist[j]->GetMaximumBin());

			for(Int_t i = hist[j]->GetMaximumBin(); i>0;i--) {	// finding the
				if( hist[j]->GetBinContent(i)<0.9*mh[j] ) {
					Eh[j] = i+1;
					break;
				}
			}	return;
			for(Int_t i = hist[j]->GetMaximumBin(); i>0;i--) {	// finding the
				if( hist[j]->GetBinContent(i)<0.1*mh[j] ) {
					Bh[j] = i+1;
					break;
				}
			}
			RisTime[j] = (Eh[j] - Bh[j])*0.1; // время нарастания
		}
		Double_t rMin[4];
		Double_t rMax[4];
		rMin[0]= 141.8; rMax[0] = 150;
		rMin[1]= 151; rMax[1] = 158.2;
		rMin[2]= 136.8; rMax[2] = 145;
		rMin[3]= 152.3; rMax[3] = 161;*/
		TCanvas *c1 = new TCanvas("c1","test",10,10,1000,600);
			//c1->Divide(2,2);
			i=0;
			//for(Int_t i=0;i<2;i++)
			{
				//c1->cd(i+1);
				hist[i]->GetYaxis()->SetTitle("Signal [V]");
				hist[i]->GetYaxis()->CenterTitle();

				hist[i]->GetXaxis()->SetTitle("Time [ns]");
				hist[i]->GetXaxis()->CenterTitle();

				hist[i]->GetXaxis()->SetRangeUser(75, 135);

				hist[i]->Draw();
				//fit1->SetParLimits(0,-3e+30,-1e+10);
				//fit1->SetParLimits(1,1e-2,1);
				//fit1->SetRange(rMin[i],rMax[i]);
				//hist[i]->Fit("fit1","R");
				//decayT[i] = 1/(fit1->GetParameter(1));
				c1->Update();
			}

			/*for(Int_t i=0;i<4;i++) {
				cout<<decayT[i]<<" that's decay time  of channel number "<<i<<endl;
				cout<<RisTime[i]<< " that's a RiseTime for this channel "<<i<<endl;
			}
			c1->Print(Form("../macros/picsDRS4/%s/IntegralForm%s", foldername, ext));*/
}
