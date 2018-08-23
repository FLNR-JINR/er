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
    ERUnpack(TString name);
    virtual ~ERUnpack();

    virtual Bool_t Init(){return kTRUE;}
    virtual Bool_t Init(SetupConfiguration* setupConf) { fSetupConfiguration = setupConf; return kTRUE;}
    virtual Bool_t DoUnpack(Int_t* data, Int_t size) = 0;
    virtual void   Reset() = 0;
    const char* GetName(){return fName.Data();}
  protected:
    virtual void Register(){};

    SetupConfiguration* fSetupConfiguration;
    Bool_t fInited;
    Bool_t fUnpacked;
    TString fName;

    std::map<TString,TClonesArray*> fDigiCollections;

  public:
    ClassDef(ERUnpack, 0)
};


#endif
