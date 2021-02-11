#ifndef ERTelescopeUnpack_H
#define ERTelescopeUnpack_H

#include <map>
#include <vector>

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERTelescopeStation {
 public:
  ERTelescopeStation(TString _type, Int_t _sideCount, TString _ampStName, TString _timeStName,
                      TString _ampStName2, TString _timeStName2, TString _ampCalFile, TString _timeCalFile,
                      TString _ampCalFile2, TString _timeCalFile2, TString _XY, TString _XYside,
                      ChannelsMapping* _channelsMapping1, ChannelsMapping* _channelsMapping2,
                      Bool_t _skipAloneChannels);
  TString type; //Si, CsI
  Int_t sideCount; //1,2
  TString ampStName;
  TString timeStName;
  TString ampStName2;
  TString timeStName2;
  TString ampCalFile;
  TString timeCalFile;
  TString ampCalFile2;
  TString timeCalFile2;
  TMatrixD* ampCalTable = nullptr;
  TMatrixD* timeCalTable = nullptr;
  TMatrixD* ampCalTable2 = nullptr;
  TMatrixD* timeCalTable2 = nullptr;
  ChannelsMapping* channelsMapping1 = nullptr;
  ChannelsMapping* channelsMapping2 = nullptr;
  TString XY; // XY, YX
  TString XYside; //X,Y
  TString bName;
  TString bName2;
  Bool_t  skipAloneChannels;
};

class ERTelescopeUnpack : public ERUnpack {
 public:
  ERTelescopeUnpack(TString detName);
  virtual ~ERTelescopeUnpack();
  virtual Bool_t Init(SetupConfiguration* setupConf);
  virtual Bool_t DoUnpack(Int_t* data, Int_t size);
  void AddSingleSiStation(TString name, TString ampStName, TString timeStName, 
                          TString ampCalFile, TString timeCalFile, TString XYside,
                          ChannelsMapping* channelsMapping = nullptr,
                          Bool_t skipAloneChannels = kTRUE);
  void AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                          TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                          TString ampCalFile2, TString timeCalFile2, TString XY,
                          ChannelsMapping* channelsMapping1 = nullptr, 
                          ChannelsMapping* channelsMapping2 = nullptr,
                          Bool_t skipAloneChannels = kTRUE);
  void AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile,
                      ChannelsMapping* channelsMapping = nullptr, Bool_t skipAloneChannels = kTRUE);
 protected:
  void AddDigi(float edep, float time, ERChannel channel, TString digiBranchName);
  TString FormBranchName(TString type, int sideCount, TString stName, TString XY, TString XYside, int volInd);
  void FormAllBranches();
  Bool_t ReadCalFiles();
  void DumpStationsInfo();
  Bool_t ApplyCalibration(TMatrixD* ampCalTable, TMatrixD* timeCalTable, 
                          Channel2AmplitudeTime& valueMap);
  Bool_t CheckSetup();
 protected:
  std::map<TString, ERTelescopeStation*> fStations;
  static Int_t fSingleSiStationNewId;
  static Int_t fDoubleSiStationNewId;
  static Int_t fSingleCsIStationNewId;
public:
  ClassDef(ERTelescopeUnpack, 0)
};

#endif
