#include "ERBeamDetUnpack.h"

#include <iostream>

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERDigi.h"
#include "ERDigi.h"
#include "ERSupport.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetUnpack::ERBeamDetUnpack(TString detName):
 ERUnpack(detName),
 fCalToFa1(0.125),
 fCalToFb1(0.),
 fCalToFa2(0.125),
 fCalToFb2(0.),
 fCalMWPCa(0.125),
 fCalMWPCb(0.)
{
}
//--------------------------------------------------------------------------------------------------
ERBeamDetUnpack::~ERBeamDetUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERBeamDetUnpack::Init(SetupConfiguration* setupConf){
	if (!ERUnpack::Init(setupConf))
		return kTRUE;
	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");
	fSetupConfiguration = setupConf;
	//@TODO check setup
	fMwpcAmpTimeStations["MWPC1"] = 0;
	fMwpcAmpTimeStations["MWPC2"] = 1;
	fMwpcAmpTimeStations["MWPC3"] = 2;
	fMwpcAmpTimeStations["MWPC4"] = 3;
	fMwpcBnames["MWPC1"] = "BeamDetMWPCDigiX1";
	fMwpcBnames["MWPC2"] = "BeamDetMWPCDigiY1";
	fMwpcBnames["MWPC3"] = "BeamDetMWPCDigiX2";
	fMwpcBnames["MWPC4"] = "BeamDetMWPCDigiY2";
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	for (auto itSt : stList){
		if (itSt.first == TString("F3")){
			fDigiCollections["BeamDetToFDigi1"] = new TClonesArray("ERDigi",
                                                             consts::approx_beamdet_tof_digi_number);
			ioman->Register("BeamDetToFDigi1", "BeamDet", fDigiCollections["BeamDetToFDigi1"], kTRUE);
		}
		if (itSt.first == TString("F5")){
			fDigiCollections["BeamDetToFDigi2"] = new TClonesArray("ERDigi",
                                                             consts::approx_beamdet_tof_digi_number);
			ioman->Register("BeamDetToFDigi2", "BeamDet", fDigiCollections["BeamDetToFDigi2"], kTRUE);
		}
		for (auto itMwpcStation : fMwpcAmpTimeStations){
			TString bName = fMwpcBnames[itMwpcStation.first];
			if (itSt.first == itMwpcStation.first){
				fDigiCollections[bName] = new TClonesArray("ERDigi",consts::approx_beamdet_mwpc_digi_number);
				ioman->Register(bName, "BeamDet", fDigiCollections[bName], kTRUE);
			}
		}
	}
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERBeamDetUnpack::DoUnpack(Int_t* data, Int_t size){
	if (!ERUnpack::DoUnpack(data,size))
		return kTRUE;
	DetEventFull* event = (DetEventFull*)data;
	DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	//ToF
	if (stList.find("F3") != stList.end() && stList.find("tF3") != stList.end()){
		Channel2AmplitudeTime valueMap;
		UnpackAmpTimeStation(detEvent, "F3","tF3",valueMap);
		if (valueMap.size() == 4){
			Double_t time = 0., amp = 0.;
			for (const auto itValue : valueMap){
				amp += itValue.second.first;
				time += itValue.second.second;
			}
			time = time * 0.25 * fCalToFa1 + fCalToFb1;
			AddToFDigi(amp, time, 1);
		} else {
			LOG(DEBUG) << "Wrong PMT number in ToF!" << FairLogger::endl;
		}
	}
	if (stList.find("F5") != stList.end() && stList.find("tF5") != stList.end()){
		Channel2AmplitudeTime valueMap;
		UnpackAmpTimeStation(detEvent, "F5","tF5",valueMap);
		if (valueMap.size() == 4){
			float time = 0., amp = 0.;
			for (const auto itValue : valueMap){
				amp += itValue.second.first;
				time += itValue.second.second;
			}
			time = time * 0.25 * fCalToFa2 + fCalToFb2;
			AddToFDigi(amp, time, 2);
		} else {
			LOG(DEBUG) << "Wrong PMT number in ToF!" << FairLogger::endl;
		}
	}
	// MWPC
	Channel2Value mwpcTime;
	if (stList.find("tMWPC") != stList.end()){
		UnpackStation(detEvent,"tMWPC",mwpcTime);
	} 
	for (auto itMwpcStation : fMwpcAmpTimeStations){
		TString mwpcAmpSt = itMwpcStation.first;
		Int_t mwpcTimeSt = itMwpcStation.second;
		if (stList.find(mwpcAmpSt) != stList.end()){
			Channel2Value mwpcAmp;
			UnpackStation(detEvent, mwpcAmpSt, mwpcAmp);
			if (mwpcTime.find(mwpcTimeSt) != mwpcTime.end()){
				for (const auto itChanel : mwpcAmp){
					const float time = mwpcTime[mwpcTimeSt] * fCalMWPCa + fCalMWPCb;
					AddMWPCDigi(itChanel.second, time, mwpcAmpSt,
								GetChannelNumber(itChanel.first, fMwpcChannelsMapping));
				}
			} else {
				LOG(DEBUG) << "MWPC time signal not found for amplitude" << FairLogger::endl;
			}
		}
	}
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddToFDigi(float edep, float time, Int_t tofNb) {
  ERDigi *digi; 
  if(tofNb == 1) {
    digi = new((*fDigiCollections["BeamDetToFDigi1"])[fDigiCollections["BeamDetToFDigi1"]->GetEntriesFast()])
        ERDigi(edep, time, 0);
  }
  if(tofNb == 2) {
    digi = new((*fDigiCollections["BeamDetToFDigi2"])[fDigiCollections["BeamDetToFDigi2"]->GetEntriesFast()])
    	ERDigi(edep, time, 0);
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddMWPCDigi(float edep, float time, TString mwpcSt, Int_t wireNb) {
  TString bName = fMwpcBnames[mwpcSt];
  new((*fDigiCollections[bName])[fDigiCollections[bName]->GetEntriesFast()])
              ERDigi(edep, time, wireNb+1);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetUnpack)
