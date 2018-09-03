#ifndef ERUnpack_H
#define ERUnpack_H

#include <map>
#include <vector>

#include "TString.h"
#include "TClonesArray.h"

#include "FairUnpack.h"

#include "SetupConfiguration.h"
#include "DetEventDetector.h"

class ERUnpack : public FairUnpack
{
  public:
    ERUnpack(TString detName);
    virtual ~ERUnpack();

    virtual Bool_t Init(){return kTRUE;}
    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);
    virtual void   Reset();
    TString GetDetName(){return fDetName;}
    Bool_t IsInited(){return fInited;}
  protected:
    virtual void Register(){};

    void UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                 std::vector<Double_t>& ampV, std::vector<Double_t>& timeV, std::vector<Int_t>& channelV);
    void UnpackStation(DetEventDetector* detEvent, TString station, std::map<Int_t,Double_t>& valueV);

    SetupConfiguration* fSetupConfiguration;
    Bool_t fInited;
    Bool_t fUnpacked;
    TString fDetName;

    std::map<TString,TClonesArray*> fDigiCollections;

  public:
    ClassDef(ERUnpack, 0)
};


#endif
