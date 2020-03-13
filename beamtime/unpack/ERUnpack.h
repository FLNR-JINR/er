#ifndef ERUnpack_H
#define ERUnpack_H

#include <map>
#include <vector>
#include <tuple>

#include "TString.h"
#include "TClonesArray.h"
#include "TMatrixD.h"

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
                                 std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap,
                                 Bool_t skipAloneChannels = kTRUE);
    void UnpackAmpTimeTACStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                 TString tacStation,
                                 std::map<Int_t, std::tuple<Double_t, Double_t, Double_t> >& valueMap,
                                 Bool_t skipAloneChannels = kTRUE);
    void UnpackStation(DetEventDetector* detEvent, TString station, std::map<Int_t,Double_t>& valueMap);

    SetupConfiguration* fSetupConfiguration;
    Bool_t fInited;
    Bool_t fUnpacked;
    TString fDetName;

    std::map<TString,TClonesArray*> fDigiCollections;

  public:
    ClassDef(ERUnpack, 0)
};


#endif
