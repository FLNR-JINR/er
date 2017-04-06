/*
 * ERNXyterTreeSource.cxx
 *
 *  Created on: Apr 3, 2017
 *      Author: vratik
 */

#include "ERNXyterTreeSource.h"

#include "FairRootManager.h"

ERNXyterTreeSource::ERNXyterTreeSource() {
	// TODO Auto-generated constructor stub

}

ERNXyterTreeSource::~ERNXyterTreeSource() {
	// TODO Auto-generated destructor stub
}

Bool_t ERNXyterTreeSource::Init() {

	//input files opening
	if (fPath == "")
		Fatal("ERRootSource", "No files for source ERRootSource");
	fFile = new TFile(fPath);
	if (!fFile->IsOpen())
		Fatal("ERRootSource", "Can`t open file for source ERRootSource");

	fTree = (TTree*)fFile->Get(fTreeName);
	if (!fTree)
		Fatal("ERRootSource", "Can`t find tree in input file for source ERRootSource");

	fInEvent = new cls_RootEvent();
	fTree->SetBranchAddress(fBranchName,&fInEvent);

	FairRootManager* ioman = FairRootManager::Instance();
	ioman->Register(fBranchName, "RawEvents", fInEvent, kTRUE);

	return kTRUE;
}

Int_t ERNXyterTreeSource::ReadEvent(UInt_t) {
	if (fTree->GetEntriesFast() == fEvent+1)
		return 1;
	fEvent++;
	fTree->GetEntry(fEvent);
	return 0;
}

void ERNXyterTreeSource::Close() {
	if (fFile){
		fFile->Close();
		delete fFile;
	}
}

void ERNXyterTreeSource::Reset() {
}

void ERNXyterTreeSource::SetFile(TString path, TString treeName, TString branchName){
	fPath = path;
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " <<
		fBranchName << " added to source ERRootSource" << endl;
}
