#include "ERTelescopeUnpack.h"

#include <iostream>

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

    FairRootManager* ioman = FairRootManager::Instance();
    if ( ! ioman ) Fatal("Init", "No FairRootManager");

    fSetupConfiguration = setupConf;

    fBnames["SQX_R"] = FormBranchName("Si",2, "SQ_R", "XY","X");
    fBnames["SQY_R"] = FormBranchName("Si",2, "SQ_R", "XY","Y");
    fBnames["SQX_L"] = FormBranchName("Si",2, "SQ_L", "XY","X");
    fBnames["SQY_L"] = FormBranchName("Si",2, "SQ_L", "XY","Y");
    fBnames["SQ300"] = FormBranchName("Si",1, "SQ300_L", "XY","X");
    fBnames["CsI_R"] = FormBranchName("CsI",-1,"CsI_R","","");
    fBnames["CsI_L"] = FormBranchName("CsI",-1,"CsI_L","","");

    fSiAmpTimeStations["SQX_R"] = "tSQX_R";
    fSiAmpTimeStations["SQY_R"] = "tSQY_R";
    fSiAmpTimeStations["SQX_L"] = "tSQX_L";
    fSiAmpTimeStations["SQY_L"] = "tSQY_L";
    fSiAmpTimeStations["SQ300"] = "tSQ300";

    fCsIStations.push_back("CsI_R");
    fCsIStations.push_back("CsI_L");

    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    for (auto itSt : stList){
        if (fSiAmpTimeStations.find(itSt.first) != fSiAmpTimeStations.end()){
            fDigiCollections[fBnames[itSt.first]] = new TClonesArray("ERQTelescopeSiDigi",1000);
            ioman->Register(fBnames[itSt.first],fDetName, fDigiCollections[fBnames[itSt.first]], kTRUE);
        }
        if (std::find(fCsIStations.begin(),fCsIStations.end(),itSt.first) != fCsIStations.end()){
            fDigiCollections[fBnames[itSt.first]] = new TClonesArray("ERQTelescopeCsIDigi",1000);
            ioman->Register(fBnames[itSt.first],fDetName, fDigiCollections[fBnames[itSt.first]], kTRUE);
        }
    }

    return kTRUE;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERTelescopeUnpack::DoUnpack(Int_t* data, Int_t size){
    if (!ERUnpack::DoUnpack(data,size))
        return kTRUE;

    DetEventFull* event = (DetEventFull*)data;

    DetEventDetector* detEvent = (DetEventDetector* )event->GetChild(fDetName);
    const std::map<TString, unsigned short> stList = fSetupConfiguration->GetStationList(fDetName);
    for (auto itSt : stList){
        TString ampStation = itSt.first;
        if (fSiAmpTimeStations.find(itSt.first) != fSiAmpTimeStations.end()){
            std::vector<Double_t> ampV, timeV;
            std::vector<Int_t> channelV;
            UnpackAmpTimeStation(detEvent, ampStation,fSiAmpTimeStations[ampStation],
                                    ampV, timeV, channelV);
            for (Int_t iChannel = 0; iChannel < channelV.size(); iChannel++)
                AddSiDigi(ampV[iChannel],timeV[iChannel],0,channelV[iChannel],fBnames[ampStation]);
        }
        if (std::find(fCsIStations.begin(),fCsIStations.end(),ampStation) != fCsIStations.end()){
            map<Int_t,Double_t> csiAmp;
            UnpackStation(detEvent,ampStation,csiAmp);
            for (auto itChannel : csiAmp){
                AddCsIDigi(itChannel.second,0.,-1,itChannel.first,fBnames[ampStation]);
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
ClassImp(ERTelescopeUnpack)