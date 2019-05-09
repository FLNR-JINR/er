#if !defined(__CLING__)

#include "TFile.h"
//#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"

#include "~/ER/beamtime/data/ERBeamTimeEventHeader.h"


#include <iostream>

using std::cout;
using std::endl;

#endif

void draw() {

	TString inFile = "he8_10_Left.reco.root";
	TString inPath = "~/media/analysis_nas/exp201810/He8/reco/";

	TFile *fr = new TFile(inPath + inFile);
	TTree *tr = (TTree*)fr->Get("er");

	/*TFile *frIn = new TFile("~/Acculinna/ExperRoot/macro/EXP1803/beamtime/he8_10_0010.Digi.root");
	TTree *trIn = (TTree*)frIn->Get("er");
	tr->AddFriend(trIn);*/

	TCanvas *c1 = new TCanvas("c1", "BEAM projections", 1500, 1000);
	c1->Divide(1,1);
	//BeamDetTrack
	c1->cd(1);
	tr->SetMarkerColor(kBlack);
	//tr->Draw("fYt:fXt", "EventHeader.fTrigger==2 && BeamDetTrack.fYt>-2. && BeamDetTrack.fXt>-2.");
	tr->Draw("fYt:fXt", " BeamDetTrack.fYt>-2. && BeamDetTrack.fXt>-2.","col");
	tr->SetMarkerColor(kRed);
	//tr->Draw("fYt:fXt", " (EventHeader.fTrigger==2 && (BeamDetTrack.fYt)^2 + (BeamDetTrack.fXt)^2 ) < 8.^2", "same");
	c1->Update();
	
}

