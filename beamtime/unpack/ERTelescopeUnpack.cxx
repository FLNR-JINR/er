#include "ERTelescopeUnpack.h"

#include <fstream>

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "DetEventFull.h"
#include "DetEventStation.h"
#include "DetMessage.h"

#include "ERSupport.h"
#include "ERDigi.h"

Int_t ERTelescopeUnpack::fSingleSiStationNewId = 0;
Int_t ERTelescopeUnpack::fSingleCsIStationNewId = 0;
Int_t ERTelescopeUnpack::fDoubleSiStationNewId = 0;     

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
        LOG(DEBUG) << FairLogger::endl << 
                      itStation.first << " unpacking ..." << FairLogger::endl;
        if (itStation.second->type == "Si"){
            Channel2AmplitudeTime valueMap;
            UnpackAmpTimeStation(detEvent,itStation.second->ampStName,itStation.second->timeStName,
                                 valueMap, itStation.second->skipAloneChannels);

            if ((itStation.second->ampCalFile != "") || (itStation.second->timeCalFile != "")){
                if (!ApplyCalibration(itStation.second->ampCalTable,itStation.second->timeCalTable,valueMap)){
                    LOG(FATAL) << "calibration error on station " << itStation.first << FairLogger::endl;
                }
            }
            else
                LOG(DEBUG) << "Calibration file for station " << itStation.second->ampStName << 
                    " or " << itStation.second->timeStName << " has not defined!" 
                    " No calibration for this detector station "<< FairLogger::endl;

            for (auto itValue : valueMap){
                ERChannel channel = GetChannelNumber(itValue.first, itStation.second->channelsMapping1);
                float amp = itValue.second.first; // [MeV]
                float time = itValue.second.second;
                AddDigi(amp,time, channel,itStation.second->bName);
            }

            if (itStation.second->sideCount == 2){
                valueMap.clear();
                UnpackAmpTimeStation(detEvent,itStation.second->ampStName2,
                                     itStation.second->timeStName2, 
                                     valueMap, itStation.second->skipAloneChannels);

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
                    ERChannel channel = GetChannelNumber(itValue.first, itStation.second->channelsMapping2);
                    float amp = itValue.second.first; // [MeV]
                    float time = itValue.second.second;
                    AddDigi(amp, time, channel, itStation.second->bName2);
                }
            }
        }

        if (itStation.second->type == "CsI"){
            Channel2AmplitudeTime valueMap;
            UnpackAmpTimeStation(detEvent,itStation.second->ampStName,itStation.second->timeStName,
                                 valueMap,itStation.second->skipAloneChannels);

            if ((itStation.second->ampCalFile != "") || (itStation.second->timeCalFile != ""))
                if (!ApplyCalibration(itStation.second->ampCalTable,itStation.second->timeCalTable, valueMap)){
                    LOG(FATAL) << "calibration error on station " << itStation.first << FairLogger::endl;
                }
            else
                LOG(DEBUG) << "Calibration file for station " << itStation.second->ampStName << 
                    " or " << itStation.second->timeStName << " has not defined!" 
                    " No calibration for this detrctor station "<< FairLogger::endl;

            for (auto itValue : valueMap){
                ERChannel channel = GetChannelNumber(itValue.first, itStation.second->channelsMapping1);
                float amp = itValue.second.first; // [MeV]
                float time = itValue.second.second;
                AddDigi(amp, time, channel, itStation.second->bName);
            }
        } 

    }
    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDigi(float edep, float time, ERChannel stripNb, TString digiBranchName)
{
  new((*fDigiCollections[digiBranchName])
        [fDigiCollections[digiBranchName]->GetEntriesFast()])ERDigi(edep, time, stripNb);
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeUnpack::FormBranchName(TString type, Int_t sideCount, TString stName, 
		                                  TString XY, TString XYside, Int_t volInd)
{
    //@todo убрать это по возможности
    Int_t stNumber = 0;
    if (fDetName.Contains("Right"))
        stNumber = 1;

    TString bName = "";
    if (sideCount == 1) {
        bName.Form("TelescopeDigi_%s_%s_%s_%d",fDetName.Data(),stName.Data(),
                                                    XYside.Data(), volInd);
    } else {
        bName.Form("TelescopeDigi_%s_%s_%s_%d_%s",fDetName.Data(),stName.Data(),
                                                    XY.Data(),volInd,XYside.Data());
    }
    return bName;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddSingleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampCalFile, TString timeCalFile, TString XYside,
                                           ChannelsMapping* channelsMapping/* = nullptr*/,
                                           Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "","", XYside,
                                                    channelsMapping, nullptr, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddDoubleSiStation(TString name, TString ampStName, TString timeStName,
                                           TString ampStName2, TString timeStName2, TString ampCalFile, TString timeCalFile,
                                           TString ampCalFile2, TString timeCalFile2, TString XY,
                                           ChannelsMapping* channelsMapping1/* = nullptr*/,
                                           ChannelsMapping* channelsMapping2/* = nullptr*/,
                                           Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "Si", 2, ampStName, timeStName, ampStName2, timeStName2, ampCalFile, timeCalFile,
                            ampCalFile2, timeCalFile2, XY, "", channelsMapping1, channelsMapping2, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::AddCsIStation(TString name,TString ampStName, TString timeStName, TString ampCalFile, TString timeCalFile,
                                      ChannelsMapping* channelsMapping /* = nullptr*/, Bool_t skipAloneChannels/* = kTRUE*/){
    ERTelescopeStation* st = new ERTelescopeStation( "CsI", -1, ampStName, timeStName, "", "", ampCalFile, timeCalFile, "", "", "", "",
                                                    channelsMapping, nullptr, skipAloneChannels);
    fStations[name] = st;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeUnpack::FormAllBranches(){

    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");
    for (auto itStation : fStations){
        if( itStation.second->sideCount == 2){
            if (itStation.second->XY == "XY"){
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","X", fDoubleSiStationNewId);
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","Y", fDoubleSiStationNewId);
            }
            else{
                itStation.second->bName = FormBranchName("Si",2,itStation.first,"XY","Y", fDoubleSiStationNewId);
                itStation.second->bName2 = FormBranchName("Si",2,itStation.first,"XY","X", fDoubleSiStationNewId);
            }
	        fDoubleSiStationNewId++;
        }
        else {
            itStation.second->bName = FormBranchName(itStation.second->type,
                                                    itStation.second->sideCount,
                                                    itStation.first,"",
                                                    itStation.second->XYside,
						                            itStation.second->type == "Si" 
                                                    ? fSingleSiStationNewId++ : fSingleCsIStationNewId++);
        }
    }
    for (auto itStation : fStations){
        TString bName = itStation.second->bName;
        if (itStation.second->type == "Si"){
            fDigiCollections[bName] = new TClonesArray("ERDigi", 10);
            ioman->Register(bName,fDetName, fDigiCollections[bName], kTRUE);
            if (itStation.second->sideCount == 2){
                TString bName2 = itStation.second->bName2;
                fDigiCollections[bName2] = new TClonesArray("ERDigi", 10);
                ioman->Register(bName2,fDetName, fDigiCollections[bName2], kTRUE);
            }
        }
        if (itStation.second->type == "CsI"){
            fDigiCollections[bName] = new TClonesArray("ERDigi", 10);
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
                                       TString _ampCalFile2, TString _timeCalFile2, TString _XY, TString _XYside,
                                       ChannelsMapping* _channelsMapping1, ChannelsMapping* _channelsMapping2,
                                       Bool_t _skipAloneChannels):
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
    bName(""),
    bName2(""),
    channelsMapping1(_channelsMapping1),
    channelsMapping2(_channelsMapping2),
    skipAloneChannels(_skipAloneChannels)
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
Bool_t ERTelescopeUnpack::ApplyCalibration(TMatrixD* ampCalTable, TMatrixD* timeCalTable, 
                                           Channel2AmplitudeTime& valueMap) {
    for (auto& itValue : valueMap) {
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
        if (timeCalTable) {
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
