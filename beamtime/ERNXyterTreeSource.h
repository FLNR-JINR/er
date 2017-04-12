/*
 * ERNXyterTreeSource.h
 *
 *  Created on: Apr 3, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERNXYTERTREESOURCE_H_
#define BEAMTIME_ERNXYTERTREESOURCE_H_

#include "FairSource.h"

#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"

#include "cls_RootEvent.h"
#include "WCalEvent.h"

#include <fstream>

using std::cout;
using std::endl;

class ERNXyterTreeSource: public FairSource {
public:
	ERNXyterTreeSource();
	virtual ~ERNXyterTreeSource();
	ClassDef(ERNXyterTreeSource,1)

	/** Virtual method Init **/
	virtual Bool_t Init();

	virtual Int_t ReadEvent(UInt_t=0);

	virtual void Close();

	virtual void Reset();

	virtual Source_Type GetSourceType(){return kFILE;}
	virtual void SetParUnpackers(){}
	virtual Bool_t InitUnpackers(){return kTRUE;}
	virtual Bool_t ReInitUnpackers(){return kTRUE;}

	void SetInFile(TString path, TString treeName, TString branchName);

	void SetCalParameters(const char* p_filename);
	void SetNonLinGraphs(const char* graph_filename);
	void PrintCalParameters();

private:
	void ImportCalParameters();
	void ImportNonLinGraphs();


private:

	TString fPath;
	TString fTreeName;
	TString fBranchName;

	TString fCalParFileName;
	TString fNonLinGraphsFileName;

	TFile* fFile;
	TTree* fTree;

	cls_RootEvent* fInEvent;
	WCalEvent *fOutEvent;

	Long64_t fEvent;

	Float_t fCalPar[64];
	const Float_t f1ePosCorrection = 1.18;
	Float_t fPedestalsCorrection[64];

	TRandom3 calRandom;

};

#endif /* BEAMTIME_ERNXYTERTREESOURCE_H_ */
