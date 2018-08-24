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
ERUnpack(detName)
{

}
//--------------------------------------------------------------------------------------------------
ERBeamDetUnpack::~ERBeamDetUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERBeamDetUnpack::Init(SetupConfiguration* setupConf){
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
		if (itSt.first == TString("F3")){
			fDigiCollections["BeamDetToFDigi1"] = new TClonesArray("ERBeamDetTOFDigi",1000);
			ioman->Register("BeamDetToFDigi1", "BeamDet", fDigiCollections["BeamDetToFDigi1"], kTRUE);
		}
		if (itSt.first == TString("F5")){
			fDigiCollections["BeamDetToFDigi2"] = new TClonesArray("ERBeamDetTOFDigi",1000);
			ioman->Register("BeamDetToFDigi2", "BeamDet", fDigiCollections["BeamDetToFDigi2"], kTRUE);
		}
		if (itSt.first == TString("MWPC1")){
			fDigiCollections["BeamDetMWPCDigiX1"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
			ioman->Register("BeamDetMWPCDigiX1", "BeamDet", fDigiCollections["BeamDetMWPCDigiX1"], kTRUE);
		}
		if (itSt.first == TString("MWPC2")){
			fDigiCollections["BeamDetMWPCDigiY1"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
			ioman->Register("BeamDetMWPCDigiY1", "BeamDet", fDigiCollections["BeamDetMWPCDigiY1"], kTRUE);
		}
		if (itSt.first == TString("MWPC3")){
			fDigiCollections["BeamDetMWPCDigiX2"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
			ioman->Register("BeamDetMWPCDigiX2", "BeamDet", fDigiCollections["BeamDetMWPCDigiX2"], kTRUE);
		}
		if (itSt.first == TString("MWPC4")){
			fDigiCollections["BeamDetMWPCDigiY2"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
			ioman->Register("BeamDetMWPCDigiY2", "BeamDet", fDigiCollections["BeamDetMWPCDigiY2"], kTRUE);
		}
	}

	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERBeamDetUnpack::DoUnpack(Int_t* data, Int_t size){
	if (fUnpacked)
		return kTRUE;
	else
		fUnpacked = kTRUE;

	DetEventFull* event = (DetEventFull*)data;

	DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	if (stList.find("F3") != stList.end() || stList.find("tF3") != stList.end()){
		UnpackTofStation(detEvent,"F3","tF3");
	}
	if (stList.find("F5") != stList.end() || stList.find("F5") != stList.end()){
		UnpackTofStation(detEvent,"F5","tF5");
	}
	if (stList.find("MWPC1") != stList.end()){
		UnpackMWPCStation(detEvent,"MWPC1",1,1);
	}
	if (stList.find("MWPC2") != stList.end()){
		UnpackMWPCStation(detEvent,"MWPC2",1,2);
	}
	if (stList.find("MWPC3") != stList.end()){
		UnpackMWPCStation(detEvent,"MWPC3",2,1);
	}
	if (stList.find("MWPC4") != stList.end()){
		UnpackMWPCStation(detEvent,"MWPC4",2,2);
	}
	
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::Reset(){
	fUnpacked = kFALSE;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::UnpackTofStation(DetEventDetector* detEvent, TString ampStation, TString timeStation){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	Double_t time = 0.,amp = 0.;
	TString ampEventElement,timeEventElement;
	ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
	timeEventElement.Form("%s_%s",fDetName.Data(),timeStation.Data());

	if (stList.find(ampStation) != stList.end()){
		DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
		if (stationEvent){
			TClonesArray* messages = stationEvent->GetDetMessages();
			DetMessage* mes = (DetMessage*)messages->At(0);
			amp = mes->fValue;
		}
		else{
			cerr << "Could not find event element for " << ampEventElement << endl;
		}
	}
	if (stList.find(timeStation) != stList.end()){
		DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(timeEventElement);
		if (stationEvent){
			TClonesArray* messages = stationEvent->GetDetMessages();
			DetMessage* mes = (DetMessage*)messages->At(0);
			time = mes->fValue;
		}
		else{
			cerr << "Could not find event element for " << timeEventElement << endl;
		}
	}
	if (ampStation == TString("F3"))
		AddToFDigi(amp,time,1); //TODO выпилить адресацию из TOF digi
	if (ampStation == TString("F5"))
		AddToFDigi(amp,time,2);
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetUnpack::UnpackMWPCStation(DetEventDetector* detEvent, TString ampStation, Int_t mwpcNb, Int_t planeNb){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
	TString ampEventElement;
	ampEventElement.Form("%s_%s",fDetName.Data(),ampStation.Data());
	DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
	if (stationEvent){
		TClonesArray* messages = stationEvent->GetDetMessages();
		for (Int_t iMassage(0); iMassage < messages->GetEntriesFast(); ++iMassage){
			DetMessage* mes = (DetMessage*)messages->At(iMassage);
			AddMWPCDigi(mes->fValue, 0., mwpcNb, planeNb, mes->fStChannel);
		}
	}
	else{
		cerr << "Could not find event element for " << ampEventElement << endl;
	}
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
                                            Int_t mwpcNb, Int_t planeNb, Int_t wireNb) {
  ERBeamDetMWPCDigi *digi;
  if(mwpcNb == 1) {
    if(planeNb == 1) {
      digi = new((*fDigiCollections["BeamDetMWPCDigiX1"])[fDigiCollections["BeamDetMWPCDigiX1"]->GetEntriesFast()])
              ERBeamDetMWPCDigi(fDigiCollections["BeamDetMWPCDigiX1"]->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fDigiCollections["BeamDetMWPCDigiY1"])[fDigiCollections["BeamDetMWPCDigiY1"]->GetEntriesFast()])
              ERBeamDetMWPCDigi(fDigiCollections["BeamDetMWPCDigiY1"]->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    }
  }
  if(mwpcNb == 2) {
    if(planeNb == 1) {
      digi = new((*fDigiCollections["BeamDetMWPCDigiX2"])[fDigiCollections["BeamDetMWPCDigiX2"]->GetEntriesFast()])
              ERBeamDetMWPCDigi(fDigiCollections["BeamDetMWPCDigiX2"]->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fDigiCollections["BeamDetMWPCDigiY2"])[fDigiCollections["BeamDetMWPCDigiY2"]->GetEntriesFast()])
              ERBeamDetMWPCDigi(fDigiCollections["BeamDetMWPCDigiY2"]->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetUnpack)