#ifndef ERUnpack_H
#define ERUnpack_H

#include <tuple>

#include "TString.h"
#include "TClonesArray.h"
#include "TMatrixD.h"
#include "TChain.h"

#include "FairUnpack.h"

#include "SetupConfiguration.h"

#include "ERBeamTimeCommon.h"

class ERUnpack : public FairUnpack {
 public:
  
  ERUnpack(const TString& detector_name);
  virtual ~ERUnpack() = default;
  virtual Bool_t Init(){return kTRUE;}
  virtual Bool_t Init(const SetupConfiguration*, TChain& input_chain_of_events);
  virtual Bool_t DoUnpack(Int_t* data, Int_t size);
  virtual void  Reset();
  TString DetectorName() { return detector_name_; }
  Bool_t IsInited() { return inited_; }
 protected:
  virtual void ConnectToInputBranches(TChain& input_chain_of_events, 
                                      const std::vector<TString>& branch_names);
  virtual void UnpackSignalFromStations() = 0;
  virtual std::vector<TString> InputBranchNames() const = 0;
  void UnpackStation(SignalsAndChannelCount signals_from_station, 
                     ChannelToSignal& channel_to_signal);
  void UnpackAmpTimeStation(SignalsAndChannelCount signals_from_amplitude_station,
                            SignalsAndChannelCount signals_from_time_station,
                            ChannelToAmpTime& channel_to_signals,
                            bool skip_alone_channels = true);
  void UnpackAmpTimeTACStation(SignalsAndChannelCount signals_from_amplitude_station,
                               SignalsAndChannelCount signals_from_time_station,
                               SignalsAndChannelCount signals_from_tac_station,
                               ChannelToAmpTimeTac& channel_to_signals,
                               bool skip_alone_channels = true);
  const SetupConfiguration* setup_configuration_;
  bool inited_ = false;
  TString detector_name_;
  std::map<TString, SignalsAndChannelCount> signals_from_stations_;
  std::map<TString, TClonesArray*> digi_collections_;
  ClassDef(ERUnpack, 0)
};

#endif
