#ifndef ERBeamDetUnpack_H
#define ERBeamDetUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERBeamDetUnpack : public ERUnpack {
 public:
  ERBeamDetUnpack(TString detName);
  virtual ~ERBeamDetUnpack();
  virtual Bool_t Init(SetupConfiguration* setupConf);
  virtual Bool_t DoUnpack(Int_t* data, Int_t size);
  /* accessors */
  void SetToFCalibration(Double_t a1, Double_t b1, Double_t a2, Double_t b2) {
    fCalToFa1 = a1; fCalToFb1 = b1; fCalToFa2 = a2; fCalToFb2 = b2; 
  }
  void SetMWPCCalibration(Double_t a, Double_t b) {
    fCalMWPCa = a; fCalMWPCb = b; 
  }
  void SetMWPCMapping(ChannelsMapping* channelsMapping) {
    fMwpcChannelsMapping = channelsMapping;
  }
 protected:
  void AddToFDigi(float edep, float time, Int_t tofNb);
  void AddMWPCDigi(float edep, float time, TString mwpcSt, Int_t wireNb);
 protected:
  std::map<TString,Int_t> fMwpcAmpTimeStations;
  std::map<TString,TString> fMwpcBnames;
  ChannelsMapping* fMwpcChannelsMapping = nullptr;
  float fCalToFa1, fCalToFb1, fCalToFa2, fCalToFb2;
  float fCalMWPCa, fCalMWPCb;
 public:
  ClassDef(ERBeamDetUnpack, 0)
};


#endif
