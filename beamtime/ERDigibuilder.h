#ifndef ERDigibuilder_H
#define ERDigibuilder_H

#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "Reader.h"

#include "ERRawEvent.h"

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

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void SetConfigurationFile(TString name){fSetupFile = name;}
    void AddFile(TString path){fPath.push_back(path);}
  private:
    // Вектор lmd файлов для обработки.
    std::vector<TString> fPath;
    // Индекс текущего файла в fPath.
    Int_t fCurFile;
    // Количество событий обработанных в предыдущих файлах
    Int_t fOldEvents;

    TString fSetupFile;
    Reader* fReader;

    Int_t OpenNextFile();
  public:
    ClassDef(ERDigibuilder, 1)
};


#endif
