#ifndef ERTelescopeUnpack_H
#define ERTelescopeUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERTelescopeUnpack : public ERUnpack
{
  public:
    ERTelescopeUnpack(TString detName);
    virtual ~ERTelescopeUnpack();

    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);
    virtual void   Reset();

  protected:
    void UnpackSiStation(DetEventDetector* detEvent, TString ampStation, TString timeStation);
    void AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, Int_t stripNb, TString digiBranchName);

  public:
    ClassDef(ERTelescopeUnpack, 0)
};


#endif
