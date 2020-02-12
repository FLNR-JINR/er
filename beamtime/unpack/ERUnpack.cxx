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
TMatrixD* ERUnpack::ReadCalFile(TString fileName){
    ifstream in;
    in.open(fileName);
    if (!in.is_open()){
        LOG(FATAL) << "Can`t open calibration file " << fileName << FairLogger::FairLogger::endl;
        return NULL;
    }

    Int_t nRows = -1, nCols = -1;
    in >> nCols;
    in >> nRows;
    if (nCols <= 0 || nRows <= 0){
        LOG(FATAL) << "Can`t read rows or cols from calibration file " << fileName << FairLogger::FairLogger::endl;
        return NULL;
    }

    TMatrixD* calTable = new TMatrixD(nRows,nCols);
    Int_t i = 0;

    while (!in.eof()){
        if (i >= nRows){
            LOG(FATAL) << "Wrong file format in " << fileName << FairLogger::FairLogger::endl;
            return NULL;
        }
        in >> (*calTable)[i][0] >> (*calTable)[i][1];
        i++;
    }

    return calTable;
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                                    std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap,
                                    Bool_t skipAloneChannels/* = kTRUE*/) {
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
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    Double_t amp = 0.;
    Int_t channel = -1;
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
        amp = ampMes->GetValue();
        channel = ampMes->GetStChannel();
        valueMap[channel] = amp;
    }   
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERUnpack)