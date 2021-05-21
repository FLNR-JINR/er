#ifndef ERDigibuilder_H
#define ERDigibuilder_H

#include <vector>
#include <map>
#include <memory>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TH1I.h"
#include "TChain.h"

#include "FairSource.h"

#include "SetupConfiguration.h"

#include "ERUnpack.h"

class ERBeamTimeEventHeader;

class ERDigibuilder : public FairSource {
 public:
  ERDigibuilder();
  ERDigibuilder(const TString& tree_name);
  virtual Bool_t Init();
  virtual Int_t ReadEvent(UInt_t=0);
  virtual void Close();
  virtual void Reset();
  virtual Source_Type GetSourceType() { return kFILE; }
  virtual void SetParUnpackers() {}
  virtual Bool_t InitUnpackers();
  virtual Bool_t ReInitUnpackers() { return kTRUE; }
  void AddUnpack(ERUnpack* unpack){ unpacks_[unpack->DetectorName()].reset(unpack); }
  void AddFile(const TString& path);
  void SetUserCut(const TCut& cut, bool hold_events_count = true);
 private:
  bool LoadEvent(uint event_number);
  void ApplyUserCut();
  static uint EventNumber();
  bool UserCutIsDefined() const;
  bool EventShouldBeProcessed(const uint event_number, ERBeamTimeEventHeader*) const;
  static void CheckEventHeader();
  void ConnectEventCommon();
  TChain input_chain_of_events_;
  std::map<TString, std::unique_ptr<ERUnpack>> unpacks_;
  /** Configuration of setup: detector named, station names, channel numbers.
    * Loaded from first file added with AddFile. **/ 
  SetupConfiguration* setup_configuration_ = nullptr;
  EventCommon* common_part_of_event_ = nullptr;
  TCut user_cut_;
  bool hold_events_count_ = true;
  TH1I* events_for_processing_ = nullptr;
  ClassDef(ERDigibuilder, 1)
};

#endif
