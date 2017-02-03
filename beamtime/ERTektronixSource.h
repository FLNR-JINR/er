#ifndef ERTektronixSource_H
#define ERTektronixSource_H

#include <stdio.h>
#include <map>
#include <fstream>

using namespace std;

#include "TString.h"

#include "FairSource.h"

#include "RawEvent.h"

class ERTektronixSource : public FairSource
{
  public:
    ERTektronixSource();
    ERTektronixSource(const ERTektronixSource& source);
    virtual ~ERTektronixSource();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void AddFile(Int_t chanel, TString path);
    void SetNPoints(Int_t pointsNb){fNPoints = pointsNb;}
  private:
    map<Int_t, vector<TString> > fPaths;
    map<Int_t, vector<ifstream*> > fFiles;
    Int_t fNPoints;
    Int_t fNProcessedFiles;
    Int_t fNForProcFiles;
    map<Int_t, Int_t> fCurrentFiles;

    //Output data
    RawEvent* fCh1;
    RawEvent* fCh2;
    RawEvent* fCh3;
    RawEvent* fCh4;
  public:
    ClassDef(ERTektronixSource, 1)
};


#endif
