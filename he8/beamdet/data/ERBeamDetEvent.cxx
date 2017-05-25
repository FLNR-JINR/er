#include "ERBeamDetEvent.h"

#include "TString.h"

#include "FairRootManager.h"

ERBeamDetEvent::ERBeamDetEvent(){

}

ERBeamDetEvent::~ERBeamDetEvent(){

}

Bool_t ERBeamDetEvent::Register(TTree* tree, TString branchName){
	//TOF
	tree->SetBranchAddress(branchName + TString(".TDC[16]"),TDC);
	tree->SetBranchAddress(branchName + TString(".QDC[16]"),QDC);

	//MWPC
	tree->SetBranchAddress(branchName + TString(".nx1"),&nx1);
	tree->SetBranchAddress(branchName + TString(".ny1"),&ny1);
	tree->SetBranchAddress(branchName + TString(".nx2"),&nx2);
	tree->SetBranchAddress(branchName + TString(".ny2"),&ny2);
	tree->SetBranchAddress(branchName + TString(".x1[32]"),x1);
	tree->SetBranchAddress(branchName + TString(".y1[32]"),y1);
	tree->SetBranchAddress(branchName + TString(".x2[32]"),x2);
	tree->SetBranchAddress(branchName + TString(".y2[32]"),y2);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register("BeamDetEvent.", "RawEvents", this, kTRUE);

	return kTRUE;
}

Int_t ERBeamDetEvent::Process(){
	ntF3l=TDC[0];
	ntF3r=TDC[1];
	ntF4r=TDC[2];
	ntF4l=TDC[3];
	ntD11 = TDC[4];	

	naF3l=QDC[0];
	naF3r=QDC[1];
	naF4r=QDC[2];
	naF4l=QDC[3];

	return 0;
}


ClassImp(ERBeamDetEvent);