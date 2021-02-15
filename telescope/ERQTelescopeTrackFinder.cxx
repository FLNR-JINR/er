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
  fSiHitStationsPair[subassemblyName].emplace(
      std::make_pair(componentId, std::pair<TString, TString>(xStripArrayName.Append("_X"), 
                                                              yStripArrayName.Append("_Y"))));
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::SetHitStation(TString subassemblyName, TString componentIdX,
                                                                     TString componentIdY) 
{
  fSiHitStationsPair[subassemblyName].emplace(
      std::make_pair(componentIdX + componentIdY, std::pair<TString, TString>(componentIdX, componentIdY)));
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
void ERQTelescopeTrackFinder::
SetTrackPositionCorrection(const TString& station_name, unsigned int channel, float strip_fraction) {
  if (strip_fraction < -0.5 || strip_fraction > 0.5) {
    LOG(FATAL) << "Correction of track position in strip should be in range [-0.5, 0.5]"
               << FairLogger::endl;
  }
  track_position_corrections_[station_name][channel] = strip_fraction;
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
  LOG(DEBUG) << "[ERQTelescopeTrackFinder]------------Started--------------------------------------"
             << FairLogger::endl;
  Reset();
  if (!fUserTargetPointIsSet) {
      ERBeamDetTrack* trackFromMWPC = (ERBeamDetTrack*)fBeamDetTrack->At(0);
      if (!trackFromMWPC) {
        //fRun->MarkFill(kFALSE);
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
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(iXDigi))->Edep();
        if (xStripEdep > fSiDigiEdepMin && xStripEdep < fSiDigiEdepMax) {
          correctStripsX.push_back(iXDigi);
        }
      }
      for (Int_t iYDigi  = 0; iYDigi < yDigi->GetEntriesFast(); iYDigi++) {
        const Double_t yStripEdep = ((ERQTelescopeSiDigi*)yDigi->At(iYDigi))->Edep();
        if (yStripEdep > fSiDigiEdepMin && yStripEdep < fSiDigiEdepMax) {
          correctStripsY.push_back(iYDigi);
        }
      }
      for (const auto itCorrectStripsX : correctStripsX) {
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(itCorrectStripsX))->Edep();
        for (const auto itCorrectStripsY : correctStripsY) {
          const Double_t yStripEdep = ((ERQTelescopeSiDigi*)yDigi->At(itCorrectStripsY))->Edep();
          if (TMath::Abs(xStripEdep - yStripEdep) < fEdepDiffXY) {
            hitTelescopePoint.push_back(std::pair<Int_t, Int_t>(itCorrectStripsX, itCorrectStripsY));
          }
        }
      }
      LOG(DEBUG) << "[ERQTelescopeTrackFinder] Strips array pair " << itComponent.second.first << " " 
                                         << itComponent.second.second << FairLogger::endl;
      LOG(DEBUG) << "[ERQTelescopeTrackFinder] Hits count on pair " << hitTelescopePoint.size() << FairLogger::endl;
      for (const auto& itHitPoint : hitTelescopePoint) {
        const auto xStripIndex = itHitPoint.first;
        const auto yStripIndex = itHitPoint.second;
        const auto* xStrip = dynamic_cast<ERQTelescopeSiDigi*>(xDigi->At(xStripIndex));
        const auto* yStrip = dynamic_cast<ERQTelescopeSiDigi*>(yDigi->At(yStripIndex));
        if (!xStrip || !yStrip)
          continue;
        const auto xChannel = xStrip->Channel();
        const auto yChannel = yStrip->Channel();
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] Branch names X:" << xDigiBranchName 
                   << " Y: " << yDigiBranchName << FairLogger::endl;
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] Strips pair numbers " << xChannel << " " 
                   << yChannel << FairLogger::endl;
        // Calc unknown coordinated using condition: target, hit on first station(closest) and
        // hit on second station lie on line :
        // {x1, y1, z1} = {fTargetX, fTargetY, fTargetZ} + k * ({x1, y1, z1} - {fTargetX, fTargetY, fTargetZ}).
        const bool xStationIsClosest = fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xChannel)
                                       < fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yChannel);
        // We know all about z coordinate, so 
        const double z1 = xStationIsClosest 
                          ? fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xChannel)
                          : fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yChannel);
        const double z2 =  xStationIsClosest 
                          ? fQTelescopeSetup->GetStripGlobalZ(yDigiBranchName, yChannel)
                          : fQTelescopeSetup->GetStripGlobalZ(xDigiBranchName, xChannel);
        assert(z1 != fTargetZ);
        const double k = (z2 - fTargetZ) / (z1 - fTargetZ);
        double x1 = 0., x2 = 0., y1 = 0., y2 = 0.;
        
        if (xStationIsClosest) { // find y1, x2 from equation
          x1 = GetGlobalTrackPositionByStrip(xDigiBranchName, xChannel)[0];
          y2 = GetGlobalTrackPositionByStrip(yDigiBranchName, yChannel)[1];
          LOG(DEBUG) << "[ERQTelescopeTrackFinder] Coordinates from strips. x1 = " << x1 
                     << " y2 = " << y2 << " z1 = " << z1 << " z2 = " << z2 << FairLogger::endl;
          y1 = (-1./k)*((1. - k)*fTargetY - y2);
          x2 = (1. - k)*fTargetX + k*x1;
        } else { // find x1, y2 from equation
          x2 = GetGlobalTrackPositionByStrip(xDigiBranchName, xChannel)[0];
          y1 = GetGlobalTrackPositionByStrip(yDigiBranchName, yChannel)[1];
          LOG(DEBUG) << "[ERQTelescopeTrackFinder] Coordinates from strips. x2 = " << x2 
                     << " y1 = " << y1 << " z1 = " << z1 << " z2 = " << z2 << FairLogger::endl;
          x1 = (-1./k)*((1. - k)*fTargetX - x2);
          y2 = (1. - k)*fTargetY + k*y1;
        }
        const auto& xStationVertex = xStationIsClosest ? TVector3(x1, y1, z1) : TVector3(x2, y2, z2);
        const auto& yStationVertex = xStationIsClosest ? TVector3(x2, y2, z2) : TVector3(x1, y1, z1);
        const auto& xStationLocalVertex = fQTelescopeSetup->ToStationCoordinateSystem(xDigiBranchName, xStationVertex);
        const auto& yStationLocalVertex = fQTelescopeSetup->ToStationCoordinateSystem(yDigiBranchName, yStationVertex);
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] X Station Vertex (" << xStationVertex.x() << " " << xStationVertex.y() 
                   << " " << xStationVertex.z() << ")" << FairLogger::endl;
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] Y Station Vertex (" << yStationVertex.x() << " " << yStationVertex.y() 
                   << " " << yStationVertex.z() << ")" << FairLogger::endl;
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] X Station Vertex in station CS (" << xStationLocalVertex.x() << " " << xStationLocalVertex.y() 
                   << " " << xStationLocalVertex.z() << ")" << FairLogger::endl;
        LOG(DEBUG) << "[ERQTelescopeTrackFinder] Y Station Vertex in station CS (" << yStationLocalVertex.x() << " " << yStationLocalVertex.y() 
                   << " " << yStationLocalVertex.z() << ")" << FairLogger::endl;
        auto* track = AddTrack(TVector3(fTargetX, fTargetY, fTargetZ), xStationVertex, yStationVertex,
                               xStationLocalVertex, yStationLocalVertex, xChannel, yChannel, xStrip->Edep(), yStrip->Edep(),
                               itComponent.first);
        track->AddLink(FairLink(xDigiBranchName, xStripIndex));
        track->AddLink(FairLink(yDigiBranchName, yStripIndex));
      }
    }
  }
  LOG(DEBUG) << "[ERQTelescopeTrackFinder]------------Finished--------------------------------------"
             << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
