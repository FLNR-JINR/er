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

    /* accessors */

    void SetToFCalibration(Double_t a1, Double_t b1, Double_t a2, Double_t b2)
        { fCalToFa1 = a1; fCalToFb1 = b1; fCalToFa2 = a2; fCalToFb2 = b2; }
    void SetMWPCCalibration(Double_t a, Double_t b)
        { fCalMWPCa = a; fCalMWPCb = b;}
    void SetMWPCMapping(std::map<Int_t, Int_t>* channelsMapping) {
      fMwpcChannelsMapping = channelsMapping;
    }

  protected:
    void AddToFDigi(Float_t edep, Double_t time, Int_t tofNb);
    void AddMWPCDigi(Float_t edep, Double_t time, 
                            TString mwpcSt, Int_t wireNb);
    std::map<TString,Int_t> fMwpcAmpTimeStations;
    std::map<TString,TString> fMwpcBnames;
    std::map<Int_t, Int_t>* fMwpcChannelsMapping = nullptr;

    Double_t fCalToFa1, fCalToFb1, fCalToFa2, fCalToFb2;
    Double_t fCalMWPCa, fCalMWPCb;

  public:
    ClassDef(ERBeamDetUnpack, 0)
};


#endif
