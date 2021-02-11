#include "ERUnpack.h"

#include <iostream>
#include <vector>


#include "FairLogger.h"

#include "DetEventStation.h"
#include "DetMessage.h"

using namespace std;
//--------------------------------------------------------------------------------------------------
ERUnpack::ERUnpack(TString detName):
FairUnpack(0,0,0,0,0),
fDetName(detName),
fInited(kFALSE),
fUnpacked(kFALSE)
{

}
//--------------------------------------------------------------------------------------------------
ERUnpack::~ERUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::Init(SetupConfiguration* setupConf){
    fSetupConfiguration = setupConf;

    if (fInited)
        return kFALSE;
    else
        fInited = kTRUE;

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERUnpack::DoUnpack(Int_t* data, Int_t size){
    if (fUnpacked)
        return kFALSE;
    else
        fUnpacked = kTRUE;

    Reset();

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::Reset(){
    fUnpacked = kFALSE;
    for (auto itCol : fDigiCollections)
        itCol.second->Delete();
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                    Channel2AmplitudeTime& valueMap, Bool_t skipAloneChannels/* = kTRUE*/) {
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    TString ampEventElement,timeEventElement;
    ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
    timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());
    DetEventStation* ampStationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
    DetEventStation* timeStationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
    if (!ampStationEvent || !timeStationEvent){
        LOG(FATAL) << "Amplitude event element or time event element not found for " << ampStation << FairLogger::endl;
        return;
    }
    TClonesArray* ampMessages = ampStationEvent->GetDetMessages();
    TClonesArray* timeMessages = timeStationEvent->GetDetMessages();
    // just copy TClonesArray to map and vector
    Channel2Value time_map,amp_map;
    vector<ERChannel> amp_channels, time_channels;
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp_map[static_cast<ERChannel>(ampMes->GetStChannel())] = static_cast<float>(ampMes->GetValue());
        amp_channels.push_back(static_cast<ERChannel>(ampMes->GetStChannel()));
    }
    for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
        DetMessage* timeMes = (DetMessage*)timeMessages->At(iTimeMessage);
        time_map[static_cast<ERChannel>(timeMes->GetStChannel())] = static_cast<float>(timeMes->GetValue());
        time_channels.push_back(static_cast<ERChannel>(timeMes->GetStChannel()));
    }
    // sort for intersection and difference algorithm
    sort(time_channels.begin(), time_channels.end());
    sort(amp_channels.begin(), amp_channels.end());
    // found intersection in amplitude and time channels
    vector<ERChannel> intersection_channels;
    set_intersection(time_channels.begin(), time_channels.end(),
                     amp_channels.begin(), amp_channels.end(),
                     back_inserter(intersection_channels));
    // found alone time and amplitude channels
    vector<ERChannel> time_dif_channels;
    vector<ERChannel> amp_dif_channels;
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
                                       TString tacStation, Channel2AmplitudeTimeTac& valueMap,
                                       Bool_t skipAloneChannels/* = kTRUE*/) {
    const auto stList = fSetupConfiguration->GetStationList(fDetName);
    TString ampEventElement,timeEventElement, tacEventElement;
    ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
    timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());
    tacEventElement.Form("%s_%s",fDetName.Data(),tacStation.Data());
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
    Channel2Value time_map,amp_map,tac_map;
    vector<ERChannel> amp_channels, time_channels, tac_channels;
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp_map[static_cast<unsigned short>(ampMes->GetStChannel())] = static_cast<float>(ampMes->GetValue());
        amp_channels.push_back(static_cast<unsigned short>(ampMes->GetStChannel()));
    }
    for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
        DetMessage* timeMes = (DetMessage*)timeMessages->At(iTimeMessage);
        time_map[static_cast<unsigned short>(timeMes->GetStChannel())] = static_cast<float>(timeMes->GetValue());
        time_channels.push_back(static_cast<unsigned short>(timeMes->GetStChannel()));
    }
    for (Int_t iTacMessage(0); iTacMessage < tacMessages->GetEntriesFast(); ++iTacMessage){
        DetMessage* tacMes = (DetMessage*)tacMessages->At(iTacMessage);
        tac_map[static_cast<unsigned short>(tacMes->GetStChannel())] =static_cast<float>(tacMes->GetValue());
        tac_channels.push_back(static_cast<unsigned short>(tacMes->GetStChannel()));
    }
    // sort for intersection and difference algorithm
    sort(time_channels.begin(), time_channels.end());
    sort(amp_channels.begin(), amp_channels.end());
    sort(tac_channels.begin(), tac_channels.end());
    // found intersection in amplitude and time channels
    vector<ERChannel> amp_time_intersection_channels;
    set_intersection(time_channels.begin(), time_channels.end(),
                     amp_channels.begin(), amp_channels.end(),
                     back_inserter(amp_time_intersection_channels));
    // intersect with tac
    vector<ERChannel> intersection_channels;
    set_intersection(amp_time_intersection_channels.begin(), amp_time_intersection_channels.end(),
                     tac_channels.begin(), tac_channels.end(),
                     back_inserter(intersection_channels));
    // found alone time and amplitude channels
    vector<ERChannel> time_dif_channels;
    vector<ERChannel> amp_dif_channels;
    vector<ERChannel> tac_dif_channels;
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
void ERUnpack::UnpackStation(DetEventDetector* detEvent, TString station, Channel2Value& valueMap){
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    float amp = -1.;
    ERChannel channel = consts::undefined_channel;
    TString eventElementName;
    eventElementName.Form("%s_%s",fDetName.Data(),station.Data());
    DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(eventElementName);
    if (!stationEvent){
        LOG(FATAL) << "Event element not found for " << station << FairLogger::endl;
        return;
    }
    TClonesArray* ampMessages = stationEvent->GetDetMessages();
    for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
        DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
        amp = static_cast<float>(ampMes->GetValue());
        channel = static_cast<ERChannel>(ampMes->GetStChannel());
        valueMap[channel] = amp;
    }   
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERUnpack)