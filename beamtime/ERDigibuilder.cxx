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
ERDigibuilder::ERDigibuilder()
  : input_chain_of_events_("accdaq")
{}
//--------------------------------------------------------------------------------------------------
ERDigibuilder::ERDigibuilder(const TString& tree_name)
  : input_chain_of_events_(tree_name)
{}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::AddFile(const TString& path) {
  input_chain_of_events_.Add(path);
  if (!setup_configuration_) {
    TFile file(path);
    std::cerr << setup_configuration_ << std::endl;
    setup_configuration_ = dynamic_cast<SetupConfiguration*>(file.Get("SetupConfiguration"));
    std::cerr << setup_configuration_ << std::endl;
    if (!setup_configuration_)
      LOG(FATAL) << "Cannot load setup configuration from file " << path << FairLogger::endl;
  }
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::Init() {
  if (input_chain_of_events_.GetEntriesFast() == 0)
    LOG(FATAL) << "No events for processing in source ERDigibuilder" << FairLogger::endl;
  ApplyUserCut();
  CheckEventHeader();
  InitUnpackers();
  ConnectEventCommon();
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDigibuilder::InitUnpackers(){
  const std::map<TString, unsigned short> detectors = setup_configuration_->GetDetectorList();
  for (const auto& detector : detectors) {
    const auto detector_name = detector.first;
    if (unpacks_.find(detector_name) != unpacks_.end()) {
      unpacks_[detector_name]->Init(setup_configuration_, input_chain_of_events_);
    } else {
      LOG(WARNING) << "[Digibuilder] " << detector_name << " is defined in setup "
                    << "configuration, but unpacker is not added!" << FairLogger::endl;
    }
  }
  for (const auto& detector_name_and_unpack : unpacks_) {
    const auto& detector_name = detector_name_and_unpack.first;
    const auto& unpack = detector_name_and_unpack.second;
    if (!unpack->IsInited()) {
      LOG(WARNING) << "[Digibuilder] Detector " << detector_name << " not found in setup file. "
                   << "Unpacker has not inited!" << FairLogger::endl;
    }
  }
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Int_t ERDigibuilder::ReadEvent(UInt_t) {
  if (!common_part_of_event_)
    LOG(FATAL) << "Object fot store common part of event was not inited" << FairLogger::endl;
  Reset();
  const auto event_number = EventNumber();
  if (!LoadEvent(event_number))
    return 1; // all events have been already processed
  FairRun* run = FairRun::Instance();
  auto* header = dynamic_cast<ERBeamTimeEventHeader*>(run->GetEventHeader());
  if (!EventShouldBeProcessed(event_number, header))
    return 0;
  header->SetTrigger(common_part_of_event_->trigger);
  for (auto& detector_name_and_unpack : unpacks_) {
    const auto& detector_name = detector_name_and_unpack.first;
    auto& unpack = detector_name_and_unpack.second;
    if (unpack->IsInited()) {
      unpack->DoUnpack(nullptr, 0);
    }
  }
  return 0;
}
//--------------------------------------------------------------------------------------------------
uint ERDigibuilder::EventNumber() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "No FairRootManager" << FairLogger::endl;
  return static_cast<uint>(ioman->GetEntryNr());
}
//--------------------------------------------------------------------------------------------------
bool ERDigibuilder::LoadEvent(const uint event_number) {
  if (event_number % 10000 == 0)
    LOG(INFO) << "[Digibuilder] Event " << event_number << FairLogger::endl;
  return input_chain_of_events_.GetEntry(event_number);
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Close() {
  LOG(INFO) << "[Digibuilder] " << EventNumber() << " events were processed" << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::Reset(){
  for (auto& detector_name_and_unpack : unpacks_) {
    auto& unpack = detector_name_and_unpack.second;
    unpack->Reset();
  }
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::ApplyUserCut(){
  if (!UserCutIsDefined())
    return;
  events_for_processing_ = new TH1I("events_for_processing", "Events for processing",
                                    input_chain_of_events_.GetEntries(), 1,
                                    input_chain_of_events_.GetEntries());
  if (input_chain_of_events_.Draw("Entry$>>events_for_processing", user_cut_, "goff") == -1)
    LOG(FATAL) << "[Digibuilder] Error in user cut expression: " << user_cut_ << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
bool ERDigibuilder::UserCutIsDefined() const {
  return user_cut_ != "";
}
//--------------------------------------------------------------------------------------------------
bool ERDigibuilder::EventShouldBeProcessed(const uint event_number,
                                           ERBeamTimeEventHeader* header) const {
  if (!UserCutIsDefined())
    return true;
  if (!events_for_processing_->GetBinContent(event_number)) {
    LOG(DEBUG) << "[Digibuilder] Event is skipped due user cut. " << FairLogger::endl;
    if (header)
      header->SetTrigger(-1);
    if (!hold_events_count_) {
      FairRun* run = FairRun::Instance();
      run->MarkFill(kFALSE);
    }
    return false;
  }
  return true;
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::CheckEventHeader() {
  FairRun* run = FairRun::Instance();
  if (!dynamic_cast<ERBeamTimeEventHeader*>(run->GetEventHeader())) {
    LOG(WARNING) << "[Digibuilder] ERBeamTimeEventHeader is not used in digibuilding. "
                 << "Trigger and another common information will not be written to output file."
                 << FairLogger::endl;
  }
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::ConnectEventCommon() {
  if (!input_chain_of_events_.FindBranch("common")) {
    LOG(FATAL) << "Input file does not contain branch common with trigger"
               << " and another common information" << FairLogger::endl;
  }
  common_part_of_event_ = new EventCommon();
  input_chain_of_events_.SetBranchAddress("common", &common_part_of_event_);
}
//--------------------------------------------------------------------------------------------------
void ERDigibuilder::SetUserCut(const TCut& cut, const bool hold_events_count/*= true*/){
    user_cut_ = cut;
    hold_events_count_ = hold_events_count;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDigibuilder)
