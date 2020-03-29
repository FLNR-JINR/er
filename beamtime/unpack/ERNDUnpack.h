#ifndef ERNDUnpack_H
#define ERNDUnpack_H

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERNDUnpack : public ERUnpack
{
  public:
    ERNDUnpack(TString detName, TString ampStation, TString timeStation, TString tacStation,
               TString ampCalFile, TString timeCalFile, TString tacCalFile,
               std::map<Int_t, Int_t>* channelsMapping = nullptr,
               Bool_t skipAloneChannels = kTRUE);
    virtual ~ERNDUnpack();

    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);
  protected:
    void AddNDDigi(Float_t edep, Double_t time, Int_t tac, Int_t channelNb);
    void ApplyCalibrations(const Int_t channel, Double_t& amp, Double_t& time, Double_t& tac);
    Bool_t CheckSetup();

    TString fAmpStation;
    TString fTimeStation;
    TString fTACStation;
    TMatrixD* fAmpCalTable = nullptr;
    TMatrixD* fTimeCalTable = nullptr;
    TMatrixD* fTACCalTable = nullptr;
    std::map<Int_t, Int_t>* fChannelsMapping = nullptr;
    Bool_t fSkipAloneChannels;
    
  public:
    ClassDef(ERNDUnpack, 0)
};


#endif
