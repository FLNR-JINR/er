#include "ERUnpack.h"

#include <iostream>

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
                        			std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	Double_t time = 0.,amp = 0.;
	Int_t channel = -1;
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

	for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
		DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
		amp = ampMes->GetValue();
		channel = ampMes->GetStChannel();
		Bool_t found = kFALSE;
		DetMessage* timeMes = NULL;
		//finding corresponding time message by channel
		for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
			DetMessage* curTimeMes = (DetMessage*)timeMessages->At(iTimeMessage);
			if (curTimeMes->GetStChannel() == channel){
				found = kTRUE;
				timeMes = curTimeMes;
				time = timeMes->GetValue();
			}
		}
		if (found){
			valueMap[channel] = std::make_pair(amp,time);
		}
		else
			LOG(DEBUG) << "Time channel for amplitude channel not found" << FairLogger::endl;
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

ClassImp(ERUnpack)