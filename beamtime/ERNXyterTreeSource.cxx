/*
 * ERNXyterTreeSource.cxx
 *
 *  Created on: Apr 3, 2017
 *      Author: vratik
 */

#include "ERNXyterTreeSource.h"

#include "FairRootManager.h"
#include "cls_RootHit.h"

#include "ERNXyterCalibrator.h"

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
	fCalParFileName(""),
	fNonLinGraphsFileName(""),
	fEvent(0)
{
	for (Int_t i = 0; i < 64; i++) {
		fCalPar[i] = 1;
	}

	fPedestalsCorrection[0] = 25;
	fPedestalsCorrection[1] = 19;
	fPedestalsCorrection[2] = 23;
	fPedestalsCorrection[3] = 21;
	fPedestalsCorrection[4] = 21;
	fPedestalsCorrection[5] = 19;
	fPedestalsCorrection[6] = 27;
	fPedestalsCorrection[7] =	25;
	fPedestalsCorrection[8] =	23;
	fPedestalsCorrection[9] =	-3;
	fPedestalsCorrection[10] =	27;
	fPedestalsCorrection[11] =	23;
	fPedestalsCorrection[12] =	1;
	fPedestalsCorrection[13] =	1;
	fPedestalsCorrection[14] =	3;
	fPedestalsCorrection[15] =	23;
	fPedestalsCorrection[16] =	27;
	fPedestalsCorrection[17] =	19;
	fPedestalsCorrection[18] =	25;
	fPedestalsCorrection[19] =	17;
	fPedestalsCorrection[20] =	19;
	fPedestalsCorrection[21] =	29;
	fPedestalsCorrection[22] =	15;
	fPedestalsCorrection[23] =	21;
	fPedestalsCorrection[24] =	19;
	fPedestalsCorrection[25] =	25;
	fPedestalsCorrection[26] =	1;
	fPedestalsCorrection[27] =	21;
	fPedestalsCorrection[28] =	21;
	fPedestalsCorrection[29] =	19;
	fPedestalsCorrection[30] =	17;
	fPedestalsCorrection[31] =	3;
	fPedestalsCorrection[32] =	17;
	fPedestalsCorrection[33] =	21;
	fPedestalsCorrection[34] =	21;
	fPedestalsCorrection[35] =	25;
	fPedestalsCorrection[36] =	21;
	fPedestalsCorrection[37] =	23;
	fPedestalsCorrection[38] =	15;
	fPedestalsCorrection[39] =	23;
	fPedestalsCorrection[40] =	15;
	fPedestalsCorrection[41] =	19;
	fPedestalsCorrection[42] =	-1;
	fPedestalsCorrection[43] =	15;
	fPedestalsCorrection[44] =	15;
	fPedestalsCorrection[45] =	-3;
	fPedestalsCorrection[46] =	19;
	fPedestalsCorrection[47] =	11;
	fPedestalsCorrection[48] =	23;
	fPedestalsCorrection[49] =	19;
	fPedestalsCorrection[50] =	19;
	fPedestalsCorrection[51] =	15;
	fPedestalsCorrection[52] =	19;
	fPedestalsCorrection[53] =	3;
	fPedestalsCorrection[54] =	21;
	fPedestalsCorrection[55] =	25;
	fPedestalsCorrection[56] =	27;
	fPedestalsCorrection[57] =	17;
	fPedestalsCorrection[58] =	19;
	fPedestalsCorrection[59] =	17;
	fPedestalsCorrection[60] =	-3;
	fPedestalsCorrection[61] =	17;
	fPedestalsCorrection[62] =	13;
	fPedestalsCorrection[63] =	21;

}

ERNXyterTreeSource::~ERNXyterTreeSource() {
	// TODO Auto-generated destructor stub
}

Bool_t ERNXyterTreeSource::Init() {

	//input files opening
	if (fPath == "")
		Fatal("ERNXyterTreeSource::Init", "No files for source ERNXyterTreeSource");
	fFile = new TFile(fPath);
	if (!fFile->IsOpen()) {
		Fatal("ERNXyterTreeSource::Init", "Can`t open file for source ERNXyterTreeSource");
		return kFALSE;
	}

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

	ImportCalParameters();
	ImportNonLinGraphs();

	return kTRUE;
}

