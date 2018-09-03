#include "ERBeamDetUnpack.h"

#include <iostream>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERBeamDetTOFDigi.h"
#include "ERBeamDetMWPCDigi.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetUnpack::ERBeamDetUnpack(TString detName):
 ERUnpack(detName),
 fToFCalConst(0.125)
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
			fDigiCollections["BeamDetToFDigi1"] = new TClonesArray("ERBeamDetTOFDigi",1000);
			ioman->Register("BeamDetToFDigi1", "BeamDet", fDigiCollections["BeamDetToFDigi1"], kTRUE);
		}
		if (itSt.first == TString("F5")){
			fDigiCollections["BeamDetToFDigi2"] = new TClonesArray("ERBeamDetTOFDigi",1000);
			ioman->Register("BeamDetToFDigi2", "BeamDet", fDigiCollections["BeamDetToFDigi2"], kTRUE);
		}
		for (auto itMwpcStation : fMwpcAmpTimeStations){
			TString bName = fMwpcBnames[itMwpcStation.first];
			if (itSt.first == itMwpcStation.first){
				fDigiCollections[bName] = new TClonesArray("ERBeamDetMWPCDigi",1000);
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
		std::vector<Double_t> ampV, timeV;
		std::vector<Int_t> channelV;
		UnpackAmpTimeStation(detEvent, "F3","tF3",ampV, timeV, channelV);
		if (channelV.size() == 4){
			Double_t time = (timeV[0] + timeV[1] + timeV[2] + timeV[3])*0.25*fToFCalConst;
			Double_t amp  = (ampV[0] +  ampV[1] +  ampV[2]  + ampV[3]);
			AddToFDigi(amp,time,1);
		}
		else
			cerr << "Wrong PMT number in ToF!" << endl;
	}
	if (stList.find("F5") != stList.end() && stList.find("tF5") != stList.end()){
		std::vector<Double_t> ampV, timeV;
		std::vector<Int_t> channelV;
		UnpackAmpTimeStation(detEvent, "F5","tF5",ampV, timeV, channelV);
		if (channelV.size() == 4){
			Double_t time = (timeV[0] + timeV[1] + timeV[2] + timeV[3])*0.25*fToFCalConst;
			Double_t amp  = (ampV[0] +  ampV[1] +  ampV[2]  + ampV[3]);
			AddToFDigi(amp,time,2);
		}
		else
			cerr << "Wrong PMT number in ToF!" << endl;
	}
	// MWPC
	std::map<Int_t, Double_t> mwpcTime;
	if (stList.find("tMWPC") != stList.end()){
		UnpackStation(detEvent,"tMWPC",mwpcTime);
	}

	for (auto itMwpcStation : fMwpcAmpTimeStations){
		TString mwpcAmpSt = itMwpcStation.first;
		Int_t mwpcTimeSt = itMwpcStation.second;
		if (stList.find(mwpcAmpSt) != stList.end()){
			std::map<Int_t, Double_t> mwpcAmp;
			UnpackStation(detEvent, mwpcAmpSt, mwpcAmp);
			if (mwpcTime.find(mwpcTimeSt) != mwpcTime.end()){
				for (auto itChanel : mwpcAmp){
					AddMWPCDigi(itChanel.second, mwpcTime[mwpcTimeSt], mwpcAmpSt, itChanel.first);
				}
			}
			else{
				cerr << "Could not find mwpc time station " << mwpcTimeSt << endl;
				for (auto itChanel : mwpcAmp){
					AddMWPCDigi(itChanel.second,0., mwpcAmpSt, itChanel.first);
				}
			}
		}
	}
	
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddToFDigi(Float_t edep, Double_t time, Int_t tofNb) {
  ERBeamDetTOFDigi *digi; 
  if(tofNb == 1) {
    digi = new((*fDigiCollections["BeamDetToFDigi1"])[fDigiCollections["BeamDetToFDigi1"]->GetEntriesFast()])
                ERBeamDetTOFDigi(fDigiCollections["BeamDetToFDigi1"]->GetEntriesFast(), edep, time, tofNb);
  }
  if(tofNb == 2) {
    digi = new((*fDigiCollections["BeamDetToFDigi2"])[fDigiCollections["BeamDetToFDigi2"]->GetEntriesFast()])
                ERBeamDetTOFDigi(fDigiCollections["BeamDetToFDigi2"]->GetEntriesFast(), edep, time, tofNb);
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::AddMWPCDigi(Float_t edep, Double_t time, 
                                            TString mwpcSt, Int_t wireNb) {
  ERBeamDetMWPCDigi *digi;
  TString bName = fMwpcBnames[mwpcSt];
  digi = new((*fDigiCollections[bName])[fDigiCollections[bName]->GetEntriesFast()])
              ERBeamDetMWPCDigi(fDigiCollections[bName]->GetEntriesFast(), edep, time, 
                                -1, -1, wireNb);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetUnpack)