#include <iostream>
using namespace std;

#include "ERHe8EventHeader.h"

ERHe8EventHeader::ERHe8EventHeader(){

}

Bool_t ERHe8EventHeader::Register(TTree* tree, TString branchName){
	tree->SetMakeClass(1);
	tree->SetBranchAddress(branchName + TString(".nevent"),&this->HE8Event_nevent);
	tree->SetBranchAddress(branchName + TString(".trigger"),&HE8Event_trigger);
	tree->SetBranchAddress(branchName + TString(".subevents"),&HE8Event_subevents);
	tree->SetBranchAddress(branchName + TString(".evsize"),&HE8Event_evsize);
	tree->SetBranchAddress(branchName + TString(".time"),&HE8Event_time);

	return kTRUE;
}

ClassImp(ERHe8EventHeader)