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

#include "cls_RootEvent.h"

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

	void SetFile(TString path, TString treeName, TString branchName);

private:

	TString fPath;
	TString fTreeName;
	TString fBranchName;

	TFile* fFile;
	TTree* fTree;

	cls_RootEvent* fInEvent;

	Int_t fEvent;
};

#endif /* BEAMTIME_ERNXYTERTREESOURCE_H_ */
