/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeDigitizer.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLink.h"
#include "FairLogger.h"

#include "ERDetectorList.h"

//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer()
  : ERDigitizer("ER qtelescope digitization")
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer(Int_t verbose)
  : ERDigitizer("ER qtelescope digitization ", verbose)
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
//-------------------------------------------------------------------------------------------------
InitStatus ERQTelescopeDigitizer::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;
  std::vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    LOG(DEBUG) << "Branch full name " << bFullName << FairLogger::endl;
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
      LOG(DEBUG) << "Branch name " << brName << FairLogger::endl;
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
    Float_t   time = std::numeric_limits<float>::max(); // min time in strip
    Int_t     pointType;
    Double_t  elossThreshold, timeThreshold;
    Double_t  elossSigma, timeSigma;
    std::map<Int_t, std::vector<Int_t>> sortedPoints;
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
        ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)(itPointBranches.second->At(iPoint));
        sortedPoints[point->GetStripNb()].push_back(iPoint);
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
          mcPoint = (ERQTelescopeSiPoint*)(itPointBranches.second->At(itPointsForCurrentVolume));
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
        ERQTelescopeSiDigi* siDigi = AddSiDigi(edep, time, itPoint.first, itPointBranches.first);
        for (const auto itPointsForCurrentVolume : itPoint.second) {
          siDigi->AddLink(FairLink("ERQTelescopeSiPoint", itPointsForCurrentVolume));
        }
      }
      if (pointType == 1) {
        ERQTelescopeSiPoint* csiPoint = (ERQTelescopeSiPoint*)(itPointBranches.second->At(0));
        ERQTelescopeCsIDigi* csiDigi = AddCsIDigi(edep, time, itPoint.first,itPointBranches.first);
        for (const auto itPointsForCurrentVolume : itPoint.second) {
          csiDigi->AddLink(FairLink("ERQTelescopeSiPoint", itPointsForCurrentVolume));
        }
      }
    }
  }

  /*@TODO: This functionality can be transferred to ERDigitizer if the information 
  about the conformity of the trigger station and the digi collection moves there.*/
  for ( const auto &itDigiBranch : fQTelescopeDigi ) {
    TString branchName = itDigiBranch.first;
    TClonesArray* digiCol = itDigiBranch.second;
    for ( const auto &trigger : fTriggers) {
      TString triggerStation = trigger.first;
      if (branchName.Contains(triggerStation)){
        LOG(DEBUG) << "Apply trigger to station " << triggerStation << FairLogger::endl;
        ApplyTrigger(triggerStation,digiCol);
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
ERQTelescopeSiDigi* ERQTelescopeDigitizer::AddSiDigi(Float_t edep, Double_t time, Int_t stripNb,
                                                    TString digiBranchName)
{
  ERQTelescopeSiDigi *digi = new((*fQTelescopeDigi[digiBranchName])
      [fQTelescopeDigi[digiBranchName]->GetEntriesFast()]) 
      ERQTelescopeSiDigi(edep, time, stripNb);
  return digi;
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi* ERQTelescopeDigitizer::AddCsIDigi(Float_t edep, Double_t time, Int_t blockNb,
                                                       TString digiBranchName)
{
  ERQTelescopeCsIDigi *digi = new((*fQTelescopeDigi[digiBranchName])
      [fQTelescopeDigi[digiBranchName]->GetEntriesFast()])
      ERQTelescopeCsIDigi(edep, time, blockNb);
  return digi;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeDigitizer)
