#include "ERQTelescopeEvent.h"

#include "TString.h"

#include "FairRootManager.h"

ERQTelescopeEvent::ERQTelescopeEvent(){

}

ERQTelescopeEvent::~ERQTelescopeEvent(){

}

Bool_t ERQTelescopeEvent::Register(TTree* tree, TString branchName){
	tree->SetBranchAddress(branchName + TString(".Q1[16]"),Q1);	
	tree->SetBranchAddress(branchName + TString(".Q2[16]"),Q2);
	tree->SetBranchAddress(branchName + TString(".Q3[16]"),Q3);
	tree->SetBranchAddress(branchName + TString(".Q4[16]"),Q4);
	tree->SetBranchAddress(branchName + TString(".Q5[16]"),Q5);
	tree->SetBranchAddress(branchName + TString(".Q6[16]"),Q6);
	tree->SetBranchAddress(branchName + TString(".TQ1[16]"),TQ1);
	tree->SetBranchAddress(branchName + TString(".TQ2[16]"),TQ2);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register("QTelescopeEvent.", "RawEvents", this, kTRUE);

	return kTRUE;
}


Int_t ERQTelescopeEvent::Process(){
	//init
	mD21=-1;mD22=-1;mD23=-1;mD24=-1;mD25=-1;mD26=-1;
	mDT21=-1;mDT22=-1;

	for (int i=0;i<32;i++){
		nD21[i]=0;neD21[i]=0;
		nD22[i]=0;neD22[i]=0;
		nD23[i]=0;neD23[i]=0;
		nD24[i]=0;neD24[i]=0;
		nD25[i]=0;neD25[i]=0;
		nD26[i]=0;neD26[i]=0;
		nDT21[i]=0;ntD21[i]=0;
		nDT22[i]=0;ntD22[i]=0;
	}

	//source to raw
	for(int i=0;i<16;i++){
		if(Q1[i]>0) {mD21++;nD21[mD21]=i+1;neD21[mD21]=Q1[i];}
		if(Q2[i]>0) {mD22++;nD22[mD22]=i+1;neD22[mD22]=Q2[i];}
		if(Q3[i]>0) {mD23++;nD23[mD23]=i+1;neD23[mD23]=Q3[i];}
		if(Q4[i]>0) {mD24++;nD24[mD24]=i+1;neD24[mD24]=Q4[i];}
		if(Q5[i]>0) {mD25++;nD25[mD25]=i+1;neD25[mD25]=Q5[i];}
		if(Q6[i]>0) {mD26++;nD26[mD26]=i+1;neD26[mD26]=Q6[i];}
		if(TQ1[i]>0) {mDT21++;nDT21[mDT21]=i+1;ntD21[mDT21]=TQ1[i];}
		if(TQ2[i]>0) {mDT22++;nDT22[mDT22]=i+1;ntD22[mDT22]=TQ2[i];}
	}

	return 0;
}




ClassImp(ERQTelescopeEvent);