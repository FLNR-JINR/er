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

	fBnames["SQX_R"] = FormBranchName("Si",2, "SQ_R", "XY","X");
	fBnames["SQY_R"] = FormBranchName("Si",2, "SQ_R", "XY","Y");
	fBnames["SQX_L"] = FormBranchName("Si",2, "SQ_L", "XY","X");
	fBnames["SQY_L"] = FormBranchName("Si",2, "SQ_L", "XY","Y");

	fSiAmpTimeStations["SQX_R"] = "tSQX_R";
	fSiAmpTimeStations["SQY_R"] = "tSQY_R";
	fSiAmpTimeStations["SQX_L"] = "tSQX_L";
	fSiAmpTimeStations["SQY_L"] = "tSQY_L";

	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	for (auto itSt : stList){
		cerr <<  itSt.first << endl;
		if (fSiAmpTimeStations.find(itSt.first) != fSiAmpTimeStations.end()){
			fDigiCollections[fBnames[itSt.first]] = new TClonesArray("ERQTelescopeSiDigi",1000);
			ioman->Register(fBnames[itSt.first],fDetName, fDigiCollections[fBnames[itSt.first]], kTRUE);
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
	for (auto itSt : stList){
		if (fSiAmpTimeStations.find(itSt.first) != fSiAmpTimeStations.end()){
			UnpackSiStation(detEvent,itSt.first,fSiAmpTimeStations[itSt.first]);
		}
	}

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
			AddSiDigi(amp,time,0,channel,fBnames[ampStation]);
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
TString ERTelescopeUnpack::FormBranchName(TString type, Int_t sideCount, TString stName, TString XY, TString XYside){
	TString bName;
	if (type == TString("Si"))
		if (sideCount == 1)
			bName.Form("ERQTelescopeSiDigi_%s_SingleSi_%s_%s_0",fDetName.Data(),stName.Data(),XYside.Data());
		else
			bName.Form("ERQTelescopeSiDigi_%s_DoubleSi_%s_%s_0_%s",fDetName.Data(),stName.Data(),XY.Data(),XYside.Data());
	return bName;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeUnpack)