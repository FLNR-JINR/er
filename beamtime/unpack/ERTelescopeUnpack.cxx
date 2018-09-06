#include "ERTelescopeUnpack.h"

#include <iostream>
#include <fstream>

#include "TClonesArray.h"

#include "FairRootManager.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::ERTelescopeUnpack(TString detName):
ERUnpack(detName)
{

}
//--------------------------------------------------------------------------------------------------
ERTelescopeUnpack::~ERTelescopeUnpack(){

}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::Init(SetupConfiguration* setupConf){
    if (!ERUnpack::Init(setupConf))
        return kTRUE;

    if (fStations.size() == 0)
        Fatal("Init", "No Stations in ERTelescopeUnpack !");


    fSetupConfiguration = setupConf;

    // TODO check setup conf and fStations
    CheckSetup();

    FormAllBranches();

    if (!ReadCalFiles())
        Fatal("Init", "Problem in ReadCalFiles!");

    DumpStationsInfo();
    
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::DoUnpack(Int_t* data, Int_t size){
    if (!ERUnpack::DoUnpack(data,size))
        return kTRUE;

    DetEventFull* event = (DetEventFull*)data;

    DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);

    for (auto itStation : fStations){
        if (itStation.second->type == "Si"){
            std::map<Int_t, std::pair<Double_t, Double_t> > valueMap;
            UnpackAmpTimeStation(detEvent,itStation.second->ampStName,itStation.second->timeStName,
                                 valueMap);
            if (!ApplyCalibration(itStation.second->calTable,valueMap)){
                cerr << "calibration error on station " << itStation.first << endl;
            }
            for (auto itValue : valueMap){
                Int_t channel = itValue.first;
                Double_t amp = itValue.second.first;
                Double_t time = itValue.second.second;
                AddSiDigi(amp,time,0,channel,itStation.second->bName);
            }
            if (itStation.second->sideCount == 2){
                valueMap.clear();
                UnpackAmpTimeStation(detEvent,itStation.second->ampStName2,itStation.second->timeStName2,
                                     valueMap);
                if (!ApplyCalibration(itStation.second->calTable2,valueMap)){
                    cerr << "calibration error on station " << itStation.first << endl;
                }
                for (auto itValue : valueMap){
                    Int_t channel = itValue.first;
                    Double_t amp = itValue.second.first;
                    Double_t time = itValue.second.second;
                    AddSiDigi(amp,time,0,channel,itStation.second->bName2);
                }
            }
        }
        if (itStation.second->type == "CsI"){
            map<Int_t,Double_t> csiAmp;
            UnpackStation(detEvent,itStation.second->ampStName,csiAmp);
            for (auto itChannel : csiAmp){
                AddCsIDigi(itChannel.second,0.,-1,itChannel.first,itStation.second->bName);
            }
        } 
    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, 
                                                                                  Int_t stripNb,
                                                                                  TString digiBranchName)
{
  ERQTelescopeSiDigi *digi = new((*fDigiCollections[digiBranchName])
                                                  [fDigiCollections[digiBranchName]->GetEntriesFast()])
              ERQTelescopeSiDigi(fDigiCollections[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                     stationNb, 
                                                                                     stripNb);
}
//-------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddCsIDigi(Float_t edep, Double_t time, Int_t wallNb, 
                                                                    Int_t blockNb,
                                                                    TString digiBranchName)
{
  cerr << digiBranchName << endl;
  ERQTelescopeCsIDigi *digi = new((*fDigiCollections[digiBranchName])
                                                   [fDigiCollections[digiBranchName]->GetEntriesFast()])
              ERQTelescopeCsIDigi(fDigiCollections[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                    wallNb, 
                                                                                    blockNb);
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeUnpack::FormBranchName(TString type, Int_t sideCount, TString stName, TString XY, TString XYside){
    TString bName = "";
    if (type == TString("Si"))
        if (sideCount == 1)
            bName.Form("ERQTelescopeSiDigi_%s_SingleSi_%s_%s_0",fDetName.Data(),stName.Data(),XYside.Data());
        else
            bName.Form("ERQTelescopeSiDigi_%s_DoubleSi_%s_%s_0_%s",fDetName.Data(),stName.Data(),XY.Data(),XYside.Data());
    if (type == TString("CsI"))
        bName.Form("ERQTelescopeCsIDigi_%s_%s_0",fDetName.Data(),stName.Data());
    return bName;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSingleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString calFile, TString XYside){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 1, ampStName, timeStName, "", "", calFile, "","", XYside);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampStName2, TString timeStName2, TString calFile,
                                           TString calFile2, TString XY){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 2, ampStName, timeStName, ampStName2, timeStName2, calFile, calFile2, XY, "");
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddCsIStation(TString name,TString ampStName,TString calFile){
    ERTelescopeStation* st = new ERTelescopeStation( "CsI", -1, ampStName, "", "", "", calFile, "", "", "");
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::FormAllBranches(){

    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    for (auto itStation : fStations){
        if( itStation.second->sideCount == 2){
            if (itStation.second->XY == "XY"){
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","X");
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","Y");
            }
            else{
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","Y");
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","X");
            }
        }
        else
            itStation.second->bName = FormBranchName(itStation.second->type,
                                                    itStation.second->sideCount,
                                                    itStation.first,"",
                                                    itStation.second->XYside);
    }

    for (auto itStation : fStations){
        TString bName = itStation.second->bName;
        if (itStation.second->type == "Si"){
            fDigiCollections[bName] = new TClonesArray("ERQTelescopeSiDigi",1000);
            ioman->Register(bName,fDetName, fDigiCollections[bName], kTRUE);
            if (itStation.second->sideCount == 2){
                TString bName2 = itStation.second->bName2;
                fDigiCollections[bName2] = new TClonesArray("ERQTelescopeSiDigi",1000);
                ioman->Register(bName2,fDetName, fDigiCollections[bName2], kTRUE);
            }
        }
        if (itStation.second->type == "CsI"){
            fDigiCollections[bName] = new TClonesArray("ERQTelescopeCsIDigi",1000);
            ioman->Register(bName,fDetName, fDigiCollections[bName], kTRUE);
        }
    }
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::DumpStationsInfo(){
    cerr << "!!! Stations info: " << endl; 
    for (auto itStation : fStations){
        cerr << "\t" << itStation.first << endl;
        cerr << "\t\ttype : " << itStation.second->type << endl <<
                "\t\tsideCount : " << itStation.second->sideCount << endl <<
                "\t\tampStName : " << itStation.second->ampStName << endl << 
                "\t\ttimeStName : " << itStation.second->timeStName << endl << 
                "\t\tampStName2 : " << itStation.second->ampStName2 << endl << 
                "\t\ttimeStName2 : " << itStation.second->timeStName2 << endl <<
                "\t\tXY : " << itStation.second->XY << endl <<
                "\t\tXYside : " << itStation.second->XYside << endl <<
                "\t\tbName : " << itStation.second->bName << endl <<
                "\t\tbName2 : " << itStation.second->bName2 << endl;
        if (itStation.second->calTable){
            cerr << "\t\tcalFile : " << itStation.second->calFile << endl;
            cerr << "\t\tcalTable : " << endl;
            itStation.second->calTable->Print();
        }
        if (itStation.second->calTable2){
            cerr << "\t\tcalFile2 : " << itStation.second->calFile2 << endl;
            cerr << "\t\tcalTable2 : " << endl;
            itStation.second->calTable2->Print();
        }
    }
}
//--------------------------------------------------------------------------------------------------
ERTelescopeStation::ERTelescopeStation(TString _type, Int_t _sideCount, TString _ampStName, TString _timeStName,
                                       TString _ampStName2, TString _timeStName2, TString _calFile, TString _calFile2,
                                       TString _XY, TString _XYside):
    type(_type),
    sideCount(_sideCount),
    ampStName(_ampStName),
    timeStName(_timeStName),
    ampStName2(_ampStName2),
    timeStName2(_timeStName2),
    calFile(_calFile),
    calFile2(_calFile2),
    XY(_XY),
    XYside(_XYside),
    calTable(NULL),
    calTable2(NULL),
    bName(""),
    bName2("")
{
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::ReadCalFiles(){
    for (auto itStation : fStations){
        if (itStation.second->type == "CsI")
            continue;

        itStation.second->calTable = ReadCalFile(itStation.second->calFile);
        if (!itStation.second->calTable)
            return kFALSE;

        if (itStation.second->sideCount == 2)
            itStation.second->calTable2 = ReadCalFile(itStation.second->calFile2);
    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
TMatrixD* ERTelescopeUnpack::ReadCalFile(TString fileName){
    ifstream in;
    in.open(fileName);
    if (!in.is_open()){
        cerr << "Can`t read calibration file " << fileName << endl;
        return NULL;
    }

    Int_t nRows = -1, nCols = -1;
    in >> nCols;
    in >> nRows;
    if (nCols <= 0 || nRows <= 0){
        cerr << "Can`t read rows or cols from calibration file " << fileName << endl;
        return NULL;
    }

    TMatrixD* calTable = new TMatrixD(nRows,nCols);
    Int_t i = 0;

    while (!in.eof()){
        if (i >= nRows){
            cerr << "Wrong file format in " << fileName << endl;
            return NULL;
        }
        in >> (*calTable)[i][0] >> (*calTable)[i][1];
        i++;
    }

    return calTable;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::ApplyCalibration(TMatrixD* calTable, std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap){
    for (auto& itValue : valueMap){
        if (itValue.first >= calTable->GetNrows()){
            cerr << "channel number not found in calibration table" << endl;
            return kFALSE;
        }
        itValue.second.first = itValue.second.first*(*calTable)[itValue.first][1] + (*calTable)[itValue.first][0];
        cerr << itValue.second.first << endl;
    }

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeUnpack)