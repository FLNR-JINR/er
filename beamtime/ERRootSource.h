#ifndef ERRootSource_H
#define ERRootSource_H

#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "ERRawEvent.h"

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
    void AddFile(TString path){fPath.push_back(path);}

    void AddEvent(ERRawEvent* event) {fRawEvents.push_back(event);}
  private:
    std::vector<TString> fPath;
    Int_t fCurFile;
    Int_t fOldEvents;
    TString fTreeName;
    TString fBranchName;
    TFile* fFile;
    TTree* fTree;
    Int_t HE8Event_nevent;

    std::vector<ERRawEvent*> fRawEvents;

    Int_t OpenNextFile();
  public:
    ClassDef(ERRootSource, 1)
};


#endif
