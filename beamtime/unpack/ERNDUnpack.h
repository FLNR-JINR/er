#ifndef ERNDUnpack_H
#define ERNDUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERNDUnpack : public ERUnpack {
 public:
  ERNDUnpack(TString detName, TString ampStation, TString timeStation, TString tacStation,
              TString ampCalFile, TString timeCalFile, TString tacCalFile,
              ChannelMapping* channelsMapping = nullptr,
              Bool_t skipAloneChannels = kTRUE);
  virtual ~ERNDUnpack() = default;
 protected:
  virtual void Register();
  virtual std::vector<TString> InputBranchNames() const;
  virtual void UnpackSignalFromStations();
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
  ChannelMapping* fChannelMapping = nullptr;
  Bool_t fSkipAloneChannels = false;
 public:
  ClassDef(ERNDUnpack, 0)
};

#endif
