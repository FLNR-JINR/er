
#include "ERDigibuilder.h"

#include <iostream>

#include "FairRootManager.h"
#include "FairRun.h"

#include "DetEventFull.h"
#include "DetEventDetector.h"
#include "DetEventStation.h"
#include "DetMessage.h"


#include "TGo4EventElement.h"
#include "TObjArray.h"
#include "TClonesArray.h"

#include "ERBeamDetTOFDigi.h"
#include "ERBeamDetMWPCDigi.h"

using namespace std;

ERDigibuilder::ERDigibuilder():
fCurFile(0),
fOldEvents(0),
fSetupFile(""),
fReader(NULL),
fSetupConfiguration(NULL)
{
}
//--------------------------------------------------------------------------------------------------
ERDigibuilder::ERDigibuilder(const ERDigibuilder& source){
}
//--------------------------------------------------------------------------------------------------
ERDigibuilder::~ERDigibuilder(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::Init(){
	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");
	//input files opening
	if (fPath.size() == 0)
		Fatal("ERDigibuilder", "No files for source ERDigibuilder");

	if (fSetupFile == "")
		Fatal("ERDigibuilder", "No SetupFile for ERDigibuilder");

	FairRun* run = FairRun::Instance();

	OpenNextFile();
	fSetupConfiguration = new SetupConfiguration(fSetupFile);

	const std::map<TString, unsigned short> detList = fSetupConfiguration->GetDetectorList();
	for (auto itDet : detList){
		const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(itDet.first);
		for (auto itSt : stList){
			cerr << itDet.first << " " <<  itSt.first << endl;
			if (itDet.first == TString("Beam_detector_ToF") && itSt.first == TString("F3")){
				fDigiCollections["BeamDetToFDigi1"] = new TClonesArray("ERBeamDetTOFDigi",1000);
				ioman->Register("BeamDetToFDigi1", "BeamDet", fDigiCollections["BeamDetToFDigi1"], kTRUE);
			}
			if (itDet.first == TString("Beam_detector_ToF") && itSt.first == TString("F5")){
				fDigiCollections["BeamDetToFDigi2"] = new TClonesArray("ERBeamDetTOFDigi",1000);
				ioman->Register("BeamDetToFDigi2", "BeamDet", fDigiCollections["BeamDetToFDigi2"], kTRUE);
			}
			if (itDet.first == TString("Beam_detector_MWPC") && itSt.first == TString("MWPC1")){
				fDigiCollections["BeamDetMWPCDigiX1"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
				ioman->Register("BeamDetMWPCDigiX1", "BeamDet", fDigiCollections["BeamDetMWPCDigiX1"], kTRUE);
			}
			if (itDet.first == TString("Beam_detector_MWPC") && itSt.first == TString("MWPC2")){
				fDigiCollections["BeamDetMWPCDigiY1"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
				ioman->Register("BeamDetMWPCDigiY1", "BeamDet", fDigiCollections["BeamDetMWPCDigiY1"], kTRUE);
			}
			if (itDet.first == TString("Beam_detector_MWPC") && itSt.first == TString("MWPC3")){
				fDigiCollections["BeamDetMWPCDigiX2"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
				ioman->Register("BeamDetMWPCDigiX2", "BeamDet", fDigiCollections["BeamDetMWPCDigiX2"], kTRUE);
			}
			if (itDet.first == TString("Beam_detector_MWPC") && itSt.first == TString("MWPC4")){
				fDigiCollections["BeamDetMWPCDigiY2"] = new TClonesArray("ERBeamDetMWPCDigi",1000);
				ioman->Register("BeamDetMWPCDigiY2", "BeamDet", fDigiCollections["BeamDetMWPCDigiY2"], kTRUE);
			}
		}
	}
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::ReadEvent(UInt_t id){
	FairRootManager* ioman = FairRootManager::Instance();
  	if ( ! ioman ) Fatal("Init", "No FairRootManager");

  	Int_t curEventInCurFile = ioman->GetEntryNr()-fOldEvents;

	//Проверяем есть ли еще события для обработки
	if (fReader->GetNEventsTotal() == curEventInCurFile){
		//файл закончился
		fOldEvents += fReader->GetNEventsTotal();
		curEventInCurFile = 0;
		if (OpenNextFile())
			return 1;
	}

	DetEventFull* event = new DetEventFull("DetEventFull1");
	fReader->ReadEvent(curEventInCurFile,event);

	if (event->GetChild("Beam_detector_ToF")){
		UnpackBeamDetTof((DetEventDetector*)event->GetChild("Beam_detector_ToF"));
	}
	if (event->GetChild("Beam_detector_MWPC")){
		UnpackBeamDetMwpc((DetEventDetector*)event->GetChild("Beam_detector_MWPC"));
	}
	
	// Loop over detector
	for (Int_t iDet(2); iDet < event->getNElements(); iDet++){
		DetEventDetector* detEvent = (DetEventDetector*)event->getEventElement(iDet);
		cerr << detEvent->GetName() << endl;
		if (TString(detEvent->GetName()).Contains("DetEventCommon"))
			continue;
		cerr << detEvent << " " <<  detEvent->getNElements() << " " << detEvent->isComposed() << endl;
		// Loop over detector
		for (Int_t iStation(0); iStation < detEvent->getMaxIndex(); iStation++){
			DetEventStation* stationEvent = (DetEventStation*)detEvent->getEventElement(iStation);
			if(stationEvent){
				cerr << "\t" << stationEvent->GetName() << endl;
				TClonesArray* messages = stationEvent->GetDetMessages();
				for (Int_t iMassage(0); iMassage < messages->GetEntriesFast(); ++iMassage){
					DetMessage* mes = (DetMessage*)messages->At(iMassage);
					cerr << "\t\t" << mes->fStChannel << " " << mes->fValue << endl;
				}
			}
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::UnpackBeamDetTof(DetEventDetector* detEvent){
	const std::map<TString, unsigned short> tofStList = fSetupConfiguration->GetStationList("Beam_detector_ToF");
	if (tofStList.find("F3") != tofStList.end() || tofStList.find("tF3") != tofStList.end()){
		UnpackTofStation(detEvent,"F3","tF3");
	}
	if (tofStList.find("F5") != tofStList.end() || tofStList.find("F5") != tofStList.end()){
		UnpackTofStation(detEvent,"F5","tF5");
	}
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::UnpackBeamDetMwpc(DetEventDetector* detEvent){
	const std::map<TString, unsigned short> mwpcStList = fSetupConfiguration->GetStationList("Beam_detector_MWPC");
	if (mwpcStList.find("MWPC1") != mwpcStList.end()){
		UnpackMWPCStation(detEvent,"MWPC1",1,1);
	}
	if (mwpcStList.find("MWPC2") != mwpcStList.end()){
		UnpackMWPCStation(detEvent,"MWPC2",1,2);
	}
	if (mwpcStList.find("MWPC3") != mwpcStList.end()){
		UnpackMWPCStation(detEvent,"MWPC3",2,1);
	}
	if (mwpcStList.find("MWPC4") != mwpcStList.end()){
		UnpackMWPCStation(detEvent,"MWPC4",2,2);
	}
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::UnpackTofStation(DetEventDetector* detEvent, TString ampStation, TString timeStation){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList("Beam_detector_ToF");
	Double_t time = 0.,amp = 0.;
	TString ampEventElement,timeEventElement;
	ampEventElement.Form("Beam_detector_ToF_%s",ampStation.Data());
	timeEventElement.Form("Beam_detector_ToF_%s",timeStation.Data());

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
void ERDigibuilder::UnpackMWPCStation(DetEventDetector* detEvent, TString ampStation, Int_t mwpcNb, Int_t planeNb){
	const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList("Beam_detector_MWPC");
	TString ampEventElement;
	ampEventElement.Form("Beam_detector_MWPC_%s",ampStation.Data());
	DetEventStation* stationEvent = (DetEventStation*)detEvent->GetChild(ampEventElement);
	cerr << ampEventElement << endl;
	if (stationEvent){
		TClonesArray* messages = stationEvent->GetDetMessages();
		for (Int_t iMassage(0); iMassage < messages->GetEntriesFast(); ++iMassage){
			DetMessage* mes = (DetMessage*)messages->At(iMassage);
			cerr << mes->fValue << " " << mes->fStChannel << endl;
			AddMWPCDigi(mes->fValue, 0., mwpcNb, planeNb, mes->fStChannel);
		}
	}
	else{
		cerr << "Could not find event element for " << ampEventElement << endl;
	}
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Close(){
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Reset(){
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::OpenNextFile(){
	if (fCurFile == fPath.size())
		return 1;
	fReader = new Reader(fPath[fCurFile++],fSetupFile);
	return 0;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::AddToFDigi(Float_t edep, Double_t time, Int_t tofNb) {
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
void ERDigibuilder::AddMWPCDigi(Float_t edep, Double_t time, 
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
ClassImp(ERDigibuilder)
