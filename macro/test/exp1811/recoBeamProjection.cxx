#if !defined(__CLING__)


#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TRandom3.h"

#include "~/Acculinna/ExperRoot/beamtime/data/ERBeamTimeEventHeader.h"
#include "~/Acculinna/ExperRoot/BeamDet/data/ERBeamDetMWPCDigi.h"

#include <iostream>

using std::cout;
using std::endl;

#endif

void Reset();
void MWPCprojection(TClonesArray *dataX1, TClonesArray *dataY1, TClonesArray *dataX2, TClonesArray *dataY2);

Int_t   GetClusterMult(TClonesArray *data);
Float_t GetClusterWire(TClonesArray *data);
Float_t GetPosition(Float_t wire, Float_t wireStep, Float_t planeOffset);

Int_t trigger;

Float_t fXt;
Float_t fYt;
bool fFill = false;

void recoBeamProjection() {

	TString inFile =  "he8_10_0010.Digi.root";
	TString outFile = "he8_10_0010.reco.root";

	TString inPath =  "./";
	TString outPath = "./";

	//////////////////////////
	//Input file
	//////////////////////////
	TFile *fr = new TFile(inPath + inFile);
	TTree *tr = (TTree*)fr->Get("er");

//	TChain *tr = new TChain("er");
//	tr->Add("~/data/exp1811/analysed/he8_07_000*");

//	tr->StartViewer();
	cout << "Found " << tr->GetEntries() << " entries" << endl;

	ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();

	TClonesArray *v_MWPCx1 = new TClonesArray("ERBeamDetMWPCDigi");
	TClonesArray *v_MWPCy1 = new TClonesArray("ERBeamDetMWPCDigi");
	TClonesArray *v_MWPCx2 = new TClonesArray("ERBeamDetMWPCDigi");
	TClonesArray *v_MWPCy2 = new TClonesArray("ERBeamDetMWPCDigi");

	tr->SetBranchAddress("EventHeader.",&header);

	tr->SetBranchAddress("BeamDetMWPCDigiX1",&v_MWPCx1);
	tr->SetBranchAddress("BeamDetMWPCDigiX2",&v_MWPCx2);
	tr->SetBranchAddress("BeamDetMWPCDigiY1",&v_MWPCy1);
	tr->SetBranchAddress("BeamDetMWPCDigiY2",&v_MWPCy2);

	//////////////////////////
	//output file
	//////////////////////////
	TFile *fw = new TFile(outPath + outFile, "RECREATE");
	TTree *tw = new TTree("reco", "Selected reconstructed variables");

//	Int_t trigger;
	tw->Branch("trigger",&trigger,"trigger/I");

	tw->Branch("fXt",&fXt,"fXt/F");
	tw->Branch("fYt",&fYt,"fYt/F");

	Long64_t noEntries = tr->GetEntries();
//	Long64_t noEntries = 100000;

	//todo: first event is triggered by 1 for some reason
	for(Long64_t nentry = 1; nentry < noEntries; nentry++) {

		if(nentry%100000==0) cout << "#Event " << nentry << "#" << endl;

		Reset();

		tr->GetEntry(nentry);

	    trigger = header->GetTrigger();

	    MWPCprojection(v_MWPCx1, v_MWPCy1, v_MWPCx2, v_MWPCy2);
	    if (fFill) {
		    tw->Fill();
	    }

	}

	fw->cd();
	tw->Write();
	fw->Close();

}

void Reset() {
	trigger = 0;
	fFill = true;
	fXt = -50.;
	fYt = -50.;
}

