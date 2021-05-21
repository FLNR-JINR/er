#include "ERNDUnpack.h"

#include <map>
#include <tuple>

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERDetectorList.h"
#include "ERNDDigi.h"
#include "ERSupport.h"

//--------------------------------------------------------------------------------------------------
ERNDUnpack::ERNDUnpack(TString detName, TString ampStation, TString timeStation, TString tacStation,
                       TString ampCalFile, TString timeCalFile, TString tacCalFile,
                       ChannelMapping* channelsMapping /*=nullptr*/,
                       Bool_t skipAloneChannels/*= kTRUE*/) : 
    ERUnpack(detName),
    fAmpStation(ampStation),
    fTimeStation(timeStation),
    fTACStation(tacStation),
    fChannelMapping(channelsMapping),
    fSkipAloneChannels(skipAloneChannels)
{
    if (ampCalFile != "")
        fAmpCalTable = ReadCalFile(ampCalFile);
    if (timeCalFile != "")
        fTimeCalTable = ReadCalFile(timeCalFile);
    if (tacCalFile != "")
        fTACCalTable = ReadCalFile(tacCalFile);
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  digi_collections_["NDDigis"] = new TClonesArray("ERNDDigi", 10);
	ioman->Register("NDDigi", "ND", digi_collections_["NDDigis"], kTRUE /* save to file */);
  if (!CheckSetup())
    Fatal("Init", "Error in ERNDUnpack setup checking !");
}
//--------------------------------------------------------------------------------------------------
std::vector<TString> ERNDUnpack::InputBranchNames() const {
  return {fAmpStation, fTimeStation, fTACStation};
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::UnpackSignalFromStations() {
  ChannelToAmpTimeTac channel_to_signals;
  UnpackAmpTimeTACStation(signals_from_stations_[fAmpStation],
                          signals_from_stations_[fTimeStation],
                          signals_from_stations_[fTACStation],
                          channel_to_signals,
                          fSkipAloneChannels);
  for (auto itValue : channel_to_signals) {
    ERChannel channel = itValue.first;
    float amp, time, tac;
    std::tie(amp, time, tac) = itValue.second;
    ApplyCalibrations(channel, amp /*[MeV]*/, time, tac);
    AddNDDigi(amp,time,tac, GetChannelNumber(channel, fChannelMapping));
  }
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::ApplyCalibrations(const ERChannel channel, float& amp, float& time, float& tac) {
  const auto applyCalibration = [this, channel](float& value, const TMatrixD* table) {
    if (!table)
      return;
    if (channel >= table->GetNrows()){
      LOG(FATAL) << "Channel " << channel << " not found in amplitude calibration table of detector " 
                  << detector_name_ << FairLogger::endl;
    }
    value = value*(*table)[channel][1] + (*table)[channel][0];
  };
  applyCalibration(amp, fAmpCalTable);
  applyCalibration(time, fTimeCalTable);
  applyCalibration(tac, fTACCalTable);
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::AddNDDigi(const float edep, const float time, const float tac, 
                           const ERChannel channelNb) { 
    auto* digiCollection = digi_collections_["NDDigis"];
    new((*digiCollection) [digiCollection->GetEntriesFast()])
        ERNDDigi(channelNb, edep, -1. /*lightYield*/, time, -1. /*neutronProb*/, tac);
}
//--------------------------------------------------------------------------------------------------
Bool_t ERNDUnpack::CheckSetup() {
    const auto stationsInConfig = setup_configuration_->GetStationList(detector_name_);
    const auto stationInConfig = [stationsInConfig, this](const TString stationName,
                                                          const TString type) {
        if (stationsInConfig.find(stationName) == stationsInConfig.end()){
            LOG(FATAL) << type << " station " << stationName << " of ND " << detector_name_ 
                       <<  " not found in setup configuration file" << FairLogger::endl;
            return kFALSE;
        }
        return kTRUE;
    };
    if (!stationInConfig(fAmpStation, "Amplitude"))
        return kFALSE;
    if (!stationInConfig(fTimeStation, "Time"))
        return kFALSE;
    if (!stationInConfig(fTACStation, "TAC"))
        return kFALSE;
    return kTRUE;
}