/*
 * ERNXyterTreeSource.cxx
 *
 *  Created on: Apr 3, 2017
 *      Author: vratik
 */

#include "ERNXyterTreeSource.h"

#include "FairRootManager.h"
#include "cls_RootHit.h"

#include <iostream>
using namespace std;

ERNXyterTreeSource::ERNXyterTreeSource() :
	fInEvent(NULL),
	fOutEvent(NULL),
	fFile(NULL),
	fTree(NULL),
	fPath(""),
	fTreeName(""),
	fBranchName(""),
	fEvent(0)
{
	for (Int_t i = 0; i < 64; i++) {
		fCalPar[i] = 0;
	}

}

ERNXyterTreeSource::~ERNXyterTreeSource() {
	// TODO Auto-generated destructor stub
}

Bool_t ERNXyterTreeSource::Init() {

	//input files opening
	if (fPath == "")
		Fatal("ERNXyterTreeSource::Init", "No files for source ERNXyterTreeSource");
	fFile = new TFile(fPath);
	if (!fFile->IsOpen())
		Fatal("ERNXyterTreeSource::Init", "Can`t open file for source ERNXyterTreeSource");

	fTree = (TTree*)fFile->Get(fTreeName);
	if (!fTree)
		Fatal("ERNXyterTreeSource::Init", "Can`t find tree in input file for source ERNXyterTreeSource");

	fInEvent = new cls_RootEvent();
	fTree->SetBranchAddress(fBranchName.Data(),&fInEvent);
//	cout << fTree->GetEntries() << " events" << endl;

	fOutEvent = new WCalEvent();

	FairRootManager* ioman = FairRootManager::Instance();
//	cout << "branch name: " << fBranchName.Data() << endl;
	//todo: name of branch is not saved in root file!!!!
	ioman->Register(fBranchName.Data(), "RawEvents", fInEvent, kTRUE);
	ioman->Register("outBranch.", "RawEvents", fOutEvent, kTRUE);

	return kTRUE;
}

Int_t ERNXyterTreeSource::ReadEvent(UInt_t) {

	cout << "begin of function ReadEvent" << endl;

	if (fTree->GetEntriesFast() == fEvent+1)
		return 1;

	fTree->GetEntry(fEvent);

	
	TClonesArray *rawHits = fInEvent->GetHits();
	cls_RootHit *rawHit;
	Int_t adcVal, channel;

	//fOutEvent->SetNumOfHits(fInEvent->GetNumOfHits());

	cout << "number of hits: " << rawHits->GetLast() << "\t" << fInEvent->GetNumOfHits() << endl;
	for (Int_t i = 0; i <= rawHits->GetLast(); i++) {
		rawHit = (cls_RootHit*)rawHits->ConstructedAt(i);
		cout << (Int_t)rawHit->GetChannel() << "\t" << rawHit->GetAdcVal() << endl;
		channel = rawHit->GetChannel();
		adcVal = rawHit->GetAdcVal();
		fOutEvent->AddHit(channel, adcVal);
	}
//	cout << "======== end of hit =========" << endl;

	fEvent++;

	cout << "end of function ReadEvent" << endl;
	
	return 0;
}

void ERNXyterTreeSource::Close() {
	if (fFile){
		fFile->Close();
		delete fFile;
	}
}

void ERNXyterTreeSource::Reset() {
	cout << "begin of function Reset" << endl;

	fOutEvent->Clear();

	cout << "end of function Reset" << endl;
}

void ERNXyterTreeSource::SetInFile(TString path, TString treeName, TString branchName){
	fPath = path;
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " <<
		fBranchName << " added to source ERNXyterTreeSource" << endl;
}

void ERNXyterTreeSource::ImportCalParameters(const char* p_filename)
{
	TString fname = p_filename;

	if (fname.Length()==0) {
		Warning("ERNXyterTreeSource::ImportCalParameters", "File with 1e calibration parameters was not set.");
		return;
	}

	Info("ERNXyterTreeSource::ImportCalParameters", "Importing calibration parameters from file %s.", fname.Data());

	std::ifstream infile(fname.Data());
	if (!infile.is_open()) {
		Error("ERNXyterTreeSource::ImportCalParameters", "File %s was not open.", fname.Data());
		return;
	}

	Int_t ch;
	Float_t val;

	for (unsigned int i=0; i<64; i++) {
		infile >> ch >> val;
		fCalPar[ch] = val/f1ePosCorrection;
	}
}
