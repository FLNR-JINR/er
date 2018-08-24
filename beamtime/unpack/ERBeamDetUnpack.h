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
    virtual void   Reset();

  protected:
    void UnpackTofStation(DetEventDetector* detEvent,TString ampStation, TString timeStation);
    void UnpackMWPCStation(DetEventDetector* detEvent, TString ampStation,Int_t mwpcNb, Int_t planeNb);
    void AddToFDigi(Float_t edep, Double_t time, Int_t tofNb);
    void AddMWPCDigi(Float_t edep, Double_t time, 
                            Int_t mwpcNb, Int_t planeNb, Int_t wireNb);

  public:
    ClassDef(ERBeamDetUnpack, 0)
};


#endif
