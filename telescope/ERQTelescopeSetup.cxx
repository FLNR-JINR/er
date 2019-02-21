/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeSetup.h"

#include <ostream>

#include "TGeoTube.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TROOT.h"
#include <Riostream.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"

using namespace std;

ERQTelescopeSetup* ERQTelescopeSetup::fInstance = NULL;
map<TString, vector<ERQTelescopeStrip*>> ERQTelescopeSetup::fStrips;  // map<station,map<component, vector<strip>>>            
//--------------------------------------------------------------------------------------------------
ERQTelescopeStrip::ERQTelescopeStrip(Double_t globalX, Double_t globalY, Double_t globalZ,
                                     Double_t  localX, Double_t  localY, Double_t localZ) 
{
  fGlobalX = globalX; 
  fGlobalY = globalY; 
  fGlobalZ = globalZ;
  fLocalX = localX; 
  fLocalY = localY; 
  fLocalZ = localZ;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeStrip::ERQTelescopeStrip(Double_t* globTrans, Double_t* localTrans) {
  fGlobalX = globTrans[0]; 
  fGlobalY = globTrans[1]; 
  fGlobalZ = globTrans[2];
  fLocalX = localTrans[0]; 
  fLocalY = localTrans[1]; 
  fLocalZ = localTrans[2];
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup::ERQTelescopeSetup(): ERSetup() {
  LOG(DEBUG) << "ERQTelescopeSetup initialized! "<< FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup::~ERQTelescopeSetup() {
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup* ERQTelescopeSetup::Instance(){
  if (fInstance == NULL) {
    fInstance = new ERQTelescopeSetup();
    return fInstance;
  }
  else
    return fInstance;
}

//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripGlobalX(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fGlobalX;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripGlobalY(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fGlobalY;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripGlobalZ(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fGlobalZ;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripLocalX(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fLocalX;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripLocalY(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fLocalY;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripLocalZ(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fLocalZ;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::GetTransInMotherNode (TGeoNode const* node, Double_t b[3]) {
  memcpy(b, node->GetMatrix()->GetTranslation(), sizeof(double)*3);
}
// Написать несколько методов для работы с объемами:
// 1) Функция получения координаты в глобальной СК. (скорее всего функция в п.1) 
//    Проход от самого низкого дочернего объема до родительского внтри одной функции
// 2) 
// ------------------------------- -------------------------------------------------------------------
void ERQTelescopeSetup::ReadGeoParamsFromParContainer() {
  if ( ! gGeoManager ) {
    std::cerr << "ERQTelescopeSetup: cannot initialise without TGeoManager!"<< std::endl;
  }
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  TGeoNode* qtelescope  = NULL;
  TGeoNode* qtelescopeDetector = NULL;
  TGeoNode* qtelescopeStation = NULL;
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) { // cycle by volumes in TOP
    TString detectorName = cave->GetDaughter(iNode)->GetName();
    LOG(DEBUG) << "[ERQTelescopeSetup] detectorName " << detectorName << FairLogger::endl;
    if ( detectorName.Contains("QTelescope", TString::kIgnoreCase) ) {
      qtelescope = cave->GetDaughter(iNode); 
      std::cout << "name " << cave->GetDaughter(iNode)->GetName() << "; NbDaasd " << cave->GetDaughter(iNode)->GetNdaughters() << std::endl;
      for (Int_t iDetector = 0; iDetector < qtelescope->GetNdaughters(); iDetector++) { // cycle by subassemblies in QTelescope
        qtelescopeDetector = qtelescope->GetDaughter(iDetector);
        TString stationName = qtelescopeDetector->GetName();
        for (Int_t iStation = 0; iStation < qtelescopeDetector->GetNdaughters(); iStation++) { // cycle by components in station
          qtelescopeStation = qtelescopeDetector->GetDaughter(iStation);
          TString qtelescopeStationName = qtelescopeStation->GetName();
          Double_t boxInStripTrans[3];
          Double_t stripInStationTrans[3];
          Double_t componentInDetectorTrans[3];
          Double_t stripInDetectorTrans[3];
          Double_t stripGlobTrans[3];
          LOG(DEBUG) << "[ERQTelescopeSetup] qtelescopeStationName " << qtelescopeStationName << FairLogger::endl;
          if (qtelescopeStationName.Contains("DoubleSi", TString::kIgnoreCase) ) {
            TGeoNode* doubleSiStrip;
            TString firstStripArrayName, secondStripArrayName;
            if (qtelescopeStationName.Contains("XY")) {
              firstStripArrayName = qtelescopeStationName + "_X";
              secondStripArrayName = qtelescopeStationName + "_Y";
            } else {
              firstStripArrayName = qtelescopeStationName + "_Y";
              secondStripArrayName = qtelescopeStationName + "_X";              
            }
            Bool_t    flagFirstStripReaded = kFALSE;
            Int_t     iDoubleSiStrip = 0;
            for (; iDoubleSiStrip < qtelescopeStation->GetNdaughters(); iDoubleSiStrip++) {
              doubleSiStrip = qtelescopeStation->GetDaughter(iDoubleSiStrip);
              GetTransInMotherNode(doubleSiStrip, stripInStationTrans);
              // Now we assume that QTelescope center in the global zero.
              // It is neccesary to write clear function to find global coordinates
              // by all forefathers nodes if possible. Maybe with some FairRoot methods
              qtelescopeDetector->LocalToMaster(stripInStationTrans, componentInDetectorTrans);
              qtelescope->LocalToMaster(componentInDetectorTrans, stripGlobTrans);
              // qtelescope->LocalToMaster(stripInDetectorTrans, stripGlobTrans);
              // std::cout << "Through master Station Local " << stripInStationTrans[0] << ", "  
              //                                << stripInStationTrans[1] << ", "
              //                                << stripInStationTrans[2] << endl;
              // std::cout << "Through master Station Global " << stripGlobTrans[0] << ", "  
              //                               << stripGlobTrans[1] << ", "
              //                               << stripGlobTrans[2] << endl;
              // exit(0);
              fStrips[firstStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans, stripInStationTrans));
              TGeoNode* doubleSiBox;
              Int_t iDoubleSiBox = 0;
              if (!flagFirstStripReaded) {
                for (; iDoubleSiBox < doubleSiStrip->GetNdaughters(); iDoubleSiBox++) {
                  doubleSiBox = doubleSiStrip->GetDaughter(iDoubleSiBox);
                  TString siBoxName = doubleSiBox->GetName();
                  GetTransInMotherNode(doubleSiBox, boxInStripTrans);
                  // doubleSiStrip->LocalToMaster(boxInStripTrans, stripInStationTrans);
                  (qtelescopeStationName.Contains("XY")) ? stripInStationTrans[0] = 0
                                                           : stripInStationTrans[1] = 0;
                  qtelescopeStation->LocalToMaster(boxInStripTrans, stripInStationTrans);
                  qtelescopeDetector->LocalToMaster(stripInStationTrans, componentInDetectorTrans);
                  qtelescope->LocalToMaster(componentInDetectorTrans, stripGlobTrans);
                  fStrips[secondStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans, boxInStripTrans));
                  // std::cout << "DoubleSi back side glob " << stripGlobTrans[0] << ", "  
                  //                                         << stripGlobTrans[1] << ", "
                  //                                         << stripGlobTrans[2] << endl;
                  // // std::cout << "DoubleSi back side  loc " << stripInDetectorTrans[0] << ", "  
                  // //                                         << stripInDetectorTrans[1] << ", "
                  // //                                         << stripInDetectorTrans[2] << endl;
                  // std::cout << "DoubleSi back side  loc " << componentInDetectorTrans[0] << ", "  
                  //                                         << componentInDetectorTrans[1] << ", "
                  //                                         << componentInDetectorTrans[2] << endl;
                  // doubleSiBox->GetMotherVolume()->SetCurrentPoint(0, 0, 0);
                  // std::cout << gGeoManager->GetMaxLevels() << std::endl;
                  // gGeoManager->CdDown(2);
                  // std::cout << gGeoManager->GetPath() << std::endl;
                  // exit(0);
                }
                flagFirstStripReaded = kTRUE;
              }
            }
          }
          if (qtelescopeStationName.Contains("SingleSi", TString::kIgnoreCase) ) {
            TGeoNode* singleSiStrip;
            Int_t     iSingleSiStrip = 0;
            for (; iSingleSiStrip < qtelescopeStation->GetNdaughters(); iSingleSiStrip++) {
              singleSiStrip = qtelescopeStation->GetDaughter(iSingleSiStrip);
              GetTransInMotherNode(singleSiStrip, stripInStationTrans);
              qtelescopeDetector->LocalToMaster(stripInStationTrans, componentInDetectorTrans);
              qtelescope->LocalToMaster(componentInDetectorTrans, stripGlobTrans);
              // std::cout << "SingleSi glob " << stripGlobTrans[0] << ", "  
              //                               << stripGlobTrans[1] << ", "
              //                               << stripGlobTrans[2] << endl;
              // std::cout << "SingleSi  loc " << stripInDetectorTrans[0] << ", "  
              //                               << stripInDetectorTrans[1] << ", "
              //                               << stripInDetectorTrans[2] << endl;
              fStrips[qtelescopeStationName].push_back(new ERQTelescopeStrip(stripGlobTrans, stripInStationTrans));            
            }
          }
        }
      }
    }
  }
  // exit(0);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeSetup)
