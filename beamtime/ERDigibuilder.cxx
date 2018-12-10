#include "ERDigibuilder.h"

#include <iostream>

#include "TObjArray.h"
#include "TTree.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairLogger.h"

#include "DetEventFull.h"
#include "DetEventDetector.h"
#include "DetEventStation.h"
#include "DetMessage.h"
#include "DetEventCommon.h"
#include "TGo4EventElement.h"


#include "ERBeamTimeEventHeader.h"

using namespace std;

ERDigibuilder::ERDigibuilder():
fCurFile(0),
fOldEvents(0),
fSetupFile(""),
fReader(NULL),
fSetupConfiguration(NULL),
fUserCut(""),
fFillSkippedEvents(kTRUE),
fEventsForProcessing(NULL)
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

    FairRun* run = FairRun::Instance();

    if (OpenNextFile() == -1)
        return kFALSE;

    fSetupConfiguration = new SetupConfiguration(fSetupFile);

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
            LOG(WARNING) << itDet.first << " is defined in setup file, but unpacker is not added!" << FairLogger::endl;
    }

    for (auto itUnpack : fUnpacks){
        if (!itUnpack.second->IsInited())
            LOG(WARNING) << "Detector " << itUnpack.first << " not found in setup file. Unpacker has not inited!" << FairLogger::endl;
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

    FairRun* run = FairRun::Instance();
    ERBeamTimeEventHeader* header = (ERBeamTimeEventHeader*) run->GetEventHeader();

    if (fUserCut != "") {
        if (!fEventsForProcessing->GetBinContent(curEventInCurFile)){
            LOG(DEBUG) << "  Skip event with user cut" << FairLogger::endl;
            header->SetTrigger(-1);
            if (!fFillSkippedEvents)
                run->MarkFill(kFALSE);
            return 0;
        }
    }

    DetEventFull* event = fReader->ReadEvent(curEventInCurFile);
    
    DetEventCommon* common  = (DetEventCommon*)event->GetChild("DetEventCommon");
    if (!common){
        LOG(FATAL) << "DetEventCommon event element not found!" << FairLogger::endl;
        return 1;
    }

    header->SetTrigger(common->trigger);

    for (auto itUnpack : fUnpacks){
        if (itUnpack.second->IsInited()){
            if (event->GetChild(itUnpack.first)){
                DumpRawToScreen((DetEventDetector*)event->GetChild(itUnpack.first));
                itUnpack.second->DoUnpack((Int_t*)event,0);
            }
            else
                LOG(WARNING) << "Event element for detector " << itUnpack.first 
                                    << " not found in event!" << FairLogger::endl;
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
    if (fEventsForProcessing)
        delete fEventsForProcessing;
    if (fCurFile == fPath.size())
        return 1;
    fReader = new Reader(fPath[fCurFile++],fSetupFile);
    
    if (fUserCut != ""){
        TTree* tr = fReader->GetInTree();
        fEventsForProcessing =  new TH1I ("hist", "Events for processing", tr->GetEntries(), 1, tr->GetEntries());
        Int_t res = 0;
        res = tr->Draw("Entry$>>hist",fUserCut,"goff");
        if (res == -1){
            LOG(FATAL) << "Error in user cut expression" << endl;
            return -1;
        }
        if (!fEventsForProcessing->GetEntries()) {
          LOG(DEBUG) << "ERDigibuilder: No data for analysis with defined user cut: "
                    << fUserCut << FairLogger::endl;
          OpenNextFile();
        }
    }
    
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::DumpRawToScreen(DetEventDetector* det){
    LOG(DEBUG) << "Dump raw of " << det->GetName() << FairLogger::endl;
    for (Int_t iSt(0); iSt<det->getMaxIndex(); iSt++){
        if (det->getEventElement(iSt)){
            DetEventStation* st = (DetEventStation*)det->getEventElement(iSt);
            LOG(DEBUG) << "\t" <<  st->GetName() << FairLogger::endl;
            TClonesArray* timeMessages = st->GetDetMessages();
            for (Int_t iTimeMessage(0); iTimeMessage < timeMessages->GetEntriesFast(); ++iTimeMessage){
                DetMessage* curTimeMes = (DetMessage*)timeMessages->At(iTimeMessage);
                LOG(DEBUG) << "\t\t" << curTimeMes->GetStChannel() << " " <<  curTimeMes->GetValue() << FairLogger::endl;
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::SetUserCut(TCut cut,Bool_t fillSkippedEvents/*=kTRUE*/){
    fUserCut = cut;
    fFillSkippedEvents=fillSkippedEvents;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDigibuilder)
