/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeTrackFinder.h"

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "FairLink.h"

#include "ERBeamDetTrack.h"
#include "ERRunAna.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::ERQTelescopeTrackFinder()
  : ERTask("ER qtelescope track finding scheme"),
    fUserTargetPointIsSet(kFALSE),
    fBeamDetTrack(NULL)
    //@Todo инициализация всех переменных
{
  fAvailibleRunManagers.push_back("ERRunAna");
  fQTelSetup = ERQTelescopeSetup::Instance();
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::ERQTelescopeTrackFinder(Int_t verbose)
  : ERTask("ER qtelescope track finding scheme ", verbose),
    fUserTargetPointIsSet(kFALSE),
    fBeamDetTrack(NULL)
{
  fAvailibleRunManagers.push_back("ERRunAna");
  fQTelSetup = ERQTelescopeSetup::Instance();
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::~ERQTelescopeTrackFinder() {
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetHitStation(TString subassemblyName, TString componentId) {
  TString xStrArrayName = componentId;
  TString yStrArrayName = componentId;
  fSiHitStationsPair[subassemblyName].emplace(make_pair(componentId, 
                                       pair<TString, TString>(xStrArrayName.Append("_X"), 
                                                              yStrArrayName.Append("_Y"))));
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetHitStation(TString subassemblyName, TString componentIdX,
                                                                     TString componentIdY) 
{
  fSiHitStationsPair[subassemblyName].emplace(make_pair(componentIdX + componentIdY, 
                                              pair<TString, TString>(componentIdX, componentIdY)));
}//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetStripEdepRange(Double_t edepMin, Double_t edepMax) {
  fSiDigiEdepMin = edepMin; 
  fSiDigiEdepMax = edepMax;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetTargetPoint(Double_t x, Double_t y, Double_t z) {
  fUserTargetPointIsSet = kTRUE;
  fTargetX = x;
  fTargetY = y;
  fTargetZ = z;
}
//--------------------------------------------------------------------------------------------------
InitStatus ERQTelescopeTrackFinder::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;

  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;
  vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    if (bFullName.Contains("Digi") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
    }
  }
  // Register output track branches only for stations that are setted by interface SetStation(){
  for (const auto itSubassemblies : fSiHitStationsPair) {
    for (const auto itComponent : itSubassemblies.second) {
      fQTelTrack[itComponent.first] = new TClonesArray("ERQTelescopeTrack");
      ioman->Register("ERQTelescopeTrack_" + itComponent.first, "QTelescope", 
                      fQTelTrack[itComponent.first], kTRUE);
    }
  }

  fBeamDetTrack = (TClonesArray*) ioman->GetObject("BeamDetTrack");   
  if (!fUserTargetPointIsSet) {
    if (!fBeamDetTrack) {
      LOG(DEBUG) << "ERQTelescopeTrackFinder: target point not initialized by user " 
                 << "(by means of SetTargetPoint()) and there is no ERBeamDetTrack branch" 
                 <<FairLogger::endl;
      return kFATAL;
    } 
  }

  fQTelSetup->ERQTelescopeSetup::ReadGeoParamsFromParContainer();

  //@TODO check setup and digi branch names
  
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::Exec(Option_t* opt) { 
  Reset();
  for (const auto itSubassemblies : fSiHitStationsPair) {
    for (const auto itComponent : itSubassemblies.second) {
      vector<pair<Int_t, Int_t>>  hitTelescopePoint;    // pairs of X and Y strips that have difference between edep less than fEdepDiffXY 
      vector<Int_t>               correctStrsX;       // strips with edep in correct interval (fSiDigiEdepMin, fSiDigiEdepMax)     
      vector<Int_t>               correctStrsY;       // strips with edep in correct interval (fSiDigiEdepMin, fSiDigiEdepMax)
      TString xDigiBrName = itComponent.second.first;
      TString yDigiBrName = itComponent.second.second;  
      TClonesArray *xDigi = fQTelDigi[xDigiBrName];
      TClonesArray *yDigi = fQTelDigi[yDigiBrName];
      if ( !xDigi || !yDigi) {
        continue;
      }
      if (xDigi->GetEntriesFast() == 0 || yDigi->GetEntriesFast()==0) {
        continue;
      }

      for (Int_t iXDigi  = 0; iXDigi < xDigi->GetEntriesFast(); iXDigi++) {
        Double_t xStrEdep = ((ERQTelescopeSiDigi*)xDigi->At(iXDigi))->GetEdep();
        if (xStrEdep > fSiDigiEdepMin && xStrEdep < fSiDigiEdepMax) {
          correctStrsX.push_back(iXDigi);
        }
      }
      for (Int_t iYDigi  = 0; iYDigi < yDigi->GetEntriesFast(); iYDigi++) {
        Double_t yStrEdep = ((ERQTelescopeSiDigi*)yDigi->At(iYDigi))->GetEdep();
        if (yStrEdep > fSiDigiEdepMin && yStrEdep < fSiDigiEdepMax) {
          correctStrsY.push_back(iYDigi);
        }
      }
      for (const auto itCorrectStrsX : correctStrsX) {
        Double_t xStrEdep = ((ERQTelescopeSiDigi*)xDigi->At(itCorrectStrsX))->GetEdep();
        for (const auto itCorrectStrsY : correctStrsY) {
          Double_t yStrEdep = ((ERQTelescopeSiDigi*)yDigi->At(itCorrectStrsY))->GetEdep();
          if (TMath::Abs(xStrEdep - yStrEdep) < fEdepDiffXY) {
            hitTelescopePoint.push_back(pair<Int_t, Int_t>(itCorrectStrsX, itCorrectStrsY));
          }
        }
      }
      LOG(DEBUG) << "Strs array pair " << itComponent.second.first << " " 
                                         << itComponent.second.second << FairLogger::endl;
      LOG(DEBUG) << "Hits count on pair " << hitTelescopePoint.size() << FairLogger::endl;
      for (auto &itHitPoint : hitTelescopePoint) {
        ERQTelescopeSiDigi* xStr = ((ERQTelescopeSiDigi*)xDigi->At(itHitPoint.first));
        ERQTelescopeSiDigi* yStr = ((ERQTelescopeSiDigi*)yDigi->At(itHitPoint.second));

        Int_t xStrNb = xStr->GetStripNb();
        Int_t yStrNb = yStr->GetStripNb();
        LOG(DEBUG) << "  Branch names " << xDigiBrName << " " << yDigiBrName << FairLogger::endl;
        LOG(DEBUG) << "  Stips pair " << itHitPoint.first << " " << itHitPoint.second << FairLogger::endl;
        LOG(DEBUG) << "  Stips pair numbers " << xStrNb << " " << yStrNb << FairLogger::endl;
        Double_t xQTeleGlobHit = ((ERQTelescopeSetup*)fQTelSetup)->GetStripGlobalX(xDigiBrName, xStrNb);
        Double_t yQTeleGlobHit = ((ERQTelescopeSetup*)fQTelSetup)->GetStripGlobalY(yDigiBrName, yStrNb);
        Double_t zQTeleGlobHitXstrip = ((ERQTelescopeSetup*)fQTelSetup)->GetStripGlobalZ(xDigiBrName, xStrNb);
        Double_t zQTeleGlobHitYstrip = ((ERQTelescopeSetup*)fQTelSetup)->GetStripGlobalZ(yDigiBrName, yStrNb);
	      Double_t zQTeleGlobHit = std::max(zQTeleGlobHitXstrip, zQTeleGlobHitYstrip);
	      Double_t xQTeleLocalHit = ((ERQTelescopeSetup*)fQTelSetup)->GetStripLocalX(xDigiBrName, xStrNb);
	      // local coordinates are crossed for X and Y coordinates in left and right 
	      // assign parts due to X-orientation by default
        Double_t yQTeleLocalHit = ((ERQTelescopeSetup*)fQTelSetup)->GetStripLocalX(yDigiBrName, yStrNb);
        Double_t zQTeleLocalHit = 0;	

        LOG(DEBUG) << "Telescope local hit X Y Z " 
		               << xQTeleLocalHit << " " 
		               << yQTeleLocalHit << " " 
		               << zQTeleLocalHit << FairLogger::endl;
        LOG(DEBUG) << "Telescope global hit X Y Z " 
		               << xQTeleGlobHit << " " 
		               << yQTeleGlobHit << " " 
		               << zQTeleGlobHit << FairLogger::endl;

        if (!fUserTargetPointIsSet) {
          ERBeamDetTrack* trackFromMWPC = (ERBeamDetTrack*)fBeamDetTrack->At(0);
          if (!trackFromMWPC) {
            fRun->MarkFill(kFALSE);
            return ;
          }
          fTargetX = trackFromMWPC->GetTargetX();
          fTargetY = trackFromMWPC->GetTargetY();
          fTargetZ = trackFromMWPC->GetTargetZ();
        }
	      Double_t sumEdep = (zQTeleGlobHitXstrip > zQTeleGlobHitYstrip) ? xStr->GetEdep() 
                                                                       : yStr->GetEdep();
        ERQTelescopeTrack *track = AddTrack(fTargetX, fTargetY, fTargetZ,
                                            xQTeleGlobHit,  yQTeleGlobHit,  zQTeleGlobHit, 
                                            xQTeleLocalHit, yQTeleLocalHit, zQTeleLocalHit,
                                            sumEdep,
                                            itComponent.first);
        track->AddLink(FairLink(xDigiBrName,itHitPoint.first));
        track->AddLink(FairLink(yDigiBrName,itHitPoint.second));
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::Reset() {
  for (const auto itTrackBranches : fQTelTrack) {
    if (itTrackBranches.second) {
      itTrackBranches.second->Delete();
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::Finish() {   
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrack* ERQTelescopeTrackFinder::AddTrack(Double_t targetX, Double_t targetY,   
                                                     Double_t targetZ,  
                                                     Double_t globalX, Double_t globalY, 
                                                     Double_t globalZ,
                                                     Double_t localX,  Double_t  localY, 
                                                     Double_t localZ,
                                                     Double_t sumEdep,
                                                     TString digiBrName) 
{
  ERQTelescopeTrack *track = new((*fQTelTrack[digiBrName])
                                  [fQTelTrack[digiBrName]->GetEntriesFast()])
                             ERQTelescopeTrack( targetX, targetY, targetZ, 
                                                globalX, globalY, globalZ,
                                                localX,  localY,  localZ,
                                                sumEdep);
  return track;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrackFinder)
