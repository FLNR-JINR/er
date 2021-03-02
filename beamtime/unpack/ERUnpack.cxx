#include "ERUnpack.h"

#include <iostream>
#include <vector>


#include "FairLogger.h"

#include "DetEventStation.h"
#include "DetMessage.h"

using namespace std;
//--------------------------------------------------------------------------------------------------
ERUnpack::ERUnpack(const TString& detector_name)
  : FairUnpack(0,0,0,0,0),
    detector_name_(detector_name)
{}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::Init(std::shared_ptr<const SetupConfiguration> setup_configuration,
                      TChain& input_chain_of_events) {
  if (inited_)
    return kFALSE;
  setup_configuration_ = setup_configuration;
  Register();
  ConnectToInputBranches(input_chain_of_events);
  inited_ = true;
  return kTRUE;
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
void ERUnpack::UnpackAmpTimeStation(const SignalsAndChannelCount signals_from_amplitude_station,
                                    const SignalsAndChannelCount signals_from_time_station,
                                    ChannelToAmplitudeAndTimeSignals& channel_to_signals,
                                    const bool skip_alone_channels/*= true*/) {
  const auto fill_containers_with_signals = 
      [this](const Signal* signals, const Channel channels_count, Channels& channels, 
                  ChannelToSignal& channel_to_signal) {
          for (Channel channel = 0; channel < channels_count; ++channel) {
            if (signals[channel] == no_signal)
              continue;
            channels.push_back(channel);
            channel_to_signal[channel] = signals[channel];
          }                                              
  };
  ChannelToSignal channels_to_time_signals, channels_to_amplitude_signals;
  Channels amplitude_channels, time_channels;
  fill_containers_with_signals(signals_from_time_station.first, 
                               signals_from_time_station.second,
                               time_channels, channels_to_time_signals);
  fill_containers_with_signals(signals_from_amplitude_station.first, 
                               signals_from_amplitude_station.second,
                               amplitude_channels, channels_to_amplitude_signals);
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
    channel_to_signals[channel] = std::make_pair(channels_to_time_signals[channel],
                                                 channels_to_amplitude_signals[channel]);
  }
  if (!skip_alone_channels) {
    for (const auto channel : alone_time_channels) {
      channel_to_signals[channel] = std::make_pair(no_signal, channels_to_time_signals[channel]);
    }
    for (const auto channel : alone_amplitude_channels) {
      channel_to_signals[channel] = std::make_pair(channels_to_amplitude_signals[channel], no_signal);
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackStation(SignalsAndChannelCount signals_from_station, 
                             ChannelToSignal& channel_to_signal) {
  const auto* signals = signals_from_station.first;
  const auto channel_count = signals_from_station.second;
  for (Channel channel = 0; channel < channel_count; channel++) {
    if (signals[channel] == no_signal)
      continue;
    channel_to_signal[channel] = signals[channel];
  }                          
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                    std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap,
                                    Bool_t skipAloneChannels/* = kTRUE*/) {
    const std::map<TString, unsigned short> stList = setup_configuration_->GetStationList(detector_name_);
    TString ampEventElement,timeEventElement;
    ampEventElement.Form("%s_%s",detector_name_.Data(),ampStation.Data());
    timeEventElement.Form("%s_%s",detector_name_.Data(),timeStation.Data());

    DetEventStation* ampStationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
    DetEventStation* timeStationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
    if (!ampStationEvent || !timeStationEvent){
        LOG(FATAL) << "Amplitude event element or time event element not found for " << ampStation << FairLogger::endl;
        return;
    }

    TClonesArray* ampMessages = ampStationEvent->GetDetMessages();
    TClonesArray* timeMessages = timeStationEvent->GetDetMessages();

    // just copy TClonesArray to map and vector
    map<Int_t,Double_t> time_map,amp_map;
    vector<Int_t> amp_channels, time_channels;
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp_map[ampMes->GetStChannel()] = ampMes->GetValue();
        amp_channels.push_back(ampMes->GetStChannel());
    }

    for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
        DetMessage* timeMes = (DetMessage*)timeMessages->At(iTimeMessage);
        time_map[timeMes->GetStChannel()] = timeMes->GetValue();
        time_channels.push_back(timeMes->GetStChannel());
    }

    // sort for intersection and difference algorithm
    sort(time_channels.begin(), time_channels.end());
    sort(amp_channels.begin(), amp_channels.end());

    // found intersection in amplitude and time channels
    vector<Int_t> intersection_channels;
    set_intersection(time_channels.begin(), time_channels.end(),
                     amp_channels.begin(), amp_channels.end(),
                     back_inserter(intersection_channels));
    
    // found alone time and amplitude channels
    vector<Int_t> time_dif_channels;
    vector<Int_t> amp_dif_channels;
    set_difference(time_channels.begin(), time_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(time_dif_channels, time_dif_channels.begin()));
    set_difference(amp_channels.begin(), amp_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(amp_dif_channels, amp_dif_channels.begin()));

    LOG(DEBUG) << "Time and amplitude coincidence channels number: " << intersection_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone time channels number: " << time_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone amplitude channels number: " << amp_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Option skip alone channels is " << skipAloneChannels
               << FairLogger::endl;

    // save it
    for (auto iIntesectionChannel : intersection_channels)
        valueMap[iIntesectionChannel] = make_pair(amp_map[iIntesectionChannel],
                                                  time_map[iIntesectionChannel]);

    if (!skipAloneChannels){
        for (auto iAloneChannel : time_dif_channels)
            valueMap[iAloneChannel] = make_pair(-1.,time_map[iAloneChannel]);
        for (auto iAloneChannel : amp_dif_channels)
            valueMap[iAloneChannel] = make_pair(amp_map[iAloneChannel],-1.);
    }
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeTACStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                       TString tacStation, std::map<Int_t, std::tuple<Double_t, Double_t, Double_t> >& valueMap,
                                       Bool_t skipAloneChannels/* = kTRUE*/) {
    const auto stList = setup_configuration_->GetStationList(detector_name_);
    TString ampEventElement,timeEventElement, tacEventElement;
    ampEventElement.Form("%s_%s",detector_name_.Data(),ampStation.Data());
    timeEventElement.Form("%s_%s",detector_name_.Data(),timeStation.Data());
    tacEventElement.Form("%s_%s",detector_name_.Data(),tacStation.Data());

    DetEventStation* ampStationEvent = static_cast<DetEventStation*>(detEvent->GetChild(ampEventElement));
    DetEventStation* timeStationEvent = static_cast<DetEventStation*>(detEvent->GetChild(timeEventElement));
    DetEventStation* tacStationEvent = static_cast<DetEventStation*>(detEvent->GetChild(tacEventElement));
    if (!ampStationEvent || !timeStationEvent || !tacStationEvent){
        LOG(FATAL) << "Amplitude event element, time or tac event element not found for " << ampStation << FairLogger::endl;
        return;
    }

    TClonesArray* ampMessages = ampStationEvent->GetDetMessages();
    TClonesArray* timeMessages = timeStationEvent->GetDetMessages();
    TClonesArray* tacMessages = tacStationEvent->GetDetMessages();

    // just copy TClonesArray to map and vector
    map<Int_t,Double_t> time_map,amp_map,tac_map;
    vector<Int_t> amp_channels, time_channels, tac_channels;
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp_map[ampMes->GetStChannel()] = ampMes->GetValue();
        amp_channels.push_back(ampMes->GetStChannel());
    }

    for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
        DetMessage* timeMes = (DetMessage*)timeMessages->At(iTimeMessage);
        time_map[timeMes->GetStChannel()] = timeMes->GetValue();
        time_channels.push_back(timeMes->GetStChannel());
    }

    for (Int_t iTacMessage(0); iTacMessage < tacMessages->GetEntriesFast(); ++iTacMessage){
        DetMessage* tacMes = (DetMessage*)tacMessages->At(iTacMessage);
        tac_map[tacMes->GetStChannel()] = tacMes->GetValue();
        tac_channels.push_back(tacMes->GetStChannel());
    }

    // sort for intersection and difference algorithm
    sort(time_channels.begin(), time_channels.end());
    sort(amp_channels.begin(), amp_channels.end());
    sort(tac_channels.begin(), tac_channels.end());

    // found intersection in amplitude and time channels
    vector<Int_t> amp_time_intersection_channels;
    set_intersection(time_channels.begin(), time_channels.end(),
                     amp_channels.begin(), amp_channels.end(),
                     back_inserter(amp_time_intersection_channels));

    // intersect with tac
    vector<Int_t> intersection_channels;
    set_intersection(amp_time_intersection_channels.begin(), amp_time_intersection_channels.end(),
                     tac_channels.begin(), tac_channels.end(),
                     back_inserter(intersection_channels));
    
    // found alone time and amplitude channels
    vector<Int_t> time_dif_channels;
    vector<Int_t> amp_dif_channels;
    vector<Int_t> tac_dif_channels;

    set_difference(time_channels.begin(), time_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(time_dif_channels, time_dif_channels.begin()));
    set_difference(amp_channels.begin(), amp_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(amp_dif_channels, amp_dif_channels.begin()));
    set_difference(tac_channels.begin(), tac_channels.end(),
                   intersection_channels.begin(), intersection_channels.end(), 
                   inserter(tac_dif_channels, tac_dif_channels.begin()));

    LOG(DEBUG) << "Time, amplitude and tac coincidence channels number: " << intersection_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone time channels number: " << time_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone amplitude channels number: " << amp_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Alone tac channels number: " << tac_dif_channels.size() 
               << FairLogger::endl;
    LOG(DEBUG) << "Option skip alone channels is " << skipAloneChannels
               << FairLogger::endl;

    // save it
    for (auto iIntesectionChannel : intersection_channels)
        valueMap[iIntesectionChannel] = make_tuple(amp_map[iIntesectionChannel],
                                                   time_map[iIntesectionChannel],
                                                   tac_map[iIntesectionChannel]);

    if (!skipAloneChannels){
        for (auto iAloneChannel : time_dif_channels)
            valueMap[iAloneChannel] = make_tuple(-1., time_map[iAloneChannel], -1.);
        for (auto iAloneChannel : amp_dif_channels)
            valueMap[iAloneChannel] = make_tuple(amp_map[iAloneChannel], -1., -1.);
        for (auto iAloneChannel : tac_dif_channels)
            valueMap[iAloneChannel] = make_tuple(-1., -1., tac_map[iAloneChannel]);
    }
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackStation(DetEventDetector* detEvent, TString station, std::map<Int_t,Double_t>& valueMap){
    const std::map<TString, unsigned short> stList = setup_configuration_->GetStationList(detector_name_);
    Double_t amp = 0.;
    Int_t channel = -1;
    TString eventElementName;
    eventElementName.Form("%s_%s",detector_name_.Data(),station.Data());

    DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(eventElementName);

    if (!stationEvent){
        LOG(FATAL) << "Event element not found for " << station << FairLogger::endl;
        return;
    }

    TClonesArray* ampMessages = stationEvent->GetDetMessages();

    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp = ampMes->GetValue();
        channel = ampMes->GetStChannel();
        valueMap[channel] = amp;
    }   
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERUnpack)