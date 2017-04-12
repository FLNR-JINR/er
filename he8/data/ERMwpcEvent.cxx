#include "ERMwpcEvent.h"

#include "TString.h"

#include "FairRootManager.h"

ERMwpcEvent::ERMwpcEvent(){

}

ERMwpcEvent::~ERMwpcEvent(){

}

Bool_t ERMwpcEvent::Register(TTree* tree, TString branchName){

	tree->SetBranchAddress(branchName + TString(".nx1"),&nx1);
	tree->SetBranchAddress(branchName + TString(".ny1"),&ny1);
	tree->SetBranchAddress(branchName + TString(".nx2"),&nx2);
	tree->SetBranchAddress(branchName + TString(".ny2"),&ny2);
	tree->SetBranchAddress(branchName + TString(".x1[32]"),x1);
	tree->SetBranchAddress(branchName + TString(".y1[32]"),y1);
	tree->SetBranchAddress(branchName + TString(".x2[32]"),x2);
	tree->SetBranchAddress(branchName + TString(".y2[32]"),y2);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register("MwpcEvent.", "RawEvents", this, kTRUE);

	return kTRUE;
}

Int_t ERMwpcEvent::Process(){
	return 0;
}


ClassImp(ERMwpcEvent);