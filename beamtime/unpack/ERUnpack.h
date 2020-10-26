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

#include "ERSupport.h"

using Channel2Value = std::map<Int_t, float>;
using Channel2AmplitudeTime = std::map<ERChannel, std::pair<float, float>>;
using Channel2AmplitudeTimeTac = std::map<ERChannel, std::tuple<float, float, float>>;

class ERUnpack : public FairUnpack {
 public:
  ERUnpack(TString detName);
  virtual ~ERUnpack();
  virtual Bool_t Init() {return kTRUE;}
  virtual Bool_t Init(SetupConfiguration* setupConf);
  virtual Bool_t DoUnpack(Int_t* data, Int_t size);
  virtual void   Reset();
  TString GetDetName() const {return fDetName;}
  Bool_t IsInited() const {return fInited;}
 protected:
  virtual void Register() {};
  void UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                            Channel2AmplitudeTime& valueMap, Bool_t skipAloneChannels = kTRUE);
  void UnpackAmpTimeTACStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                TString tacStation, Channel2AmplitudeTimeTac& valueMap, Bool_t skipAloneChannels = kTRUE);
  void UnpackStation(DetEventDetector* detEvent, TString station, Channel2Value& valueMap);
 protected:
  SetupConfiguration* fSetupConfiguration = nullptr;
  Bool_t fInited;
  Bool_t fUnpacked;
  TString fDetName;
  std::map<TString,TClonesArray*> fDigiCollections;
 public:
  ClassDef(ERUnpack, 0)
};

#endif
