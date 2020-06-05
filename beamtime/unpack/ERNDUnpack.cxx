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
                       std::map<Int_t, Int_t>* channelsMapping /*=nullptr*/,
                       Bool_t skipAloneChannels/*= kTRUE*/) : 
    ERUnpack(detName),
    fAmpStation(ampStation),
    fTimeStation(timeStation),
    fTACStation(tacStation),
    fChannelsMapping(channelsMapping),
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
ERNDUnpack::~ERNDUnpack()
{

}
//--------------------------------------------------------------------------------------------------
Bool_t ERNDUnpack::Init(SetupConfiguration* setupConf){
    if (!ERUnpack::Init(setupConf))
        return kTRUE;
    FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");
    fDigiCollections["NDDigis"] = new TClonesArray("ERNDDigi", 1000);
	ioman->Register("NDDigi", "ND", fDigiCollections["NDDigis"], kTRUE /* save to file */);
    if (!CheckSetup())
        Fatal("Init", "Error in ERNDUnpack setup checking !");
}
//--------------------------------------------------------------------------------------------------
Bool_t ERNDUnpack::DoUnpack(Int_t* data, Int_t size){
    if (!ERUnpack::DoUnpack(data,size))
        return kTRUE;
    DetEventFull* event = (DetEventFull*)data;
    DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
    std::map<Int_t, std::tuple<Double_t, Double_t, Double_t>> valueMap;
    UnpackAmpTimeTACStation(detEvent, fAmpStation, fTimeStation, fTACStation,
                            valueMap, fSkipAloneChannels);
    for (auto itValue : valueMap){
        Int_t channel = itValue.first;
        Double_t amp, time, tac;
        std::tie(amp, time, tac) = itValue.second;
        ApplyCalibrations(channel, amp /*[MeV]*/, time, tac);
        AddNDDigi(amp,time,tac, GetChannelNumber(channel, fChannelsMapping));
    }
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::ApplyCalibrations(const Int_t channel, Double_t& amp, Double_t& time, Double_t& tac) {
    const auto applyCalibration = [this, channel](Double_t& value, const TMatrixD* table) {
        if (!table)
            return;
        if (channel >= table->GetNrows()){
            LOG(FATAL) << "Channel " << channel << " not found in amplitude calibration table of detector " 
                        << fDetName << FairLogger::endl;
        }
        value = value*(*table)[channel][1] + (*table)[channel][0];
    };
    applyCalibration(amp, fAmpCalTable);
    applyCalibration(time, fTimeCalTable);
    applyCalibration(tac, fTACCalTable);
}
//--------------------------------------------------------------------------------------------------
void ERNDUnpack::AddNDDigi(const Float_t edep, const Double_t time, const Int_t tac, const Int_t channelNb) {
    TVector3 pos, dpos; // fields for simulation info  
    auto* digiCollection = fDigiCollections["NDDigis"];
    new((*digiCollection) [digiCollection->GetEntriesFast()])
        ERNDDigi(pos, dpos, channelNb, edep, -1. /*lightYield*/, time, -1. /*neutronProb*/, tac);
}
//--------------------------------------------------------------------------------------------------
Bool_t ERNDUnpack::CheckSetup() {
    const auto stationsInConfig = fSetupConfiguration->GetStationList(fDetName);
    const auto stationInConfig = [stationsInConfig, this](const TString stationName,
                                                              const TString type) {
        if (stationsInConfig.find(stationName) == stationsInConfig.end()){
            LOG(FATAL) << type << " station " << stationName << " of ND " << fDetName 
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