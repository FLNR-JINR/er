#include "ERRTelescopeEvent.h"

#include "TString.h"

#include "FairRootManager.h"

ERRTelescopeEvent::ERRTelescopeEvent(){

}

ERRTelescopeEvent::~ERRTelescopeEvent(){

}

Bool_t ERRTelescopeEvent::Register(TTree* tree, TString branchName){

	tree->SetBranchAddress(branchName + TString(".S1[16]"),S1);
	tree->SetBranchAddress(branchName + TString(".R1[16]"),R1);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register("RTelescopeEvent.", "RawEvents", this, kTRUE);

	return kTRUE;
}


Int_t ERRTelescopeEvent::Process(){
	mD11=-1;
	mD12=-1;
	for(int i=0;i<32;i++){
		nD11[i]=0;neD11[i]=0;
		nD12[i]=0;neD12[i]=0;
	}
	for(int i=0;i<16;i++){
		if(S1[i]>0) {mD11++;nD11[mD11]=i+1;neD11[mD11]=S1[i];}
		if(R1[i]>0) {mD12++;nD12[mD12]=i+1;neD12[mD12]=R1[i];}
	}	

	return 0;
}

ClassImp(ERRTelescopeEvent);