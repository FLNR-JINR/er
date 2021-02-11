#ifndef ERNDUnpack_H
#define ERNDUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERNDUnpack : public ERUnpack {
 public:
  ERNDUnpack(TString detName, TString ampStation, TString timeStation, TString tacStation,
              TString ampCalFile, TString timeCalFile, TString tacCalFile,
              ChannelsMapping* channelsMapping = nullptr,
              Bool_t skipAloneChannels = kTRUE);
  virtual ~ERNDUnpack();
  virtual Bool_t Init(SetupConfiguration* setupConf);
  virtual Bool_t DoUnpack(Int_t* data, Int_t size);
 protected:
  void AddNDDigi(float edep, float time, float tac, ERChannel channelNb);
  void ApplyCalibrations(const ERChannel channel, float& amp, float& time, float& tac);
  Bool_t CheckSetup();
 protected:
  TString fAmpStation;
  TString fTimeStation;
  TString fTACStation;
  TMatrixD* fAmpCalTable = nullptr;
  TMatrixD* fTimeCalTable = nullptr;
  TMatrixD* fTACCalTable = nullptr;
  ChannelsMapping* fChannelsMapping = nullptr;
  Bool_t fSkipAloneChannels = false;
 public:
  ClassDef(ERNDUnpack, 0)
};

#endif
