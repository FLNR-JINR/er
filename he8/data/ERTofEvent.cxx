#include "ERTofEvent.h"

#include "TString.h"

#include "FairRootManager.h"

ERTofEvent::ERTofEvent(){

}

ERTofEvent::~ERTofEvent(){

}

Bool_t ERTofEvent::Register(TTree* tree, TString branchName){
	tree->SetBranchAddress(branchName + TString(".TDC[16]"),TDC);
	tree->SetBranchAddress(branchName + TString(".QDC[16]"),QDC);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register("TofEvent.", "RawEvents", this, kTRUE);

	return kTRUE;
}

Int_t ERTofEvent::Process(){
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


ClassImp(ERTofEvent);