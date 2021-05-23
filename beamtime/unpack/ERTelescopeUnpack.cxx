#include "ERTelescopeUnpack.h"

#include <fstream>

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERSupport.h"
#include "ERDigi.h"

//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::ERTelescopeUnpack(TString detName):
 ERUnpack(detName) { }
//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::~ERTelescopeUnpack(){
    for (auto itStation : fStations){
        if (itStation.second)
            delete itStation.second;
    }
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::Register() {
    if (fStations.size() == 0)
        LOG(FATAL) << "No Stations in ERTelescopeUnpack !" << FairLogger::endl;
    if (!CheckSetup())
        LOG(FATAL) << "Error in ERTelescopeUnpack setup checking !" << FairLogger::endl;
    if (!ReadCalFiles())
        LOG(FATAL) << "Problem in ReadCalFiles!" << FairLogger::endl;
    FormAllBranches();
    DumpStationsInfo();
}
//--------------------------------------------------------------------------------------------------
std::vector<TString> ERTelescopeUnpack::InputBranchNames() const {
  std::vector<TString> station_names;
  for (const auto& it_station : fStations) {
    auto* station = it_station.second;
    for (const auto& station_name : {station->ampStName, station->timeStName, 
                                     station->ampStName2, station->timeStName2}) {
      if (station_name != "") {
        station_names.push_back(station_name);
      }
    }
  }
  return station_names;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::CreateDigisFromRawStations(
    const TString& er_station, const TString& er_branch_name,
    const TString& amp_station, const TString& time_station,
    TMatrixD* amp_cal_table, TMatrixD* time_cal_table,
    const ChannelMapping* channel_mapping, bool skip_alone_channels) {            
  ChannelToAmpTime channel_to_signals;
  UnpackAmpTimeStation(signals_from_stations_[amp_station], 
                       signals_from_stations_[time_station], 
                       channel_to_signals,
                       skip_alone_channels);
  for (const auto it : channel_to_signals){
    const auto raw_channel = it.first;
    const auto raw_amp = it.second.first;
    const auto raw_time = it.second.second;
    const auto amp_and_time = ApplyCalibration(raw_channel, raw_amp, raw_time,
                                               amp_cal_table, time_cal_table);
    AddDigi(amp_and_time.first, amp_and_time.second, 
            GetChannelNumber(it.first, channel_mapping), er_branch_name);
  }                                
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::UnpackSignalFromStations() {
  for (auto itStation : fStations) {
    LOG(DEBUG) << itStation.first << " unpacking ..." << FairLogger::endl;
    const auto* station = itStation.second;
    CreateDigisFromRawStations(itStation.first, station->bName, station->ampStName, station->timeStName,
                               station->ampCalTable, station->timeCalTable, itStation.second->channelsMapping1,
                               station->skipAloneChannels);
    if (station->sideCount == 2) {
      CreateDigisFromRawStations(itStation.first, station->bName2, station->ampStName2, station->timeStName2,
                                 station->ampCalTable2, station->timeCalTable2, itStation.second->channelsMapping2,
                                 station->skipAloneChannels);
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDigi(float edep, float time, ERChannel stripNb, TString digiBranchName)
{
  new((*digi_collections_[digiBranchName])
        [digi_collections_[digiBranchName]->GetEntriesFast()])ERDigi(edep, time, stripNb);
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeUnpack::FormBranchName(TString type, Int_t sideCount, TString stName, 
		                                  TString XY, TString XYside) {
    TString branch_name;
    if (type == "CsI") {
        branch_name.Form("TelescopeDigi_%s_%s",detector_name_.Data(),stName.Data());
    } else {
        if (sideCount == 1) {
            branch_name.Form("TelescopeDigi_%s_%s_%s",detector_name_.Data(),stName.Data(),
                                                      XYside.Data());
        } else {
            branch_name.Form("TelescopeDigi_%s_%s_%s_%s",detector_name_.Data(), stName.Data(),
                                                         XY.Data(), XYside.Data());
        }
    }
    return branch_name;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSingleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampCalFile, TString timeCalFile, TString XYside,
                                           ChannelMapping* channelsMapping/* = nullptr*/,
                                           Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "","", XYside,
                                                    channelsMapping, nullptr, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                                           TString ampCalFile2, TString timeCalFile2, TString XY,
                                           ChannelMapping* channelsMapping1/* = nullptr*/,
                                           ChannelMapping* channelsMapping2/* = nullptr*/,
                                           Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 2, ampStName, timeStName, ampStName2, timeStName2, ampCalFile, timeCalFile,
                            ampCalFile2, timeCalFile2, XY, "", channelsMapping1, channelsMapping2, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile,
                                      ChannelMapping* channelsMapping /* = nullptr*/, Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "CsI", -1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "", "", "",
                                                    channelsMapping, nullptr, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::FormAllBranches(){
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  for (auto itStation : fStations){
      if( itStation.second->sideCount == 2){
          if (itStation.second->XY == "XY"){
              itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","X");
              itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","Y");
          }
          else{
              itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","Y");
              itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","X");
          }
      }
      else {
          itStation.second->bName = FormBranchName(itStation.second->type,
                                                  itStation.second->sideCount,
                                                  itStation.first,"",
                                                  itStation.second->XYside);
      }
  }
  for (auto itStation : fStations){
      TString bName = itStation.second->bName;
      if (itStation.second->type == "Si") {
          digi_collections_[bName] = new TClonesArray("ERDigi", 10);
          ioman->Register(bName,detector_name_, digi_collections_[bName], kTRUE);
          if (itStation.second->sideCount == 2){
              TString bName2 = itStation.second->bName2;
              digi_collections_[bName2] = new TClonesArray("ERDigi", 10);
              ioman->Register(bName2,detector_name_, digi_collections_[bName2], kTRUE);
          }
      }
      if (itStation.second->type == "CsI") {
          digi_collections_[bName] = new TClonesArray("ERDigi", 10);
          ioman->Register(bName,detector_name_, digi_collections_[bName], kTRUE);
      }
  }
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::DumpStationsInfo(){
    LOG(INFO) << "!!! Stations info: " << FairLogger::endl; 
    for (auto itStation : fStations){
        LOG(INFO) << "\t" << itStation.first << FairLogger::endl;
        LOG(INFO) << "\t\ttype : " << itStation.second->type << FairLogger::endl <<
                "\t\tsideCount : " << itStation.second->sideCount << FairLogger::endl <<
                "\t\tampStName : " << itStation.second->ampStName << FairLogger::endl << 
                "\t\ttimeStName : " << itStation.second->timeStName << FairLogger::endl << 
                "\t\tampStName2 : " << itStation.second->ampStName2 << FairLogger::endl << 
                "\t\ttimeStName2 : " << itStation.second->timeStName2 << FairLogger::endl <<
                "\t\tXY : " << itStation.second->XY << FairLogger::endl <<
                "\t\tXYside : " << itStation.second->XYside << FairLogger::endl <<
                "\t\tbName : " << itStation.second->bName << FairLogger::endl <<
                "\t\tbName2 : " << itStation.second->bName2 << FairLogger::endl;
        if (itStation.second->ampCalTable){
            LOG(INFO) << "\t\tampCalFile : " << itStation.second->ampCalFile << FairLogger::endl;
            LOG(INFO) << "\t\tampCalTable : " << FairLogger::endl;
            itStation.second->ampCalTable->Print();
        }
        if (itStation.second->timeCalTable){
            LOG(INFO) << "\t\ttimeCalFile : " << itStation.second->timeCalFile << FairLogger::endl;
            LOG(INFO) << "\t\ttimeCalTable : " << FairLogger::endl;
            itStation.second->timeCalTable->Print();
        }
        if (itStation.second->ampCalTable2){
            LOG(INFO) << "\t\tampCalFile2 : " << itStation.second->ampCalFile2 << FairLogger::endl;
            LOG(INFO) << "\t\tampCalTable2 : " << FairLogger::endl;
            itStation.second->ampCalTable2->Print();
        }
        if (itStation.second->timeCalTable2){
            LOG(INFO) << "\t\ttimeCalFile2 : " << itStation.second->timeCalFile2 << FairLogger::endl;
            LOG(INFO) << "\t\ttimeCalTable2 : " << FairLogger::endl;
            itStation.second->timeCalTable2->Print();
        }
    }
}
//--------------------------------------------------------------------------------------------------
ERTelescopeStation::ERTelescopeStation(TString _type, Int_t _sideCount, TString _ampStName, TString _timeStName,
                                       TString _ampStName2, TString _timeStName2, TString _ampCalFile, TString _timeCalFile,
                                       TString _ampCalFile2, TString _timeCalFile2, TString _XY, TString _XYside,
                                       ChannelMapping* _channelsMapping1, ChannelMapping* _channelsMapping2,
                                       Bool_t _skipAloneChannels):
    type(_type),
    sideCount(_sideCount),
    ampStName(_ampStName),
    timeStName(_timeStName),
    ampStName2(_ampStName2),
    timeStName2(_timeStName2),
    ampCalFile(_ampCalFile),
    timeCalFile(_timeCalFile),
    ampCalFile2(_ampCalFile2),
    timeCalFile2(_timeCalFile2),
    XY(_XY),
    XYside(_XYside),
    bName(""),
    bName2(""),
    channelsMapping1(_channelsMapping1),
    channelsMapping2(_channelsMapping2),
    skipAloneChannels(_skipAloneChannels)
{
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::ReadCalFiles(){
    for (auto itStation : fStations){
        
        if (itStation.second->ampCalFile != ""){
            itStation.second->ampCalTable = ReadCalFile(itStation.second->ampCalFile);
            if (!itStation.second->ampCalTable)
                return kFALSE;
        }

        if (itStation.second->timeCalFile != ""){
            itStation.second->timeCalTable = ReadCalFile(itStation.second->timeCalFile);
            if (!itStation.second->timeCalTable)
                return kFALSE;
        }
              
        if (itStation.second->sideCount == 2 && itStation.second->ampCalFile2 != ""){
            itStation.second->ampCalTable2 = ReadCalFile(itStation.second->ampCalFile2);
            if (!itStation.second->ampCalTable2)
                return kFALSE;
        }

        if (itStation.second->sideCount == 2 && itStation.second->timeCalFile2 != ""){
            itStation.second->timeCalTable2 = ReadCalFile(itStation.second->timeCalFile2);
            if (!itStation.second->timeCalTable2)
                return kFALSE;
        }

    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
std::pair<float, float> ERTelescopeUnpack::
ApplyCalibration(ERChannel channel, Signal amp, Signal time, 
                 TMatrixD* ampCalTable, TMatrixD* timeCalTable) {
  std::pair<float, float> amp_and_time = {static_cast<float>(amp) , static_cast<float>(time)};
  if (ampCalTable) {
    if (channel >= ampCalTable->GetNrows()){
        LOG(FATAL) << "Channel " << channel << " not found in amplitude calibration table of detector " 
                    << detector_name_ << FairLogger::endl;
    }
    amp_and_time.first = amp_and_time.first * static_cast<float>((*ampCalTable)[channel][1]) + static_cast<float>((*ampCalTable)[channel][0]);
  }
  if (timeCalTable) {
    if (channel >= timeCalTable->GetNrows()){
        LOG(FATAL) << "Channel " << channel << " not found in time calibration table of detector " 
                    << detector_name_ << FairLogger::endl;
    }
    amp_and_time.second = amp_and_time.second * static_cast<float>((*timeCalTable)[channel][1]) + static_cast<float>((*timeCalTable)[channel][0]);
  }
  return amp_and_time;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::CheckSetup() {
    const std::map<TString, unsigned short> stationsInConfig = setup_configuration_->GetStationList(detector_name_);
    for (auto itStation : fStations){
        ERTelescopeStation* station = itStation.second;
        if (stationsInConfig.find(station->ampStName) == stationsInConfig.end()){
            LOG(FATAL) << "Amplitude station " << station->ampStName <<
                          " of telescope " << detector_name_ << 
                          " not found in setup configuration file" << FairLogger::endl;
            return kFALSE;
        }
        if (station->timeStName != ""){
            if (stationsInConfig.find(station->timeStName) == stationsInConfig.end()){
                LOG(FATAL) << "Time station " << station->timeStName <<
                            " of telescope " << detector_name_ << 
                            " not found in setup configuration file" << FairLogger::endl;
                return kFALSE;
            }
        }
        if (station->sideCount == 2){
            if (stationsInConfig.find(station->ampStName2) == stationsInConfig.end()){
            LOG(FATAL) << "Amplitude station " << station->ampStName2 <<
                            " of telescope " << detector_name_ << 
                            " not found in setup configuration file" << FairLogger::endl;
            return kFALSE;
            }
            if (station->timeStName2 != ""){
                if (stationsInConfig.find(station->timeStName2) == stationsInConfig.end()){
                    LOG(FATAL) << "Time station " << station->timeStName2 <<
                            " of telescope " << detector_name_ << 
                            " not found in setup configuration file" << FairLogger::endl;
                    return kFALSE;
                }
            }
        }
    }
    for (auto itConfSt : stationsInConfig){
        TString stationName = itConfSt.first;
        Bool_t found = kFALSE;
        for (auto itStation : fStations){
            ERTelescopeStation* station = itStation.second;
            if (station->ampStName == stationName ||
                station->timeStName == stationName || 
                station->ampStName2 == stationName || 
                station->timeStName2 == stationName)
            LOG(WARNING) << "Station " << stationName << " in setup file, but not defined to unpack!" << FairLogger::FairLogger::endl; 
        }
    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeUnpack)