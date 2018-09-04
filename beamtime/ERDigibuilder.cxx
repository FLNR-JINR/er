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

using namespace std;

ERDigibuilder::ERDigibuilder():
fCurFile(0),
fOldEvents(0),
fSetupFile(""),
fReader(NULL),
fSetupConfiguration(NULL),
fEvent(NULL)
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
	//input files opening
	if (fPath.size() == 0)
		Fatal("ERDigibuilder", "No files for source ERDigibuilder");

	if (fSetupFile == "")
		Fatal("ERDigibuilder", "No SetupFile for ERDigibuilder");

	FairRun* run = FairRun::Instance();

	OpenNextFile();
	fSetupConfiguration = new SetupConfiguration(fSetupFile);

	fEvent = new DetEventFull("DetEventFull1");

	InitUnpackers();
	
	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::InitUnpackers(){
	const std::map<TString, unsigned short> detList = fSetupConfiguration->GetDetectorList();
	for (auto itDet : detList){
		if (fUnpacks.find(itDet.first) != fUnpacks.end())
			fUnpacks[itDet.first]->Init(fSetupConfiguration);
		else
			cerr << itDet.first << " is defined in setup file, but unpacker is not added!" << endl;
	}

	for (auto itUnpack : fUnpacks){
		if (!itUnpack.second->IsInited())
			cerr << "Detector " << itUnpack.first << " not found in setup file. Unpacker has not inited!" << endl;
	}

	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::ReadEvent(UInt_t id){
	Reset();
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

	fReader->ReadEvent(curEventInCurFile,fEvent);

	for (auto itUnpack : fUnpacks){
		if (itUnpack.second->IsInited()){
			if (fEvent->GetChild(itUnpack.first)){
				itUnpack.second->DoUnpack((Int_t*)fEvent,0);

				//
				DetEventDetector* det= (DetEventDetector*)fEvent->GetChild(itUnpack.first);
				cerr << "! " << det->GetName() << endl;
				for (Int_t iSt(0); iSt<det->getMaxIndex(); iSt++){
					if (det->getEventElement(iSt)){
						DetEventStation* st = (DetEventStation*)det->getEventElement(iSt);
						cerr << "! \t" <<  st->GetName() << endl;
						TClonesArray* timeMessages = st->GetDetMessages();
						for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
							DetMessage* curTimeMes = (DetMessage*)timeMessages->At(iTimeMessage);
							cerr << "! \t\t" << curTimeMes->fStChannel << " " <<  curTimeMes->fValue << endl;
						}
					}
				}
				//
			}
			else
				cerr << "Event element for detector " << itUnpack.first << " not found in event!";
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Close(){
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Reset(){
	for (auto itUnpack : fUnpacks)
		itUnpack.second->Reset();
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::OpenNextFile(){
	if (fReader)
		delete fReader;
	if (fCurFile == fPath.size())
		return 1;
	fReader = new Reader(fPath[fCurFile++],fSetupFile);
	return 0;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDigibuilder)