Int_t GetClusterMult(TClonesArray *data)
{

	if (!data) return 0;

	Int_t entries = data->GetEntriesFast();

	if (entries<2) return entries;

	Int_t wire1, wire2;
	Int_t noclusters = 1;

	for (Int_t i = 1; i < entries; i++) {
		//check if entries are in specific order
		wire1 = ((ERBeamDetMWPCDigi*)data->At(i))->GetWireNb();
		wire2 = ((ERBeamDetMWPCDigi*)data->At(i-1))->GetWireNb();

//		cout << wire2 << "\t" << wire1 << endl;
		//todo number 32 is related to number of wires
		// and should be taken from Parameters
		if ( abs(wire1 - wire2) > 1 && abs(wire1 - wire2) < 32) noclusters++;
	}

	return noclusters;
}

Float_t GetClusterWire(TClonesArray *data)
{
	//working correctly only for cluster multiplicity equal to 1
	//get wire number in, generally, half-numbers, i.e. if cluster
	//consists from 2 wires n and n+1, cluster position is n+0.5

	Int_t wire1 = ((ERBeamDetMWPCDigi*)data->At(0))->GetWireNb();

	return (Float_t)wire1 + 0.5*((Float_t)data->GetEntriesFast()-1);

} //--------------------------------------------------------------------

Float_t GetPosition(Float_t wire, Float_t wireStep,
		Float_t planeOffset) {
	//TODO: number of wires (16) as parameter
	//TODO: omit gRandom
	return (wire + /*gRandom->Uniform(-0.5, 0.5)*/ - 16.5)*wireStep + planeOffset;
}

void MWPCprojection(TClonesArray *dataX1, TClonesArray *dataY1, TClonesArray *dataX2, TClonesArray *dataY2) {

	// if (GetClusterMult(dataX1)!= 1 || GetClusterMult(dataY1)!=1 || GetClusterMult(dataX2)!=1 || GetClusterMult(dataY2)!=1) {
	// 	fFill = false;
	// 	return;
	// }
	if (dataX1->GetEntriesFast() !=1 // multiplicity not equals unit
	    || dataY1->GetEntriesFast()!=1 
	    || dataX2->GetEntriesFast() !=1 
	    || dataY2->GetEntriesFast()!=1)
	{
		fFill = false;
		return;
	}

	const Float_t fMWPCwireStepX1 = -1.25;
	const Float_t fMWPCwireStepY1 =  1.25;		//step between two wires
	const Float_t fMWPCwireStepX2 = -1.25;		//step between two wires
	const Float_t fMWPCwireStepY2 =  1.25;		//step between two wires

	const Float_t fMWPC1_X_offset = 0.;//-1.19;
	const Float_t fMWPC1_Y_offset = 0.;//-2.12;
	const Float_t fMWPC2_X_offset = 0.;// 0.2;
	const Float_t fMWPC2_Y_offset = 0.;//-1.02;

	const Float_t fMWPCz1 = -815.;	//z coordinate of the center of MWPC1 [mm]
	const Float_t fMWPCz2 = -270.;	//z coordinate of the center of MWPC2

	Float_t x1c, y1c, x2c, y2c, xtc, ytc;

	Float_t wireC[4]; //x1, y1, x2, y2

	TClonesArray* v_MWPC[4] = { dataX1, dataY1, dataX2, dataY2 };	//x1, y1, x2, y2


	for (Int_t i = 0; i < 4; i++) {
		wireC[i] = GetClusterWire(v_MWPC[i]);
	}

	//cluster multiplicity equal to 1
	x1c = GetPosition(wireC[0], fMWPCwireStepX1, fMWPC1_X_offset);
	y1c = GetPosition(wireC[1], fMWPCwireStepY1, fMWPC1_Y_offset);

	x2c = GetPosition(wireC[2], fMWPCwireStepX2, fMWPC2_X_offset);
	y2c = GetPosition(wireC[3], fMWPCwireStepY2, fMWPC2_Y_offset);


	xtc = x1c - (x2c -x1c)*fMWPCz1/(fMWPCz2-fMWPCz1);
	ytc = y1c - (y2c -y1c)*fMWPCz1/(fMWPCz2-fMWPCz1);

	fXt = xtc;
	fYt = ytc;
}
