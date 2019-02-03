#include "ERTelescopeUnpack.h"

#include <iostream>
#include <fstream>

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairLogger.h"

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
    for (auto itStation : fStations){
        if (itStation.second)
            delete itStation.second;
    }
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::Init(SetupConfiguration* setupConf){
    if (!ERUnpack::Init(setupConf))
        return kTRUE;

    if (fStations.size() == 0)
        Fatal("Init", "No Stations in ERTelescopeUnpack !");


    fSetupConfiguration = setupConf;

    if (!CheckSetup())
        Fatal("Init", "Error in ERTelescopeUnpack setup checking !");

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

    for (auto itStation : fStations){
        if (itStation.second->type == "Si"){
            std::map<Int_t, std::pair<Double_t, Double_t> > valueMap;
            UnpackAmpTimeStation(detEvent,itStation.second->ampStName,itStation.second->timeStName,
                                 valueMap);

            if ((itStation.second->ampCalFile != "") || (itStation.second->timeCalFile != "")){
                if (!ApplyCalibration(itStation.second->ampCalTable,itStation.second->timeCalTable,valueMap)){
                    LOG(FATAL) << "calibration error on station " << itStation.first << FairLogger::endl;
                }
            }
            else
                LOG(DEBUG) << "Calibration file for station " << itStation.second->ampStName << 
                    " or " << itStation.second->timeStName << " has not defined!" 
                    " No calibration for this detrctor station "<< FairLogger::endl;

            for (auto itValue : valueMap){
                Int_t channel = itValue.first;
                Double_t amp = itValue.second.first /1000.; //to GeV
                Double_t time = itValue.second.second;
                AddSiDigi(amp,time,0,channel,itStation.second->bName);
            }

            if (itStation.second->sideCount == 2){
                valueMap.clear();
                UnpackAmpTimeStation(detEvent,itStation.second->ampStName2,
                                     itStation.second->timeStName2, valueMap);

                if ((itStation.second->ampCalFile2 != "") || (itStation.second->timeCalFile2 != "")){
                    if (!ApplyCalibration(itStation.second->ampCalTable2,itStation.second->timeCalTable2,valueMap)){
                        LOG(FATAL) << "calibration error on station " << itStation.first << FairLogger::endl;
                    }
                }
                else
                    LOG(DEBUG) << "Calibration file for station " << itStation.second->ampStName2 << 
                        " or " << itStation.second->timeStName2 << " has not defined!" 
                        " No calibration for this detrctor station "<< FairLogger::endl;

                for (auto itValue : valueMap){
                    Int_t channel = itValue.first;
                    Double_t amp = itValue.second.first /1000.; //to GeV
                    Double_t time = itValue.second.second;
                    AddSiDigi(amp, time, 0, channel, itStation.second->bName2);
                }
            }
        }

        if (itStation.second->type == "CsI"){
            std::map<Int_t, std::pair<Double_t, Double_t> > valueMap;
            UnpackAmpTimeStation(detEvent,itStation.second->ampStName,itStation.second->timeStName,
                                     valueMap);

            if ((itStation.second->ampCalFile != "") || (itStation.second->timeCalFile != ""))
                if (!ApplyCalibration(itStation.second->ampCalTable,itStation.second->timeCalTable, valueMap)){
                    LOG(FATAL) << "calibration error on station " << itStation.first << FairLogger::endl;
                }
            else
                LOG(DEBUG) << "Calibration file for station " << itStation.second->ampStName << 
                    " or " << itStation.second->timeStName << " has not defined!" 
                    " No calibration for this detrctor station "<< FairLogger::endl;

            for (auto itValue : valueMap){
                Int_t channel = itValue.first;
                Double_t amp = itValue.second.first;
                Double_t time = itValue.second.second;
                AddCsIDigi(amp, time, -1, channel, itStation.second->bName);
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
  ERQTelescopeCsIDigi *digi = new((*fDigiCollections[digiBranchName])
                                                   [fDigiCollections[digiBranchName]->GetEntriesFast()])
              ERQTelescopeCsIDigi(fDigiCollections[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                    wallNb, 
                                                                                    blockNb);
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeUnpack::FormBranchName(TString type, Int_t sideCount, TString stName, TString XY, TString XYside, Int_t volNb){
    //@todo убрать это по возможности
    Int_t stNumber = 0;
    if (fDetName.Contains("Right"))
        stNumber = 1;

    TString bName = "";
    if (type == TString("Si"))
        if (sideCount == 1)
            bName.Form("ERQTelescopeSiDigi_%s_SingleSi_%s_%s_%d",fDetName.Data(),stName.Data(),XYside.Data(), volNb);
        else
            bName.Form("ERQTelescopeSiDigi_%s_DoubleSi_%s_%s_%d_%s",fDetName.Data(),stName.Data(),XY.Data(),volNb,XYside.Data());
    if (type == TString("CsI"))
        bName.Form("ERQTelescopeCsIDigi_%s_%s_%d",fDetName.Data(),stName.Data(), volNb);
    return bName;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSingleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampCalFile, TString timeCalFile, TString XYside){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "","", XYside);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                                           TString ampCalFile2, TString timeCalFile2, TString XY){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 2, ampStName, timeStName, ampStName2, timeStName2, ampCalFile, timeCalFile,
                            ampCalFile2, timeCalFile2, XY, "");
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile){
    ERTelescopeStation* st = new ERTelescopeStation( "CsI", -1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "", "", "");
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::FormAllBranches(){

    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");
    static Int_t doubleSiCnt = 0;
    static Int_t singleSiCnt = 0;
    static Int_t csiCnt = 0;
    for (auto itStation : fStations){
        if( itStation.second->sideCount == 2){
            if (itStation.second->XY == "XY"){
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","X", doubleSiCnt);
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","Y", doubleSiCnt);
            }
            else{
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","Y", doubleSiCnt);
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","X", doubleSiCnt);
            }
            doubleSiCnt++;
        }
        else {
            int volTypeNb;
            if (itStation.second->type.Contains("CsI")) {
                volTypeNb = csiCnt++;
            } else {
                volTypeNb = singleSiCnt++;                
            }
            itStation.second->bName = FormBranchName(itStation.second->type,
                                                    itStation.second->sideCount,
                                                    itStation.first,"",
                                                    itStation.second->XYside,
                                                    volTypeNb);
        }
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
    LOG(INFO) << "!!! Stations info: " << FairLogger::endl; 
    for (auto itStation : fStations){
        LOG(INFO) << "\t" << itStation.first << FairLogger::endl;
        LOG(INFO) << "\t\ttype : " << itStation.second->type << FairLogger::endl <<
                "\t\tsideCount : " << itStation.second->sideCount << FairLogger::endl <<
                "\t\tampStName : " << itStation.second->ampStName << FairLogger::endl << 
                "\t\ttimeStName : " << itStation.second->timeStName << FairLogger::endl << 
                "\t\tampStName2 : " << itStation.second->ampStName2 << FairLogger::endl << 
                "\t\ttimeStName2 : " << itStation.second->timeStName2 << FairLogger::endl <<
                "\t\tXY : " << itStation.second->XY << FairLogger::endl <<
                "\t\tXYside : " << itStation.second->XYside << FairLogger::endl <<
                "\t\tbName : " << itStation.second->bName << FairLogger::endl <<
                "\t\tbName2 : " << itStation.second->bName2 << FairLogger::endl;
        if (itStation.second->ampCalTable){
            LOG(INFO) << "\t\tampCalFile : " << itStation.second->ampCalFile << FairLogger::endl;
            LOG(INFO) << "\t\tampCalTable : " << FairLogger::endl;
            itStation.second->ampCalTable->Print();
        }
        if (itStation.second->timeCalTable){
            LOG(INFO) << "\t\ttimeCalFile : " << itStation.second->timeCalFile << FairLogger::endl;
            LOG(INFO) << "\t\ttimeCalTable : " << FairLogger::endl;
            itStation.second->timeCalTable->Print();
        }
        if (itStation.second->ampCalTable2){
            LOG(INFO) << "\t\tampCalFile2 : " << itStation.second->ampCalFile2 << FairLogger::endl;
            LOG(INFO) << "\t\tampCalTable2 : " << FairLogger::endl;
            itStation.second->ampCalTable2->Print();
        }
        if (itStation.second->timeCalTable2){
            LOG(INFO) << "\t\ttimeCalFile2 : " << itStation.second->timeCalFile2 << FairLogger::endl;
            LOG(INFO) << "\t\ttimeCalTable2 : " << FairLogger::endl;
            itStation.second->timeCalTable2->Print();
        }
    }
}
//--------------------------------------------------------------------------------------------------
ERTelescopeStation::ERTelescopeStation(TString _type, Int_t _sideCount, TString _ampStName, TString _timeStName,
                                       TString _ampStName2, TString _timeStName2, TString _ampCalFile, TString _timeCalFile,
                                       TString _ampCalFile2, TString _timeCalFile2, TString _XY, TString _XYside):
    type(_type),
    sideCount(_sideCount),
    ampStName(_ampStName),
    timeStName(_timeStName),
    ampStName2(_ampStName2),
    timeStName2(_timeStName2),
    ampCalFile(_ampCalFile),
    timeCalFile(_timeCalFile),
    ampCalFile2(_ampCalFile2),
    timeCalFile2(_timeCalFile2),
    XY(_XY),
    XYside(_XYside),
    ampCalTable(NULL),
    timeCalTable(NULL),
    ampCalTable2(NULL),
    timeCalTable2(NULL),
    bName(""),
    bName2("")
{
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::ReadCalFiles(){
    for (auto itStation : fStations){
        
        if (itStation.second->ampCalFile != ""){
            itStation.second->ampCalTable = ReadCalFile(itStation.second->ampCalFile);
            if (!itStation.second->ampCalTable)
                return kFALSE;
        }

        if (itStation.second->timeCalFile != ""){
            itStation.second->timeCalTable = ReadCalFile(itStation.second->timeCalFile);
            if (!itStation.second->timeCalTable)
                return kFALSE;
        }
              
        if (itStation.second->sideCount == 2 && itStation.second->ampCalFile2 != ""){
            itStation.second->ampCalTable2 = ReadCalFile(itStation.second->ampCalFile2);
            if (!itStation.second->ampCalTable2)
                return kFALSE;
        }

        if (itStation.second->sideCount == 2 && itStation.second->timeCalFile2 != ""){
            itStation.second->timeCalTable2 = ReadCalFile(itStation.second->timeCalFile2);
            if (!itStation.second->timeCalTable2)
                return kFALSE;
        }

    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
TMatrixD* ERTelescopeUnpack::ReadCalFile(TString fileName){
    ifstream in;
    in.open(fileName);
    if (!in.is_open()){
        LOG(FATAL) << "Can`t open calibration file " << fileName << FairLogger::FairLogger::endl;
        return NULL;
    }

    Int_t nRows = -1, nCols = -1;
    in >> nCols;
    in >> nRows;
    if (nCols <= 0 || nRows <= 0){
        LOG(FATAL) << "Can`t read rows or cols from calibration file " << fileName << FairLogger::FairLogger::endl;
        return NULL;
    }

    TMatrixD* calTable = new TMatrixD(nRows,nCols);
    Int_t i = 0;

    while (!in.eof()){
        if (i >= nRows){
            LOG(FATAL) << "Wrong file format in " << fileName << FairLogger::FairLogger::endl;
            return NULL;
        }
        in >> (*calTable)[i][0] >> (*calTable)[i][1];
        i++;
    }

    return calTable;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::ApplyCalibration(TMatrixD* ampCalTable, TMatrixD* timeCalTable, 
                            std::map<Int_t, std::pair<Double_t, Double_t> >& valueMap){

    for (auto& itValue : valueMap){

        //Amplitude calibration
        if (ampCalTable){
            if (itValue.first >= ampCalTable->GetNrows()){
                LOG(FATAL) << "Channel " << itValue.first << " not found in amplitude calibration table of detector " 
                           << fDetName << FairLogger::endl;
                return kFALSE;
            }
            itValue.second.first = itValue.second.first*(*ampCalTable)[itValue.first][1] + (*ampCalTable)[itValue.first][0];
        }
        
        //Time calibration
        if (timeCalTable){
            if (itValue.first >= timeCalTable->GetNrows()){
                LOG(FATAL) << "Channel " << itValue.first << " not found in time calibration table of detector " 
                           << fDetName << FairLogger::endl;
                return kFALSE;
            }
            itValue.second.second = itValue.second.second*(*timeCalTable)[itValue.first][1] + (*timeCalTable)[itValue.first][0];
        }
    }

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::CheckSetup() {

    const std::map<TString, unsigned short> stationsInConfig = fSetupConfiguration->GetStationList(fDetName);
    for (auto itStation : fStations){
        ERTelescopeStation* station = itStation.second;
        if (stationsInConfig.find(station->ampStName) == stationsInConfig.end()){
            LOG(FATAL) << "Amplitude station " << station->ampStName <<
                          " of telescope " << fDetName << 
                          " not found in setup configuration file" << FairLogger::endl;
            return kFALSE;
        }
        if (station->timeStName != ""){
            if (stationsInConfig.find(station->timeStName) == stationsInConfig.end()){
                LOG(FATAL) << "Time station " << station->timeStName <<
                            " of telescope " << fDetName << 
                            " not found in setup configuration file" << FairLogger::endl;
                return kFALSE;
            }
        }
        if (station->sideCount == 2){
            if (stationsInConfig.find(station->ampStName2) == stationsInConfig.end()){
            LOG(FATAL) << "Amplitude station " << station->ampStName2 <<
                            " of telescope " << fDetName << 
                            " not found in setup configuration file" << FairLogger::endl;
            return kFALSE;
            }
            if (station->timeStName2 != ""){
                if (stationsInConfig.find(station->timeStName2) == stationsInConfig.end()){
                    LOG(FATAL) << "Time station " << station->timeStName2 <<
                            " of telescope " << fDetName << 
                            " not found in setup configuration file" << FairLogger::endl;
                    return kFALSE;
                }
            }
        }
    }

    for (auto itConfSt : stationsInConfig){
        TString stationName = itConfSt.first;
        Bool_t found = kFALSE;
        for (auto itStation : fStations){
            ERTelescopeStation* station = itStation.second;
            if (station->ampStName == stationName ||
                station->timeStName == stationName || 
                station->ampStName2 == stationName || 
                station->timeStName2 == stationName)
            LOG(WARNING) << "Station " << stationName << " in setup file, but not defined to unpack!" << FairLogger::FairLogger::endl; 
        }
    }

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeUnpack)