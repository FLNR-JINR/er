#ifndef ERRootSource_H
#define ERRootSource_H

#include <stdio.h>
#include <map>
#include <fstream>

using namespace std;

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "TLiEvent.h"

class ERRootSource : public FairSource
{
  public:
    ERRootSource();
    ERRootSource(const ERRootSource& source);
    virtual ~ERRootSource();

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
    //Iput data
    TLiEvent* fInEvent;
    Int_t fEvent;
  public:
    ClassDef(ERRootSource, 1)
};


#endif
