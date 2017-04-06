
#include "ERRootSource.h"

#include "FairRootManager.h"

#include <iostream>
using namespace std;

ERRootSource::ERRootSource():
fInEvent(NULL),
fFile(NULL),
fTree(NULL),
fPath(""),
fTreeName(""),
fBranchName(""),
fEvent(0)
{
}

ERRootSource::ERRootSource(const ERRootSource& source){
}

ERRootSource::~ERRootSource(){

}

Bool_t ERRootSource::Init(){
	//input files opening
	if (fPath == "")
		Fatal("ERRootSource", "No files for source ERRootSource");
	fFile = new TFile(fPath);
	if (!fFile->IsOpen())
		Fatal("ERRootSource", "Can`t open file for source ERRootSource");

	fTree = (TTree*)fFile->Get(fTreeName);
	if (!fTree)
		Fatal("ERRootSource", "Can`t find tree in input file for source ERRootSource");
	
	fInEvent = new TLiEvent();
	fTree->SetBranchAddress(fBranchName,&fInEvent);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register(fBranchName, "RawEvents", fInEvent, kTRUE);
	
	return kTRUE;
}

Int_t ERRootSource::ReadEvent(UInt_t id){
	//Проверяем есть ли еще события для обработки
	if (fTree->GetEntriesFast() == fEvent+1)
		return 1;
	fEvent++;
	fTree->GetEntry(fEvent);
	return 0;
}

void ERRootSource::Close(){
	if (fFile){
		fFile->Close();
		delete fFile;
	}
}

void ERRootSource::Reset(){
}

void ERRootSource::SetFile(TString path, TString treeName, TString branchName){
	fPath = path;
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " << 
		fBranchName << " added to source ERRootSource" << endl;
}