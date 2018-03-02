/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeDigitizer.h"

#include <limits>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLink.h"
#include "FairLogger.h"

#include "ERDetectorList.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer()
  : FairTask("ER qtelescope digitization"), 
  fSiElossSigma(0),
  fSiTimeSigma(0),
  fSiElossThreshold(0)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer(Int_t verbose)
  : FairTask("ER qtelescope digitization ", verbose),
  fSiElossSigma(0),
  fSiTimeSigma(0),
  fSiElossThreshold(0)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::~ERQTelescopeDigitizer()
{
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeDigitizer::SetParContainers() {
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
//-------------------------------------------------------------------------------------------------
InitStatus ERQTelescopeDigitizer::Init() {
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;
  vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    cout << "Branch full name " << bFullName << endl;
    if (bFullName.Contains("Point") && bFullName.Contains("QTelescope")) {
      // Rename input point branches to output digi branches by changing class name prefix
      // ERDetectorPoint_sensitiveVolNameNumber -> ERDetectorDigi_sensitiveVolNameNumber
      // In map of output collections first parameter is full input branch name without class prefix 
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopePoints[brName] = (TClonesArray*) ioman->GetObject(bFullName);
      TString bPrefixName(bFullName(0, bPrefixNameLength));     
      bPrefixName.Replace(bPrefixNameLength - 5, bPrefixNameLength, "Digi");
      TString outBrName =  bPrefixName + "_" + brName;
      fQTelescopeDigi[brName] = new TClonesArray(bPrefixName);
      cout << "Branch name " << brName << endl;
      ioman->Register(outBrName, "QTelescope", fQTelescopeDigi[brName], kTRUE);
    }
  }
  return kSUCCESS;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeDigitizer::Exec(Option_t* opt) {
  Reset();
  for (const auto &itPointBranches : fQTelescopePoints) {
    Float_t   edep = 0.; //sum edep in strip
    Float_t   time = numeric_limits<float>::max(); // min time in strip
    Int_t     pointType;
    Double_t  elossThreshold, timeThreshold;
    Double_t  elossSigma, timeSigma;
    map<Int_t, vector<Int_t>> sortedPoints;
    if (itPointBranches.first.Contains("Si")) {
      pointType = 0;  // Si point
      elossThreshold = fSiElossThreshold;
      elossSigma     = fSiElossSigma;
      timeSigma      = fSiTimeSigma;
      for (Int_t iPoint = 0; iPoint < itPointBranches.second->GetEntriesFast(); iPoint++){
        ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)(itPointBranches.second->At(iPoint));
        sortedPoints[point->GetStripNb()].push_back(iPoint);
      }
    }
    if (itPointBranches.first.Contains("CsI")) {
      pointType = 1;  // CsI point
      elossThreshold = fCsIElossThreshold;
      elossSigma     = fCsIElossSigma;
      timeSigma      = fCsITimeSigma;
      for (Int_t iPoint = 0; iPoint < itPointBranches.second->GetEntriesFast(); iPoint++){
        ERQTelescopeCsIPoint* point = (ERQTelescopeCsIPoint*)(itPointBranches.second->At(iPoint));
        sortedPoints[point->GetBlockNb()].push_back(iPoint);
      }
    }
    FairMCPoint* mcPoint;
    for (const auto &itPoint : sortedPoints) {
      edep = 0;
      for (const auto itPointsForCurrentVolume : itPoint.second) {
        if (pointType == 0) { // 0 - Si point, 1 - CsI point
          mcPoint = (ERQTelescopeSiPoint*)(itPointBranches.second->At(itPointsForCurrentVolume));
        }
        if (pointType == 1) { // 0 - Si point, 1 - CsI point
          mcPoint = (ERQTelescopeCsIPoint*)(itPointBranches.second->At(itPointsForCurrentVolume));
        }
        edep += mcPoint->GetEnergyLoss();
        if (mcPoint->GetTime() < time) {
          time = mcPoint->GetTime();
        }
      }
      if (edep == 0) {  // if no points in input branch
        continue;
      }
      edep = gRandom->Gaus(edep, elossSigma);
      if (edep < elossThreshold)
        continue;

      time = gRandom->Gaus(time, timeSigma);

      if (pointType == 0) { // 0 - Si point, 1 - CsI point
        ERQTelescopeSiPoint* siPoint = (ERQTelescopeSiPoint*)(itPointBranches.second->At(0));
        ERQTelescopeSiDigi* siDigi = AddSiDigi(edep, time, siPoint->GetStationNb(),
                                                           itPoint.first,
                                                           itPointBranches.first);
        for (const auto itPointsForCurrentVolume : itPoint.second) {
          siDigi->AddLink(FairLink("ERQTelescopeSiPoint", itPointsForCurrentVolume));
        }
      }
      if (pointType == 1) {
        ERQTelescopeCsIPoint* csiPoint = (ERQTelescopeCsIPoint*)(itPointBranches.second->At(0));
        ERQTelescopeCsIDigi* csiDigi = AddCsIDigi(edep, time, csiPoint->GetWallNb(),
                                                              itPoint.first,    
                                                              itPointBranches.first);
        for (const auto itPointsForCurrentVolume : itPoint.second) {
          csiDigi->AddLink(FairLink("ERQTelescopeCsIPoint", itPointsForCurrentVolume));
        }
      }   
    }
  }
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeDigitizer::Reset() {
  for (const auto itDigiBranches : fQTelescopeDigi) {
    if (itDigiBranches.second) {
      itDigiBranches.second->Delete();
    }
  }
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeDigitizer::Finish(){   
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi* ERQTelescopeDigitizer::AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, 
                                                                                  Int_t stripNb,
                                                                                  TString digiBranchName)
{
  ERQTelescopeSiDigi *digi = new((*fQTelescopeDigi[digiBranchName])
                                                  [fQTelescopeDigi[digiBranchName]->GetEntriesFast()])
              ERQTelescopeSiDigi(fQTelescopeDigi[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                     stationNb, 
                                                                                     stripNb);
  return digi;
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi* ERQTelescopeDigitizer::AddCsIDigi(Float_t edep, Double_t time, Int_t wallNb, 
                                                                                    Int_t blockNb,
                                                                                    TString digiBranchName)
{
  ERQTelescopeCsIDigi *digi = new((*fQTelescopeDigi[digiBranchName])
                                                   [fQTelescopeDigi[digiBranchName]->GetEntriesFast()])
              ERQTelescopeCsIDigi(fQTelescopeDigi[digiBranchName]->GetEntriesFast(), edep, time, 
                                                                                    wallNb, 
                                                                                    blockNb);
  return digi;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeDigitizer)
