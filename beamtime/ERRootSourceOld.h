#ifndef ERRootSourceOld_H
#define ERRootSourceOld_H

#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "ERRawEvent.h"

class ERRootSourceOld : public FairSource
{
  public:
    ERRootSourceOld();
    ERRootSourceOld(const ERRootSourceOld& source);
    virtual ~ERRootSourceOld();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void SetFile(TString path, TString treeName, TString branchName);

    void AddEvent(ERRawEvent* event) {fRawEvents.push_back(event);}
  private:
    TString fPath;
    TString fTreeName;
    TString fBranchName;
    TFile* fFile;
    TTree* fTree;
    Int_t HE8Event_nevent;

    std::vector<ERRawEvent*> fRawEvents;
  public:
    ClassDef(ERRootSourceOld, 1)
};


#endif