#ifndef ERUnpack_H
#define ERUnpack_H

#include <map>

#include "TString.h"
#include "TClonesArray.h"

#include "FairUnpack.h"

#include "SetupConfiguration.h"

class ERUnpack : public FairUnpack
{
  public:
    ERUnpack(TString detName);
    virtual ~ERUnpack();

    virtual Bool_t Init(){return kTRUE;}
    virtual Bool_t Init(SetupConfiguration* setupConf) { fSetupConfiguration = setupConf; return kTRUE;}
    virtual Bool_t DoUnpack(Int_t* data, Int_t size) = 0;
    virtual void   Reset();
    TString GetDetName(){return fDetName;}
    Bool_t IsInited(){return fInited;}
  protected:
    virtual void Register(){};

    SetupConfiguration* fSetupConfiguration;
    Bool_t fInited;
    Bool_t fUnpacked;
    TString fDetName;

    std::map<TString,TClonesArray*> fDigiCollections;

  public:
    ClassDef(ERUnpack, 0)
};


#endif
