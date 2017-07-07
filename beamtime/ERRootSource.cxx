
#include "ERRootSource.h"

#include "FairRootManager.h"
#include "FairRun.h"

#include "ERHe8EventHeader.h"

#include <iostream>
using namespace std;

ERRootSource::ERRootSource():
fFile(NULL),
fTree(NULL),
fPath(""),
fTreeName(""),
fBranchName("")
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
	
	if (fRawEvents.size() == 0)
		Fatal("ERRootSource", "ERRootSource without regiistered events");

	FairRun* run = FairRun::Instance();
	ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
	header->Register(fTree, fBranchName);

	for (Int_t iREvent = 0; iREvent < fRawEvents.size(); iREvent++)
		fRawEvents[iREvent]->Register(fTree, fBranchName);
	return kTRUE;
}

Int_t ERRootSource::ReadEvent(UInt_t id){
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