
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
	//input files opening
	if (fPath.size() == 0)
		Fatal("ERDigibuilder", "No files for source ERDigibuilder");

	if (fSetupFile == "")
		Fatal("ERDigibuilder", "No SetupFile for ERDigibuilder");

	FairRun* run = FairRun::Instance();

	OpenNextFile();
	fSetupConfiguration = new SetupConfiguration(fSetupFile);

	return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::InitUnpackers(){
	const std::map<TString, unsigned short> detList = fSetupConfiguration->GetDetectorList();
	for (auto itDet : detList){
		if (itDet.first == TString("Beam_detector_ToF") || itDet.first == TString("Beam_detector_MWPC"))
			if (fUnpacks.find("BeamDet") != fUnpacks.end())
				fUnpacks["BeamDet"]->Init(fSetupConfiguration);
			else
				cerr << "Beam_detector_ToF is defined in setup file, but unpacker is not added!" << endl;
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

	if (event->GetChild("Beam_detector_ToF") || event->GetChild("Beam_detector_MWPC"))
		if (fUnpacks.find("BeamDet") != fUnpacks.end())
			fUnpacks["BeamDet"]->DoUnpack((Int_t*)event,0);

	return 0;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Close(){
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Reset(){

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
