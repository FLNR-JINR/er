#ifndef ERBeamDetUnpack_H
#define ERBeamDetUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERBeamDetUnpack : public ERUnpack
{
  public:
    ERBeamDetUnpack(TString detName);
    virtual ~ERBeamDetUnpack();

    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);

  protected:
    void AddToFDigi(Float_t edep, Double_t time, Int_t tofNb);
    void AddMWPCDigi(Float_t edep, Double_t time, 
                            TString mwpcSt, Int_t wireNb);
    std::map<TString,Int_t> fMwpcAmpTimeStations;
    std::map<TString,TString> fMwpcBnames;

  public:
    ClassDef(ERBeamDetUnpack, 0)
};


#endif