Int_t ERNXyterTreeSource::ReadEvent(UInt_t) {

//	cout << "begin of function ReadEvent" << endl;

	if ( !(fEvent%100000) ) {
		std::cout << "####### EVENT " << fEvent << " #####" << std::endl;
	}

	if (fTree->GetEntriesFast() == fEvent+1) {
		std::cout << "####### EVENT " << fEvent << " #####" << std::endl;
		return 1;
	}

	fTree->GetEntry(fEvent);

	
	TClonesArray *rawHits = fInEvent->GetHits();
	cls_RootHit *rawHit;
	UChar_t channel;
	Int_t adcVal;
	Float_t adcNonLin, adcNonLinCorr, adcCalibrated;

	//fOutEvent->SetNumOfHits(fInEvent->GetNumOfHits());

//	cout << "number of hits: " << rawHits->GetLast() << "\t" << fInEvent->GetNumOfHits() << endl;
	for (Int_t i = 0; i <= rawHits->GetLast(); i++) {
		rawHit = (cls_RootHit*)rawHits->ConstructedAt(i);
//		cout << (Int_t)rawHit->GetChannel() << "\t" << rawHit->GetAdcVal()
//				<< "\t" << ERNXyterCalibrator::Instance().GetCalibratedVal(channel, adcVal) << endl;
		channel = rawHit->GetChannel();
		adcVal = rawHit->GetAdcVal();
		adcNonLin = ERNXyterCalibrator::Instance().GetCalibratedVal(channel, adcVal);
		adcNonLinCorr = adcNonLin - fPedestalsCorrection[channel];
//		adcCalibrated = (adcNonLinCorr+calRandom.Uniform(-0.5, 0.5))/fEffCalib[ch];
		adcCalibrated = (adcNonLinCorr+calRandom.Uniform(-0.5, 0.5))/fCalPar[channel];

//		cout << (Int_t)channel << "\t" << adcVal << "\t" << adcNonLin
//				<< "\t" << adcNonLinCorr << "\t" << adcCalibrated << endl;

//		cout << adcVal << "\t" << ERNXyterCalibrator::Instance().GetCalibratedVal(channel, adcVal) << endl;

//		fOutEvent->AddHit(channel, adcVal);
		fOutEvent->AddHit(channel, adcVal, adcNonLin, adcNonLinCorr, adcCalibrated);
	}
//	cout << "======== end of hit =========" << endl;

	fEvent++;

//	cout << "end of function ReadEvent" << endl;
	
	return 0;
}

void ERNXyterTreeSource::Close() {
	if (fFile){
		fFile->Close();
		delete fFile;
	}
}

void ERNXyterTreeSource::Reset() {
//	cout << "begin of function Reset" << endl;

	fOutEvent->Clear();

//	cout << "end of function Reset" << endl;
}

void ERNXyterTreeSource::SetInFile(TString path, TString treeName, TString branchName){
	fPath = path;
	fTreeName = treeName;
	fBranchName = branchName;
	cout << "Input file " << path << " with tree name " << fTreeName <<" and branch name " <<
		fBranchName << " added to source ERNXyterTreeSource" << endl;
}

void ERNXyterTreeSource::SetCalParameters(const char* p_filename)
{
	fCalParFileName = p_filename;
}

void ERNXyterTreeSource::SetNonLinGraphs(const char* graph_filename) {
	fNonLinGraphsFileName = graph_filename;
}

void ERNXyterTreeSource::ImportCalParameters() {

	if (fCalParFileName.Length()==0) {
		Warning("ERNXyterTreeSource::ImportCalParameters",
				"File with 1e calibration parameters was not set.");
		return;
	}

	Info("ERNXyterTreeSource::ImportCalParameters",
			"Importing calibration parameters from file %s corrected for %f.",
			fCalParFileName.Data(), f1ePosCorrection);

	std::ifstream infile(fCalParFileName.Data());
	if (!infile.is_open()) {
		Error("ERNXyterTreeSource::ImportCalParameters", "File %s was not open.", fCalParFileName.Data());
		return;
	}

	Int_t ch;
	Float_t val;

	for (unsigned int i=0; i<64; i++) {
		infile >> ch >> val;
		fCalPar[ch] = val/f1ePosCorrection;
	}

	infile.close();
}

void ERNXyterTreeSource::ImportNonLinGraphs() {
//	TString graphFile = graph_filename;
	if (fNonLinGraphsFileName.Length()==0) {
		Warning("ERNXyterTreeSource::ImportNonLinGraphs", "File with graphs for non-linearity correction was not set.");
		return;
	}

	Info("ERNXyterTreeSource::ImportNonLinGraphs", "Importing graphs for non-linearity correction from file %s.", fNonLinGraphsFileName.Data());

	ERNXyterCalibrator::Instance().ImportGraphs(fNonLinGraphsFileName);
}

void ERNXyterTreeSource::PrintCalParameters() {
	cout << "channel\tparameter" << endl;
	for (Int_t i = 0; i<64; i++) {
		cout << i << "\t" << fCalPar[i] << endl;
	}
}
