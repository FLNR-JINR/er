#ifndef ERTelescopeUnpack_H
#define ERTelescopeUnpack_H

#include <map>
#include <vector>

#include "TString.h"
#include "TMatrixD.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERTelescopeStation{
public:
    ERTelescopeStation(TString _type, Int_t _sideCount, TString _ampStName, TString _timeStName,
                        TString _ampStName2, TString _timeStName2, TString _ampCalFile, TString _timeCalFile,
                        TString _ampCalFile2, TString _timeCalFile2, TString _XY, TString _XYside);
    TString type; //Si, CsI
    Int_t sideCount; //1,2
    TString ampStName;
    TString timeStName;
    TString ampStName2;
    TString timeStName2;
    TString ampCalFile;
    TString timeCalFile;
    TString ampCalFile2;
    TString timeCalFile2;
    TMatrixD* ampCalTable;
    TMatrixD* timeCalTable;
    TMatrixD* ampCalTable2;
    TMatrixD* timeCalTable2;
    TString XY; // XY, YX
    TString XYside; //X,Y
    TString bName;
    TString bName2;
};

class ERTelescopeUnpack : public ERUnpack
{
  public:
    ERTelescopeUnpack(TString detName);
    virtual ~ERTelescopeUnpack();

    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);

    void AddSingleSiStation(TString name, TString ampStName, TString timeStName, 
                            TString ampCalFile, TString timeCalFile, TString XYside);
    void AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                            TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                            TString ampCalFile2, TString timeCalFile2, TString XY);
    void AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile);

  protected:
    void AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, Int_t stripNb, TString digiBranchName);
    void AddCsIDigi(Float_t edep, Double_t time, Int_t wallNb, Int_t blockNb, TString digiBranchName);
    TString FormBranchName(TString type, Int_t sideCount, TString stName, TString XY, TString XYside, Int_t volNb);
    void FormAllBranches();
    Bool_t ReadCalFiles();
    TMatrixD* ReadCalFile(TString fileName);
    void DumpStationsInfo();
    Bool_t ApplyCalibration(TMatrixD* ampCalTable, TMatrixD* timeCalTable, std::map<Int_t, 
                            std::pair<Double_t, Double_t> >& valueMap);
    Bool_t CheckSetup();

    std::map<TString,ERTelescopeStation*> fStations;
  public:
    ClassDef(ERTelescopeUnpack, 0)
};


#endif
