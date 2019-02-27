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

class ERDigibuilder : public FairSource
{
  public:
    ERDigibuilder();
    ERDigibuilder(const ERDigibuilder& source);
    virtual ~ERDigibuilder();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers();

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void AddUnpack(ERUnpack* unpack){ fUnpacks[unpack->GetDetName()] = unpack; }

    void SetConfigurationFile(TString name){fSetupFile = name;}
    void AddFile(TString path){fPath.push_back(path);}
    void SetUserCut(TCut cut,Bool_t fillSkippedEvents=kTRUE);
  private:
    std::map<TString, ERUnpack*> fUnpacks;

    // Вектор lmd файлов для обработки.
    std::vector<TString> fPath;
    // Индекс текущего файла в fPath.
    Int_t fCurFile;
    // Количество событий обработанных в предыдущих файлах
    Int_t fOldEvents;

    TString fSetupFile;
    SetupConfiguration* fSetupConfiguration;
    Reader* fReader;

    TCut fUserCut;
    Bool_t fFillSkippedEvents;
    TH1I* fEventsForProcessing;

    void DumpRawToScreen(DetEventDetector* det);
    Int_t OpenNextFile();
  public:
    ClassDef(ERDigibuilder, 1)
};

#endif
