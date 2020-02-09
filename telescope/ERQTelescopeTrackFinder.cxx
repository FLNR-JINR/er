/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeTrackFinder.h"

#include <cassert>

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "FairLink.h"

#include "ERBeamDetTrack.h"
#include "ERRunAna.h"

//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::ERQTelescopeTrackFinder()
  : ERTask("ER qtelescope track finding scheme"),
    fUserTargetPointIsSet(kFALSE),
    fBeamDetTrack(NULL)
    //@Todo инициализация всех переменных
{
  fAvailibleRunManagers.push_back("ERRunAna");
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::ERQTelescopeTrackFinder(Int_t verbose)
  : ERTask("ER qtelescope track finding scheme ", verbose),
    fUserTargetPointIsSet(kFALSE),
    fBeamDetTrack(NULL)
{
  fAvailibleRunManagers.push_back("ERRunAna");
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrackFinder::~ERQTelescopeTrackFinder() {
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetHitStation(TString subassemblyName, TString componentId) {
  TString xStripArrayName = componentId;
  TString yStripArrayName = componentId;
  fSiHitStationsPair[subassemblyName].emplace(make_pair(componentId, 
                                       pair<TString, TString>(xStripArrayName.Append("_X"), 
                                                              yStripArrayName.Append("_Y"))));
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
  std::vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    if (bFullName.Contains("Digi") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
    }
  }
  // Register output track branches only for stations that are setted by interface SetStation(){
  for (const auto itSubassemblies : fSiHitStationsPair) {
    for (const auto itComponent : itSubassemblies.second) {
      fQTelescopeTrack[itComponent.first] = new TClonesArray("ERQTelescopeTrack");
      ioman->Register("ERQTelescopeTrack_" + itComponent.first, "QTelescope", 
                      fQTelescopeTrack[itComponent.first], kTRUE);
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

  fQTelescopeSetup->ERQTelescopeSetup::ReadGeoParamsFromParContainer();

  //@TODO check setup and digi branch names
  
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::Exec(Option_t* opt) { 
  Reset();
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
  for (const auto& itSubassemblies : fSiHitStationsPair) {
    for (const auto& itComponent : itSubassemblies.second) {
      // pairs of X and Y strips that have difference between edep less than fEdepDiffXY 
      std::vector<std::pair<Int_t, Int_t>>  hitTelescopePoint;
      // strips with edep in correct interval (fSiDigiEdepMin, fSiDigiEdepMax)     
      std::vector<Int_t> correctStripsX;
      // strips with edep in correct interval (fSiDigiEdepMin, fSiDigiEdepMax)          
      std::vector<Int_t> correctStripsY;
      const TString xDigiBranchName = itComponent.second.first;
      const TString yDigiBranchName = itComponent.second.second;  
      const TClonesArray* xDigi = fQTelescopeDigi[xDigiBranchName];
      const TClonesArray* yDigi = fQTelescopeDigi[yDigiBranchName];
      if ( !xDigi || !yDigi) {
        continue;
      }
      if (xDigi->GetEntriesFast() == 0 || yDigi->GetEntriesFast()==0) {
        continue;
      }
      for (Int_t iXDigi  = 0; iXDigi < xDigi->GetEntriesFast(); iXDigi++) {
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(iXDigi))->GetEdep();
	LOG(DEBUG) << "xStripEdep " << xStripEdep << "; iXDigi " << iXDigi << FairLogger::endl;
        if (xStripEdep > fSiDigiEdepMin && xStripEdep < fSiDigiEdepMax) {
          correctStripsX.push_back(iXDigi);
        }
      }
      for (Int_t iYDigi  = 0; iYDigi < yDigi->GetEntriesFast(); iYDigi++) {
        const Double_t yStripEdep = ((ERQTelescopeSiDigi*)yDigi->At(iYDigi))->GetEdep();
	LOG(DEBUG) << "yStripEdep " << yStripEdep << "; iYDigi " << iYDigi << FairLogger::endl;
        if (yStripEdep > fSiDigiEdepMin && yStripEdep < fSiDigiEdepMax) {
          correctStripsY.push_back(iYDigi);
        }
      }
      for (const auto itCorrectStripsX : correctStripsX) {
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(itCorrectStripsX))->GetEdep();
        for (const auto itCorrectStripsY : correctStripsY) {
          const Double_t yStripEdep = ((ERQTelescopeSiDigi*)yDigi->At(itCorrectStripsY))->GetEdep();
          if (TMath::Abs(xStripEdep - yStripEdep) < fEdepDiffXY) {
            hitTelescopePoint.push_back(pair<Int_t, Int_t>(itCorrectStripsX, itCorrectStripsY));
          }
        }
      }
      LOG(DEBUG) << "Strips array pair " << itComponent.second.first << " " 
                                         << itComponent.second.second << FairLogger::endl;
      LOG(DEBUG) << "Hits count on pair " << hitTelescopePoint.size() << FairLogger::endl;
      for (const auto& itHitPoint : hitTelescopePoint) {
        const auto xStripIndex = itHitPoint.first;
        const auto yStripIndex = itHitPoint.second;
        const auto* xStrip = dynamic_cast<ERQTelescopeSiDigi*>(xDigi->At(xStripIndex));
        const auto* yStrip = dynamic_cast<ERQTelescopeSiDigi*>(yDigi->At(yStripIndex));
        if (!xStrip || !yStrip)
          continue;
        const auto xStripNb = xStrip->GetStripNb();
        const auto yStripNb = yStrip->GetStripNb();
        LOG(DEBUG) << "  Branch names X:" << xDigiBranchName 
                   << " Y: " << yDigiBranchName << FairLogger::endl;
        LOG(DEBUG) << "  Strips pair " << xStripIndex << " " << yStripIndex << FairLogger::endl;
        LOG(DEBUG) << "  Strips pair numbers " << xStripNb << " " << yStripNb << FairLogger::endl;
        // Calc unknown coordinated using condition: target, hit on first station(closest) and
        // hit on second station lie on line :
        // {x1, y1, z1} = {fTargetX, fTargetY, fTargetZ} + k * ({x1, y1, z1} - {fTargetX, fTargetY, fTargetZ}).
        const bool xStationIsClosest = fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xStripNb)
                                       < fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yStripNb);
        // We know all about z coordinate, so 
        const double z1 = xStationIsClosest 
                          ? fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xStripNb)
                          : fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yStripNb);
        const double z2 =  xStationIsClosest 
                          ? fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yStripNb)
                          : fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xStripNb);
        assert(z1 != fTargetZ);
        const double k = (z2 - fTargetZ) / (z1 - fTargetZ);
        double x1 = 0., x2 = 0., y1 = 0., y2 = 0.;
	LOG(DEBUG) << "xStationIsClosest " << xStationIsClosest << FairLogger::endl;
        if (xStationIsClosest) { // find y1, x2 from equation
          x1 = fQTelescopeSetup->GetStripGlobalX(xDigiBranchName, xStripNb);
          y2 = fQTelescopeSetup->GetStripGlobalY(yDigiBranchName, yStripNb);
          y1 = (-1./k)*((1. - k)*fTargetY - y2);
          x2 = (1. - k)*fTargetX + k*x1;
        } else { // find x1, y2 from equation
          x2 = fQTelescopeSetup->GetStripGlobalX(xDigiBranchName, xStripNb);
          y1 = fQTelescopeSetup->GetStripGlobalY(yDigiBranchName, yStripNb);
          x1 = (-1./k)*((1. - k)*fTargetX - x2);
          y2 = (1. - k)*fTargetY + k*y1;
        }
	LOG(DEBUG) << "[ERQTelescopeTrack] x1, y1 " << x1 << " " << y1 << FairLogger::endl;
	LOG(DEBUG) << "[ERQTelescopeTrack] x2, y2 " << x2 << " " << y2 << FairLogger::endl;
        const auto xQTeleGlobHit = (x1 + x2) / 2.;
        const auto yQTeleGlobHit = (y1 + y2) / 2.;
        const auto zQTeleGlobHit = (z1 + z2) / 2.;
        const auto xQTeleLocalHit = fQTelescopeSetup->GetStripLocalX(xDigiBranchName, xStripNb);
        const auto yQTeleLocalHit = fQTelescopeSetup->GetStripLocalY(yDigiBranchName, yStripNb);
        const auto zQTeleLocalHit = (fQTelescopeSetup->GetStripLocalZ(xDigiBranchName, xStripNb)
                                     + fQTelescopeSetup->GetStripLocalZ(yDigiBranchName, yStripNb))/2.;
        LOG(DEBUG) << " Local hit X Y Z " << xQTeleLocalHit << " " << yQTeleLocalHit 
                   << " " << zQTeleLocalHit << FairLogger::endl;
        LOG(DEBUG) << " Global hit X Y Z " << xQTeleGlobHit << " " << yQTeleGlobHit 
                   << " " << zQTeleGlobHit << FairLogger::endl;
        
        //Double_t sumEdep = yStrip->GetEdep();        
        const Double_t sumEdep = xStrip->GetEdep();        
        auto* track = AddTrack(fTargetX, fTargetY, fTargetZ, 
                                            xQTeleGlobHit,  yQTeleGlobHit,  zQTeleGlobHit,                                            
                                            xQTeleLocalHit, yQTeleLocalHit, zQTeleLocalHit,
                                            sumEdep,
                                            itComponent.first);
        track->AddLink(FairLink(xDigiBranchName, xStripIndex));
        track->AddLink(FairLink(yDigiBranchName, yStripIndex));
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::Reset() {
  for (const auto itTrackBranches : fQTelescopeTrack) {
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
                                                     TString digiBranchName) 
{
  ERQTelescopeTrack *track = new((*fQTelescopeTrack[digiBranchName])
                                                   [fQTelescopeTrack[digiBranchName]->GetEntriesFast()])
                             ERQTelescopeTrack( targetX, targetY, targetZ, 
                                                globalX, globalY, globalZ,
                                                localX,  localY,  localZ,
                                                sumEdep);
  return track;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrackFinder)
