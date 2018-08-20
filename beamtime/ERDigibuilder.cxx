
#include "ERDigibuilder.h"

#include "FairRootManager.h"
#include "FairRun.h"

#include <iostream>
using namespace std;

ERDigibuilder::ERDigibuilder():
fFile(NULL),
fTree(NULL),
fTreeName(""),
fBranchName(""),
fCurFile(0),
fOldEvents(0)
{
}

ERDigibuilder::ERDigibuilder(const ERDigibuilder& source){
}

ERDigibuilder::~ERDigibuilder(){

}

Bool_t ERDigibuilder::Init(){
	//input files opening
	if (fPath.size() == 0)
		Fatal("ERDigibuilder", "No files for source ERDigibuilder");

	FairRun* run = FairRun::Instance();
	return kTRUE;
}

Int_t ERDigibuilder::ReadEvent(UInt_t id){
	/*
	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");

	//Проверяем есть ли еще события для обработки
	if (fTree->GetEntriesFast() == ioman->GetEntryNr()-fOldEvents){
		fOldEvents += ioman->GetEntryNr();
		if (OpenNextFile())
			return 1;
	}
	//cout << "ev" << ioman->GetEntryNr() << endl;
	fTree->GetEntry(ioman->GetEntryNr()-fOldEvents);

	for (Int_t iREvent = 0; iREvent < fRawEvents.size(); iREvent++)
		fRawEvents[iREvent]->Process();
	*/
	return 0;
}

void ERDigibuilder::Close(){
	/*
	if (fFile){
		fFile->Close();
		delete fFile;
	}
	*/
}

void ERDigibuilder::Reset(){
}

void ERDigibuilder::SetFile(TString path, TString treeName, TString branchName){
	fPath.push_back(path);
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " << 
		fBranchName << " added to source ERDigibuilder" << endl;
}