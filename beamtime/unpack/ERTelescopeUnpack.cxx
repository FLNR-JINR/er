#include "ERTelescopeUnpack.h"

#include <iostream>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::ERTelescopeUnpack(TString detName):
ERUnpack(detName)
{

}
//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::~ERTelescopeUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::Init(SetupConfiguration* setupConf){
	ERUnpack::Init(setupConf);

	if (fInited)
		return kTRUE;
	else
		fInited = kTRUE;


	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");

	fSetupConfiguration = setupConf;

	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	for (auto itSt : stList){
		cerr <<  itSt.first << endl;
		if (itSt.first == TString("SQX_R")){
			fDigiCollections["ERQTelescopeSiDigi_Right_telescope_DoubleSi_SD2_XY_1_X"] = new TClonesArray("ERQTelescopeSiDigi",1000);
			ioman->Register("ERQTelescopeSiDigi_Right_telescope_DoubleSi_SD2_XY_1_X",
				 "Right_telescope", fDigiCollections["ERQTelescopeSiDigi_Right_telescope_DoubleSi_SD2_XY_1_X"], kTRUE);
		}
	}

	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::DoUnpack(Int_t* data, Int_t size){
	if (fUnpacked)
		return kTRUE;
	else
		fUnpacked = kTRUE;

	DetEventFull* event = (DetEventFull*)data;

	DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	if (stList.find("SQX_R") != stList.end() &&  stList.find("tSQX_R") != stList.end())
		UnpackSiStation(detEvent,"SQX_R","tSQX_R");

	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::UnpackSiStation(DetEventDetector* detEvent, TString ampStation, TString timeStation){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	Double_t time = 0.,amp = 0.;
	Int_t channel = -1;
	TString ampEventElement,timeEventElement;
	ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
	timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());

	DetEventStation* ampStationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
	DetEventStation* timeStationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
	if (!ampStationEvent || timeStationEvent)
		cerr << "Amplitude event element or time event element not found for " << ampStation << endl;

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
			TString digiBranchName = "";
			if (ampStation == "SQX_R")
				digiBranchName = "ERQTelescopeSiDigi_Right_telescope_DoubleSi_SD2_XY_1_X";	
			AddSiDigi(amp,time,0,channel,digiBranchName);
		}
		else
			cerr << "Time channel for amplitude channel not found" << endl;
	}
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, 
                                                                                  Int_t stripNb,
                                                                                  TString digiBranchName)
{
  ERQTelescopeSiDigi *digi = new((*fDigiCollections[digiBranchName])
                                                  [fDigiCollections[digiBranchName]->GetEntriesFast()])
              ERQTelescopeSiDigi(fDigiCollections[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                     stationNb, 
                                                                                     stripNb);
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::Reset(){
	fUnpacked = kFALSE;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeUnpack)