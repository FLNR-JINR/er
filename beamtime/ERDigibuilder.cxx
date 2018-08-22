
#include "ERDigibuilder.h"

#include <iostream>

#include "FairRootManager.h"
#include "FairRun.h"

#include "DetEventFull.h"

using namespace std;

ERDigibuilder::ERDigibuilder():
fCurFile(0),
fOldEvents(0),
fSetupFile(""),
fReader(NULL)
{
}

ERDigibuilder::ERDigibuilder(const ERDigibuilder& source){
}

ERDigibuilder::~ERDigibuilder(){

}

Bool_t ERDigibuilder::Init(){
	//input files opening
	if (fPath.size() == 0)
		Fatal("ERDigibuilder", "No files for source ERDigibuilder");

	if (fSetupFile == "")
		Fatal("ERDigibuilder", "No SetupFile for ERDigibuilder");

	FairRun* run = FairRun::Instance();

	OpenNextFile();

	return kTRUE;
}

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
	
	return 0;
}

void ERDigibuilder::Close(){
}

void ERDigibuilder::Reset(){
}

Int_t ERDigibuilder::OpenNextFile(){
	if (fCurFile == fPath.size())
		return 1;
	fReader = new Reader(fPath[fCurFile++],fSetupFile);
	return 0;
}

ClassImp(ERDigibuilder)
