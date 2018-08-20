#ifndef ERDigibuilder_H
#define ERDigibuilder_H

#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "ERRawEvent.h"

class ERDigibuilder : public FairSource
{
  public:
    ERDigibuilder();
    ERDigibuilder(const ERDigibuilder& source);
    virtual ~ERDigibuilder();

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
  private:
    std::vector<TString> fPath;
    Int_t fCurFile;
    Int_t fOldEvents;
    TString fTreeName;
    TString fBranchName;
    TFile* fFile;
    TTree* fTree;
  public:
    ClassDef(ERDigibuilder, 1)
};


#endif
