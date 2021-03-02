#ifndef ERBeamDetUnpack_H
#define ERBeamDetUnpack_H

#include "TString.h"

#include "ERUnpack.h"

class ERBeamDetUnpack : public ERUnpack {
 public:
  ERBeamDetUnpack(const TString& detector_name);
  virtual ~ERBeamDetUnpack() = default;
  void SetToFCalibration(float F3_A, float F3_B, float F5_A, float F5_B);
  void SetMWPCCalibration(float A, float B);
  void SetMWPCMapping(const ChannelMapping& mapping) { mwpc_channels_mapping_ = mapping;}
 protected:
  virtual void Register();
  virtual void ConnectToInputBranches(TChain& input_chain_of_events);
  virtual void UnpackSignalFromStations();
  void UnpackToFStation(const TString& amplitude_station, const TString& time_station,
                        ushort tof_number);
  void UnpackMWPCStations();
  float CalibrateToF(float time, ushort tof_number);
  void AddToFDigi(float edep, float time, ushort tof_number);
  void AddMWPCDigi(float edep, float time, TString mwpcSt, Int_t wireNb);
  const std::map<TString, ERChannel> mwpc_amplitude_station_to_time_channel_;
  const std::map<TString, TString> mwpc_branch_names_;
  ChannelMapping mwpc_channels_mapping_;
  float F3_time_calibration_A = 0.125;
  float F3_time_calibration_B = 0.;
  float F5_time_calibration_A = 0.125;
  float F5_time_calibration_B = 0.;
  float MWPC_time_calibration_A = 0.125;
  float MWPC_time_calibration_B = 0.;
  ClassDef(ERBeamDetUnpack, 0)
};


#endif
