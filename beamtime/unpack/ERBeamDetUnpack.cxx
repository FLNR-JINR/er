#include "ERBeamDetUnpack.h"

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "ERDigi.h"

//--------------------------------------------------------------------------------------------------
ERBeamDetUnpack::ERBeamDetUnpack(const TString& detName)
  : ERUnpack(detName),
    mwpc_amplitude_station_to_time_channel_({{"MWPC1", 0}, {"MWPC2", 1}, {"MWPC3", 2}, {"MWPC4", 3}}),
    mwpc_branch_names_({{"MWPC1", "BeamDetMWPCDigiX1"}, {"MWPC2", "BeamDetMWPCDigiY1"},
                        {"MWPC3", "BeamDetMWPCDigiX2"}, {"MWPC4", "BeamDetMWPCDigiY2"}})
{}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::SetToFCalibration(const float F3_A, const float F3_B, 
                                        const float F5_A, const float F5_B) {
  F3_time_calibration_A = F3_A; 
  F3_time_calibration_B = F3_B;
  F5_time_calibration_A = F5_A;
  F5_time_calibration_B = F5_B; 
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::SetMWPCCalibration(const float A, const float B) {
  MWPC_time_calibration_A = A;
  MWPC_time_calibration_B = B;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::Register() {
	if (!setup_configuration_)
    LOG(FATAL) << "[ERBeamDetUnpack] Setup was not inited" << FairLogger::endl;
	FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "[ERBeamDetUnpack] No FairRootManager" << FairLogger::endl;
	const std::map<TString, unsigned short> stList = setup_configuration_->GetStationList(detector_name_);
	for (auto itSt : stList){
		if (itSt.first == TString("F3")){
			digi_collections_["BeamDetToFDigi1"] = new TClonesArray("ERDigi",1000);
			ioman->Register("BeamDetToFDigi1", "BeamDet", digi_collections_["BeamDetToFDigi1"], kTRUE);
		}
		if (itSt.first == TString("F5")){
			digi_collections_["BeamDetToFDigi2"] = new TClonesArray("ERDigi",1000);
			ioman->Register("BeamDetToFDigi2", "BeamDet", digi_collections_["BeamDetToFDigi2"], kTRUE);
		}
		for (auto itMwpcStation : mwpc_amplitude_station_to_time_channel_){
			TString bName = mwpc_branch_names_.at(itMwpcStation.first);
			if (itSt.first == itMwpcStation.first){
				digi_collections_[bName] = new TClonesArray("ERDigi",1000);
				ioman->Register(bName, "BeamDet", digi_collections_[bName], kTRUE);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::ConnectToInputBranches(TChain& input_chain_of_events) {
  if (!setup_configuration_)
    LOG(FATAL) << "[ERBeamDetUnpack] Setup configuration was not inited." << FairLogger::endl;
  for (const auto& station_name : {TString("F3"), TString("tF3"), TString("F5"), TString("tF5"),
                                   TString("tMWPC"), TString("MWPC1"), TString("MWPC2"),
                                   TString("MWPC3"), TString("MWPC4")}) {
    const auto channel_count = setup_configuration_->GetChannelCount(detector_name_, station_name);
    if (channel_count == -1) {
      LOG(FATAL) << "[ERBeamDetUnpack] Setup does not contain station " << station_name 
                 << FairLogger::endl;
    }
    if (!input_chain_of_events.FindBranch(station_name)) {
      LOG(FATAL) << "[ERBeamDetUnpack] Input file does not contain branch for station " 
                 << station_name << FairLogger::endl;
    }
    auto* signals = new short[channel_count];
    input_chain_of_events.SetBranchAddress(station_name, signals);
    signals_from_stations_[station_name] = std::make_pair(signals, channel_count);
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::UnpackSignalFromStations() {
  UnpackToFStation("F3", "tF3", 1);
  UnpackToFStation("F5", "tF5", 2);
  UnpackMWPCStations();
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::UnpackToFStation(const TString& amplitude_station, 
                                       const TString& time_station, const ushort tof_number) {
  ChannelToAmplitudeAndTimeSignals channel_to_signals;
  UnpackAmpTimeStation(signals_from_stations_[amplitude_station], 
                       signals_from_stations_[time_station], 
                       channel_to_signals);
  if (channel_to_signals.size() == 4) {
    float time = 0., amplitude = 0.;
    for (const auto channel_to_amplitude_and_time : channel_to_signals) {
      const auto amplitude_and_time = channel_to_amplitude_and_time.second;
      amplitude += amplitude_and_time.first;
      time += amplitude_and_time.second;
    }
    time = CalibrateToF(time * 0.25, tof_number);
    AddToFDigi(amplitude, time, tof_number);
  } else {
    LOG(DEBUG) << "[ERBeamDetUnpack] Wrong PMT number in ToF number " << tof_number << FairLogger::endl;
  }                               
}
//--------------------------------------------------------------------------------------------------
float ERBeamDetUnpack::CalibrateToF(const float time, const ushort tof_number) {
  if (tof_number == 1) {
    return time * F3_time_calibration_A + F3_time_calibration_B;
  } else if (tof_number == 2) {
    return time * F5_time_calibration_A + F5_time_calibration_B;
  } else {
    LOG(FATAL) << "[ERBeamDetUnpack] Unknown TOF number " << tof_number << FairLogger::endl;
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::UnpackMWPCStations() {
  ChannelToSignal channels_to_time_signals;
  UnpackStation(signals_from_stations_["tMWPC"], channels_to_time_signals);
  for (const auto& amplitude_station_and_time_channel : mwpc_amplitude_station_to_time_channel_) {
    const auto& amplitude_station = amplitude_station_and_time_channel.first;
    const auto time_channel = amplitude_station_and_time_channel.second;
    if (channels_to_time_signals.find(time_channel) == channels_to_time_signals.end()) {
      LOG(DEBUG) << "[ERBeamDetUnpack] MWPC time signal not found for amplitude station " 
                 << amplitude_station << FairLogger::endl;
      continue;
    }
    ChannelToSignal channels_to_amplitude_signals;
    UnpackStation(signals_from_stations_[amplitude_station], channels_to_amplitude_signals);
    for (const auto channel_and_signal_from_amplitude_station : channels_to_amplitude_signals) {
      const auto channel = channel_and_signal_from_amplitude_station.first;
      auto amplitude = channel_and_signal_from_amplitude_station.second;
      float time = channels_to_time_signals[time_channel] * MWPC_time_calibration_A 
                   + MWPC_time_calibration_B;
      AddMWPCDigi(amplitude, time, amplitude_station, 
                  GetChannelNumber(channel, &mwpc_channels_mapping_));
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddToFDigi(float edep, float time, ushort tof_number) {
  ERDigi *digi; 
  if(tof_number == 1) {
    digi = new((*digi_collections_["BeamDetToFDigi1"])[digi_collections_["BeamDetToFDigi1"]->GetEntriesFast()])
        ERDigi(edep, time, tof_number);
  }
  if(tof_number == 2) {
    digi = new((*digi_collections_["BeamDetToFDigi2"])[digi_collections_["BeamDetToFDigi2"]->GetEntriesFast()])
    	ERDigi(edep, time, tof_number);
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddMWPCDigi(float edep, float time, TString mwpcSt, Int_t wireNb) {
  TString bName = mwpc_branch_names_.at(mwpcSt);
  new((*digi_collections_[bName])[digi_collections_[bName]->GetEntriesFast()])
    ERDigi(edep, time, wireNb+1);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetUnpack)
