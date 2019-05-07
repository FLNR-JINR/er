#if !defined(__CLING__)


//#include "../install/include/conversion/ConvertRepackedToCalibrated.h"
//#include "../install/include/data/DetEventCommon.h"
#include "TFile.h"
//#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"

#include "~/Acculinna/ER/beamtime/data/ERBeamTimeEventHeader.h"


#include <iostream>

using std::cout;
using std::endl;

#endif

void showBeam() {

	TString inFile = "he8_10_0010.reco.root";
	TString inPath = "~/Acculinna/ER/showBeam_VC/reco/";

	TFile *fr = new TFile(inPath + inFile);
	TTree *tr = (TTree*)fr->Get("reco");

	TFile *frIn = new TFile("~/Acculinna/ER/showBeam_VC/analysed/he8_10_0010.Digi.root");
	TTree *trIn = (TTree*)frIn->Get("er");
	tr->AddFriend(trIn);

//	tr->StartViewer();
//	trIn->StartViewer();

////////////////////////////////////////////////
//	c1
////////////////////////////////////////////////

	TCanvas *c1 = new TCanvas("c1", "diagnostics", 1000, 1000);
	c1->Divide(2,2);

	c1->cd(1);
	tr->Draw("trigger", "");
	c1->Update();

	c1->cd(2);
//	tr->Draw("BeamDetMWPCDigiY1.fWireNb:BeamDetMWPCDigiX1.fWireNb", "Length$(BeamDetMWPCDigiX1)>0 && Length$(BeamDetMWPCDigiY1)>0", "col");
	tr->Draw("BeamDetMWPCDigiY1.fWireNb:BeamDetMWPCDigiX1.fWireNb", "BeamDetMWPCDigiY1.fWireNb", "col");
	c1->Update();

	c1->cd(3);
//	tr->Draw("Length$(BeamDetMWPCDigiX1)", "Length$(BeamDetMWPCDigiX1)>0");

	c1->cd(4);
//	tr->Draw("Length$(BeamDetMWPCDigiY1)", "Length$(BeamDetMWPCDigiY1)>0");

////////////////////////////////////////////////
//	c2
////////////////////////////////////////////////

	TCanvas *c2 = new TCanvas("c2", "MWPC projections", 1500, 1000);
	c2->Divide(3,2);

	c2->cd(1);
	tr->Draw("trigger", "");
	tr->Draw("BeamDetMWPCDigiY1.fWireNb:BeamDetMWPCDigiX1.fWireNb", "trigger==2 && BeamDetMWPCDigiY1.fWireNb>0", "col");
	c2->Update();

	c2->cd(4);
	//	tr->Draw("BeamDetMWPCDigiY1.fWireNb:BeamDetMWPCDigiX1.fWireNb", "Length$(BeamDetMWPCDigiX1)>0 && Length$(BeamDetMWPCDigiY1)>0", "col");
	tr->Draw("BeamDetMWPCDigiY1.fWireNb:BeamDetMWPCDigiX1.fWireNb", "trigger==3 && BeamDetMWPCDigiY1.fWireNb>0", "col");
	c2->Update();


	c2->cd(2);
	tr->Draw("BeamDetMWPCDigiY2.fWireNb:BeamDetMWPCDigiX2.fWireNb", "trigger==2 && BeamDetMWPCDigiY2.fWireNb>0", "col");
	c2->Update();

	c2->cd(5);
	tr->Draw("BeamDetMWPCDigiY2.fWireNb:BeamDetMWPCDigiX2.fWireNb", "trigger==3 && BeamDetMWPCDigiY2.fWireNb>0", "col");
	c2->Update();

	c2->cd(3);
	tr->Draw("fYt:fXt", "trigger==2 && fYt>-40. && fXt>-40.");
	tr->SetMarkerColor(kRed);
	tr->Draw("fYt:fXt", "trigger==2 && ( (fYt)^2 + (fXt)^2 ) < 8.^2", "same");
	c2->Update();

	c2->cd(6);
	tr->SetMarkerColor(kBlack);
	tr->Draw("fYt:fXt", "trigger==3 && fYt>-40. && fXt>-40.");
	tr->SetMarkerColor(kRed);
	tr->Draw("fYt:fXt", "trigger==3 && ( (fYt-2)^2 + (fXt+3)^2 ) < 9.^2", "same");
	c2->Update();

}

