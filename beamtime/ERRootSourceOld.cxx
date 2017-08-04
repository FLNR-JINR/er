#include "ERRootSourceOld.h"

#include "FairRootManager.h"
#include "FairRun.h"

#include "ERHe8EventHeader.h"

#include <iostream>
using namespace std;

ERRootSourceOld::ERRootSourceOld():
fFile(NULL),
fTree(NULL),
fPath(""),
fTreeName(""),
fBranchName("")
{
}

ERRootSourceOld::ERRootSourceOld(const ERRootSourceOld& source){
}

ERRootSourceOld::~ERRootSourceOld(){

}

Bool_t ERRootSourceOld::Init(){
	//input files opening
	if (fPath == "")
		Fatal("ERRootSourceOld", "No files for source ERRootSourceOld");
	fFile = new TFile(fPath);
	if (!fFile->IsOpen())
		Fatal("ERRootSourceOld", "Can`t open file for source ERRootSourceOld");

	fTree = (TTree*)fFile->Get(fTreeName);
	if (!fTree)
		Fatal("ERRootSourceOld", "Can`t find tree in input file for source ERRootSourceOld");
	
	if (fRawEvents.size() == 0)
		Fatal("ERRootSourceOld", "ERRootSourceOld without regiistered events");

	FairRun* run = FairRun::Instance();
	ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
	header->Register(fTree, fBranchName);

	for (Int_t iREvent = 0; iREvent < fRawEvents.size(); iREvent++)
		fRawEvents[iREvent]->Register(fTree, fBranchName);
	return kTRUE;
}

Int_t ERRootSourceOld::ReadEvent(UInt_t id){
	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");
	//Проверяем есть ли еще события для обработки
	if (fTree->GetEntriesFast() == ioman->GetEntryNr()+1)
		return 1;
	//cout << "ev" << ioman->GetEntryNr() << endl;
	fTree->GetEntry(ioman->GetEntryNr());

	for (Int_t iREvent = 0; iREvent < fRawEvents.size(); iREvent++)
		fRawEvents[iREvent]->Process();
	return 0;
}

void ERRootSourceOld::Close(){
	if (fFile){
		fFile->Close();
		delete fFile;
	}
}

void ERRootSourceOld::Reset(){
}

void ERRootSourceOld::SetFile(TString path, TString treeName, TString branchName){
	fPath = path;
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " << 
		fBranchName << " added to source ERRootSourceOld" << endl;
}