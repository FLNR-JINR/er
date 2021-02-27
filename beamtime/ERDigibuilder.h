#ifndef ERDigibuilder_H
#define ERDigibuilder_H

#include <vector>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TH1I.h"

#include "FairSource.h"

#include "Reader.h"
#include "SetupConfiguration.h"
#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERDigibuilder : public FairSource {
 public:
  ERDigibuilder() = default;
  virtual Bool_t Init();
  virtual Int_t ReadEvent(UInt_t=0);
  virtual void Close();
  virtual void Reset();
  virtual Source_Type GetSourceType() { return kFILE; }
  virtual void SetParUnpackers() {}
  virtual Bool_t InitUnpackers();
  virtual Bool_t ReInitUnpackers() { return kTRUE; }
  void AddUnpack(ERUnpack* unpack){ unpacks_[unpack->GetDetName()] = unpack; }
  void AddFile(const TString& path) { file_paths_.push_back(path); }
  void SetUserCut(const TCut& cut, bool fill_skipped_events = true);
 private:
  std::map<TString, ERUnpack*> unpacks_;
  std::vector<TString> file_paths_;
  uint current_file_index_ = 0;
  uint events_count_in_already_processed_files_ = 0;
  SetupConfiguration* setup_configuration_ = nullptr;
  TCut user_cut_;
  bool fill_skipped_events_ = true;
  TH1I* events_for_processing_ = nullptr;
  //void DumpRawToScreen(DetEventDetector* det);
  Int_t OpenNextFile();
  ClassDef(ERDigibuilder, 1)
};

#endif
