#include "ERUnpack.h"

#include <iostream>
#include <vector>

#include "FairLogger.h"

//--------------------------------------------------------------------------------------------------
ERUnpack::ERUnpack(const TString& detector_name)
  : FairUnpack(0,0,0,0,0),
    detector_name_(detector_name)
{}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::Init(const SetupConfiguration* setup_configuration,
                      TChain& input_chain_of_events) {
  if (inited_)
    return kFALSE;
  setup_configuration_ = setup_configuration;
  Register();
  ConnectToInputBranches(input_chain_of_events, this->InputBranchNames());
  inited_ = true;
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::ConnectToInputBranches(TChain& input_chain_of_events, 
                                      const std::vector<TString>& branch_names) {
  if (!setup_configuration_)
    LOG(FATAL) << "[ERUnpack] Setup configuration was not inited." << FairLogger::endl;
  for (const auto& station_name : branch_names) {
    const auto channel_count = setup_configuration_->GetChannelCount(detector_name_, station_name);
    if (channel_count == -1) {
      LOG(FATAL) << "[ERUnpack] Setup does not contain station " << station_name 
                 << FairLogger::endl;
    }
    if (!input_chain_of_events.FindBranch(station_name)) {
      LOG(FATAL) << "[ERUnpack] Input file does not contain branch for station " 
                 << station_name << FairLogger::endl;
    }
    auto* signals = new short[channel_count];
    input_chain_of_events.SetBranchAddress(station_name, signals);
    signals_from_stations_[station_name] = std::make_pair(signals, channel_count);
  }
}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::DoUnpack(Int_t* data, Int_t size) {
  Reset();
  UnpackSignalFromStations();
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::Reset() {
  for (auto& station_name_and_collection : digi_collections_) {
    auto& digi_collection = station_name_and_collection.second;
    digi_collection->Delete();
  }
}
//--------------------------------------------------------------------------------------------------
void FillContainersWithSignals(const Signal* signals, const ERChannel channels_count, Channels& channels, 
                               ChannelToSignal& channel_to_signal) {
  for (ERChannel channel = 0; channel < channels_count; ++channel) {
    if (signals[channel] == no_signal)
      continue;
    channels.push_back(channel);
    channel_to_signal[channel] = signals[channel];
  }                                              
};
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackStation(SignalsAndChannelCount signals_from_station, 
                             ChannelToSignal& channel_to_signal) {
  Channels channels;
  FillContainersWithSignals(signals_from_station.first, signals_from_station.second,
                            channels, channel_to_signal);                     
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(const SignalsAndChannelCount signals_from_amplitude_station,
                                    const SignalsAndChannelCount signals_from_time_station,
                                    ChannelToAmpTime& channel_to_signals,
                                    const bool skip_alone_channels/*= true*/) {
  ChannelToSignal channels_to_time_signals, channels_to_amp_signals;
  Channels amplitude_channels, time_channels;
  FillContainersWithSignals(signals_from_time_station.first, 
                            signals_from_time_station.second,
                            time_channels, channels_to_time_signals);
  FillContainersWithSignals(signals_from_amplitude_station.first, 
                            signals_from_amplitude_station.second,
                            amplitude_channels, channels_to_amp_signals);
  // sort for intersection and difference algorithm                                                     
  std::sort(time_channels.begin(), time_channels.end());
  std::sort(amplitude_channels.begin(), amplitude_channels.end());
  // found intersection in amplitude and time channels
  Channels intersection_channels;
  std::set_intersection(time_channels.begin(), time_channels.end(),
                        amplitude_channels.begin(), amplitude_channels.end(),
                        std::back_inserter(intersection_channels));
  // found alone time and amplitude channels
  Channels alone_time_channels, alone_amplitude_channels;
  std::set_difference(time_channels.begin(), time_channels.end(),
                      intersection_channels.begin(), intersection_channels.end(), 
                      std::inserter(alone_time_channels, alone_time_channels.begin()));
  std::set_difference(amplitude_channels.begin(), amplitude_channels.end(),
                      intersection_channels.begin(), intersection_channels.end(), 
                      std::inserter(alone_amplitude_channels, alone_amplitude_channels.begin()));
  // save it
  for (const auto channel : intersection_channels) {
    channel_to_signals[channel] = std::make_pair(channels_to_amp_signals[channel],
                                                 channels_to_time_signals[channel]);
  }
  if (!skip_alone_channels) {
    for (const auto channel : alone_time_channels) {
      channel_to_signals[channel] = std::make_pair(no_signal, channels_to_time_signals[channel]);
    }
    for (const auto channel : alone_amplitude_channels) {
      channel_to_signals[channel] = std::make_pair(channels_to_amp_signals[channel], no_signal);
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeTACStation(SignalsAndChannelCount signals_from_amplitude_station,
                                       SignalsAndChannelCount signals_from_time_station,
                                       SignalsAndChannelCount signals_from_tac_station,
                                       ChannelToAmpTimeTac& channel_to_signals,
                                       bool skip_alone_channels/*= true*/) {
  ChannelToSignal channels_to_time_signals, channels_to_amp_signals, channels_to_tac_signals;
  Channels amp_channels, time_channels, tac_channels;
  FillContainersWithSignals(signals_from_time_station.first, 
                            signals_from_time_station.second,
                            time_channels, channels_to_time_signals);
  FillContainersWithSignals(signals_from_amplitude_station.first, 
                            signals_from_amplitude_station.second,
                            amp_channels, channels_to_amp_signals);
  FillContainersWithSignals(signals_from_tac_station.first, 
                            signals_from_tac_station.second,
                            tac_channels, channels_to_tac_signals);
  // sort for intersection and difference algorithm                                                     
  std::sort(time_channels.begin(), time_channels.end());
  std::sort(amp_channels.begin(), amp_channels.end());
  std::sort(tac_channels.begin(), tac_channels.end());
  // find intersection in amplitude and time channels
  Channels amp_time_intersection_channels;
  std::set_intersection(time_channels.begin(), time_channels.end(),
                        amp_channels.begin(), amp_channels.end(),
                        std::back_inserter(amp_time_intersection_channels));
  // intersect with tac
  Channels intersection_channels;
  std::set_intersection(amp_time_intersection_channels.begin(), amp_time_intersection_channels.end(),
                        tac_channels.begin(), tac_channels.end(),
                        std::back_inserter(intersection_channels));
  // find alone time and amplitude channels
  Channels time_dif_channels;
  Channels amp_dif_channels;
  Channels tac_dif_channels;
  std::set_difference(time_channels.begin(), time_channels.end(),
                      intersection_channels.begin(), intersection_channels.end(), 
                      std::inserter(time_dif_channels, time_dif_channels.begin()));
  std::set_difference(amp_channels.begin(), amp_channels.end(),
                      intersection_channels.begin(), intersection_channels.end(), 
                      std::inserter(amp_dif_channels, amp_dif_channels.begin()));
  std::set_difference(tac_channels.begin(), tac_channels.end(),
                      intersection_channels.begin(), intersection_channels.end(), 
                      std::inserter(tac_dif_channels, tac_dif_channels.begin()));
  for (const auto channel : intersection_channels) {
    channel_to_signals[channel] = std::make_tuple(channels_to_amp_signals[channel],
                                                  channels_to_time_signals[channel],
                                                  channels_to_tac_signals[channel]);
  }
  if (!skip_alone_channels){
    for (const auto channel : time_dif_channels)
      channel_to_signals[channel] = std::make_tuple(no_signal, channels_to_time_signals[channel], no_signal);
    for (const auto channel : amp_dif_channels)
      channel_to_signals[channel] = std::make_tuple(channels_to_amp_signals[channel], no_signal, no_signal);
    for (const auto channel : tac_dif_channels)
      channel_to_signals[channel] = std::make_tuple(no_signal, no_signal, channels_to_tac_signals[channel]);
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERUnpack)