#include "ERUnpack.h"

#include <iostream>

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
		itCol.second->Clear();
}
//--------------------------------------------------------------------------------------------------
void ERUnpack::UnpackAmpTimeStation(DetEventDetector* detEvent, TString ampStation, TString timeStation,
                        std::vector<Double_t>& ampV, std::vector<Double_t>& timeV,std::vector<Int_t>& channelV){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	Double_t time = 0.,amp = 0.;
	Int_t channel = -1;
	TString ampEventElement,timeEventElement;
	ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
	timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());

	DetEventStation* ampStationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
	DetEventStation* timeStationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
	if (!ampStationEvent || !timeStationEvent){
		cerr << "Amplitude event element or time event element not found for " << ampStation << endl;
		return;
	}

	TClonesArray* ampMessages = ampStationEvent->GetDetMessages();
	TClonesArray* timeMessages = timeStationEvent->GetDetMessages();

	for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
		DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
		amp = ampMes->fValue;
		channel = ampMes->fStChannel;
		Bool_t found = kFALSE;
		DetMessage* timeMes = NULL;
		//finding corresponding time message by channel
		for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
			DetMessage* curTimeMes = (DetMessage*)timeMessages->At(iTimeMessage);
			if (curTimeMes->fStChannel == channel){
				found = kTRUE;
				timeMes = curTimeMes;
			}
		}
		if (found){
			ampV.push_back(amp);
			timeV.push_back(time);
			channelV.push_back(channel);
		}
		else
			cerr << "Time channel for amplitude channel not found" << endl;
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
		cerr << "Event element not found for " << station << endl;
		return;
	}

	TClonesArray* ampMessages = stationEvent->GetDetMessages();

	for (Int_t iAmpMassage(0); iAmpMassage < ampMessages->GetEntriesFast(); ++iAmpMassage){
		DetMessage* ampMes = (DetMessage*)ampMessages->At(iAmpMassage);
		amp = ampMes->fValue;
		channel = ampMes->fStChannel;
		valueMap[channel] = amp;
	}
}

ClassImp(ERUnpack)