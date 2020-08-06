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
#include "ERPoint.h"

//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer()
  : ERDigitizer("ER telescope digitization")
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer(Int_t verbose)
  : ERDigitizer("ER telescope digitization ", verbose)
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
    if (bFullName.Contains("Point") && bFullName.Contains("Telescope")) {
      // Rename input point branches to output digi branches by changing class name prefix
      // ERDetectorPoint_sensitiveVolNameNumber -> ERDetectorDigi_sensitiveVolNameNumber
      // In map of output collections first parameter is full input branch name without class prefix 
      Int_t bPrefixNameLength = bFullName.First('_');
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopePoints[brName] = (TClonesArray*) ioman->GetObject(bFullName);
      TString bPrefixName(bFullName(0, bPrefixNameLength));
      bPrefixName.Replace(bPrefixNameLength - 5, bPrefixNameLength, "Digi");
      TString outBrName =  bPrefixName + "_" + brName;
      fQTelescopeDigi[brName] = new TClonesArray("ERDigi");
      LOG(DEBUG) << "Branch name " << brName << FairLogger::endl;
      ioman->Register(outBrName, "Telescope", fQTelescopeDigi[brName], kTRUE);
    }
  }
  return kSUCCESS;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeDigitizer::Exec(Option_t* opt) {
  Reset();
  for (const auto &itPointBranches : fQTelescopePoints) {
    Double_t  elossThreshold, timeThreshold;
    Double_t  elossSigma, timeSigma;
    std::map<Int_t, std::vector<Int_t>> sortedPoints;
    if (itPointBranches.first.Contains("Si")) {
      elossThreshold = fSiElossThreshold;
      elossSigma     = fSiElossSigma;
      timeSigma      = fSiTimeSigma;
    }
    if (itPointBranches.first.Contains("CsI")) {
      elossThreshold = fCsIElossThreshold;
      elossSigma     = fCsIElossSigma;
      timeSigma      = fCsITimeSigma;
    }
    for (Int_t iPoint = 0; iPoint < itPointBranches.second->GetEntriesFast(); iPoint++){
      ERPoint* point = (ERPoint*)(itPointBranches.second->At(iPoint));
      sortedPoints[point->GetVolNb()].push_back(iPoint);
    }
    for (const auto &itPoint : sortedPoints) {
      Float_t   edep = 0.; //sum edep in strip
      Float_t   time = std::numeric_limits<float>::max(); // min time in strip
      for (const auto itPointsForCurrentVolume : itPoint.second) {
        const auto* point = (ERPoint*)(itPointBranches.second->At(itPointsForCurrentVolume));
        edep += point->GetEnergyLoss();
        if (point->GetTime() < time) {
          time = point->GetTime();
        }
      }
      if (edep == 0) {  // if no points in input branch
        continue;
      }
      edep = gRandom->Gaus(edep, elossSigma);
      if (edep < elossThreshold)
        continue;
      time = gRandom->Gaus(time, timeSigma);
      auto* digi = AddDigi(edep, time, itPoint.first, itPointBranches.first);
      for (const auto itPointsForCurrentVolume : itPoint.second) {
        digi->AddLink(FairLink("ERPoint", itPointsForCurrentVolume));
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
ERDigi* ERQTelescopeDigitizer::AddDigi(Float_t edep, Double_t time, Int_t stripNb,
                                       TString digiBranchName) {
  ERDigi *digi = new((*fQTelescopeDigi[digiBranchName])
      [fQTelescopeDigi[digiBranchName]->GetEntriesFast()]) ERDigi(edep, time, stripNb);
  return digi;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeDigitizer)