TVector3 ERQTelescopeTrackFinder::
GetGlobalTrackPositionByStrip(const TString& branch_name, const int channel) const {
  // Local position of strip center
  auto local_position = fQTelescopeSetup->GetStripLocalPosition(branch_name, channel);
  const auto strip_width = fQTelescopeSetup->GetStripWidth(branch_name, channel);
  // Apply user coorections                                                     
  for (const auto& station_to_channels : track_position_corrections_) {
    const auto station_name = station_to_channels.first;
    if (!branch_name.Contains(station_name))
      continue;
    const auto channel_to_position_correction = station_to_channels.second;
    const auto channel_and_correction = channel_to_position_correction.find(channel);
    if (channel_and_correction == channel_to_position_correction.end())
      continue;
    const auto correction = channel_and_correction->second;
    const auto current_position = local_position[0];
    local_position[0] = current_position + strip_width * correction;
    LOG(DEBUG) << "[ERQTelescopeTrackFinder] Local position of strip " << channel << " of " 
               << station_name << " corrected from " << current_position << " to " 
               << local_position[0] << FairLogger::endl;
  }
  return fQTelescopeSetup->ToGlobalCoordinateSystem(branch_name, local_position);                                              
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
ERQTelescopeTrack* ERQTelescopeTrackFinder::AddTrack(
    const TVector3& targetVertex, const TVector3& xStationVertex, const TVector3& yStationVertex,
    const TVector3& xStationLocalVertex, const TVector3& yStationLocalVertex, 
    const Int_t xChannel, const Int_t yChannel, const Double_t xEdep, const Double_t yEdep,
    const TString& digiBranchName) {
  return new((*fQTelescopeTrack[digiBranchName]) [fQTelescopeTrack[digiBranchName]->GetEntriesFast()])
      ERQTelescopeTrack(targetVertex, xStationVertex, yStationVertex, xStationLocalVertex,
                        yStationLocalVertex, xChannel, yChannel, xEdep, yEdep);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrackFinder)
