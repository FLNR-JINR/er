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
                      ChannelMapping* _channelsMapping1, ChannelMapping* _channelsMapping2,
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
  ChannelMapping* channelsMapping1 = nullptr;
  ChannelMapping* channelsMapping2 = nullptr;
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
  void AddSingleSiStation(TString name, TString ampStName, TString timeStName, 
                          TString ampCalFile, TString timeCalFile, TString XYside,
                          ChannelMapping* channelsMapping = nullptr,
                          Bool_t skipAloneChannels = kTRUE);
  void AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                          TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                          TString ampCalFile2, TString timeCalFile2, TString XY,
                          ChannelMapping* channelsMapping1 = nullptr, 
                          ChannelMapping* channelsMapping2 = nullptr,
                          Bool_t skipAloneChannels = kTRUE);
  void AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile,
                      ChannelMapping* channelsMapping = nullptr, Bool_t skipAloneChannels = kTRUE);
 protected:
  virtual void Register();
  virtual std::vector<TString> InputBranchNames() const;
  virtual void UnpackSignalFromStations();
  void CreateDigisFromRawStations(const TString& er_station, const TString& er_branch_name,
                                  const TString& amp_station, const TString& time_station,
                                  TMatrixD* amp_cal_table, TMatrixD* time_cal_table,
                                  const ChannelMapping* channel_mapping, bool skip_alone_channels);  
  void AddDigi(float edep, float time, ERChannel channel, TString digiBranchName);
  TString FormBranchName(TString type, int sideCount, TString stName, TString XY, TString XYside, int volInd);
  void FormAllBranches();
  Bool_t ReadCalFiles();
  void DumpStationsInfo();
  std::pair<float, float> ApplyCalibration(ERChannel channel, Signal amp, Signal time, 
                                           TMatrixD* ampCalTable, TMatrixD* timeCalTable);
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
