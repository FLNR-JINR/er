#include "ERDigibuilder.h"

#include <iostream>

#include "TObjArray.h"
#include "TTree.h"
#include "TTreeCache.h"

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

//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::Init(){
  //input files opening
  if (file_paths_.size() == 0)
    LOG(FATAL) << "No files for source ERDigibuilder" << FairLogger::endl;
  if (OpenNextFile() != 0)
    return kFALSE;
  InitUnpackers();
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::InitUnpackers(){
  const std::map<TString, unsigned short> detectors = setup_configuration_->GetDetectorList();
  for (const auto& detector : detectors) {
    const auto detector_name = detector.first;
    if (unpacks_.find(detector_name) != unpacks_.end()) {
        unpacks_[detector_name]->Init(setup_configuration_);
    } else {
        LOG(WARNING) << "[Digibuilder] " << detector_name << " is defined in setup ",
                      << "configuration, but unpacker is not added!" << FairLogger::endl;
    }
  }
  for (const auto& detector_name_and_unpack : unpacks_) {
    const auto detector_name = detector_name_and_unpack.first;
    const auto unpack = detector_name_and_unpack.second;
    if (!unpack->IsInited()) {
      LOG(WARNING) << "[Digibuilder] Detector " << detector_name << " not found in setup file."
                   << "Unpacker has not inited!" << FairLogger::endl;
    }
  }
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::ReadEvent(UInt_t id){
    Reset();
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman) Fatal("Init", "No FairRootManager");
    if (ioman->GetEntryNr() % 10000 == 0)
        LOG(INFO) << "[Digibuilder] Event " << ioman->GetEntryNr() << FairLogger::endl;
    const uint event_number_in_current_file = ioman->GetEntryNr() 
                                              - events_count_in_already_processed_files_;
    //Проверяем есть ли еще события для обработки
    if (fReader->GetNEventsTotal() == curEventInCurFile) {
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
            LOG(DEBUG) << "[Digibuilder]  Skip event with user cut" << FairLogger::endl;
            header->SetTrigger(-1);
            if (!fFillSkippedEvents)
                run->MarkFill(kFALSE);
            return 0;
        }
    }
    DetEventFull* event = fReader->ReadEvent(curEventInCurFile);
    DetEventCommon* common  = (DetEventCommon*)event->GetChild("DetEventCommon");
    if (!common){
        LOG(FATAL) << "[Digibuilder] DetEventCommon event element not found!" << FairLogger::endl;
        return 1;
    }
    header->SetTrigger(common->trigger);
    for (auto itUnpack : unpacks_){
        if (itUnpack.second->IsInited()){
            if (event->GetChild(itUnpack.first)) {
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
    FairRootManager* ioman = FairRootManager::Instance();
    LOG(INFO) << "[Digibuilder] " << ioman->GetEntryNr() << " events were processed" << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Reset(){
    for (auto itUnpack : unpacks_)
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
    
    if (fUserCut != "") {
        TTree* tr = fReader->GetInTree();
        fEventsForProcessing =  new TH1I ("hist", "Events for processing", tr->GetEntries(), 1, tr->GetEntries());
        Int_t res = 0;
        res = tr->Draw("Entry$>>hist",fUserCut,"goff");
        if (res == -1){
            LOG(FATAL) << "[Digibuilder] Error in user cut expression" << endl;
            return -1;
        }
        if (!fEventsForProcessing->GetEntries()) {
          LOG(WARNING) << "[Digibuilder] No data for analysis with defined user cut: "
                    << fUserCut << FairLogger::endl;
          return OpenNextFile();
        }
    }
    
    return 0;
}
//--------------------------------------------------------------------------------------------------
/*
void ERDigibuilder::DumpRawToScreen(DetEventDetector* det){
    LOG(DEBUG) << "[Digibuilder] Dump raw of " << det->GetName() << FairLogger::endl;
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
}*/
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::SetUserCut(TCut cut,Bool_t fillSkippedEvents/*=kTRUE*/){
    fUserCut = cut;
    fFillSkippedEvents=fillSkippedEvents;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDigibuilder)
