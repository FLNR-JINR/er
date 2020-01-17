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
  std::vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    if (bFullName.Contains("Digi") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
    }
  }
  // Register output track branches only for stations that are setted by interface SetStation(){
  fQTelDigi["Fictious"] = nullptr;
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
      const TClonesArray* xDigi = fQTelDigi[xDigiBranchName];
      // const TClonesArray* yDigi = fQTelDigi[yDigiBranchName];
      if (!xDigi) {
        continue;
      }
      if (xDigi->GetEntriesFast() == 0) {
        continue;
      }
      correctStripsY.push_back(0);
      
      //std::vector<double> thresholds = {24, 28, 28, 30, 28, 29, 31, 28, 28, 29, 29, 29, 25, 28, 28, 23};
      //std::vector<double> coeff_a = {0.0170149, 0.016903, 0.0167109, 0.0165498, 0.0167078, 0.0166459, 0.0168446, 0.0164798, 0.0167559, 0.0169723, 0.017026, 0.0171465, 0.0170058, 0.0170906, 0.0167915, 0.0164734};
      //std::vector<double> coeff_b = {-0.30411, -0.377272, -0.29458, -0.378852, -0.395058, -0.395741, -0.358301, -0.273369, -0.434498, -0.269807, -0.29015, -0.286433, -0.303534, -0.262838, -0.351829, -0.34803};
      //int mult = 0;
      for (Int_t iXDigi  = 0; iXDigi < xDigi->GetEntriesFast(); iXDigi++) {
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(iXDigi))->GetEdep();
	//cout << (xStripEdep - coeff_b[iXDigi]) / coeff_a[iXDigi] << endl;
	//if ((xStripEdep - coeff_b[iXDigi]) / coeff_a[iXDigi] > thresholds[iXDigi]) {
	//  cout << (xStripEdep - coeff_b[iXDigi]) / coeff_a[iXDigi] << endl;
        //  mult++;
	//}
        if (xStripEdep > fSiDigiEdepMin && xStripEdep < fSiDigiEdepMax) {
          correctStripsX.push_back(iXDigi);
        }
      }
      //if (mult != 1) {
      //  continue;
      //}
      for (const auto itCorrectStripsX : correctStripsX) {
        const Double_t xStripEdep = ((ERQTelescopeSiDigi*)xDigi->At(itCorrectStripsX))->GetEdep();
        for (const auto itCorrectStripsY : correctStripsY) {
          const Double_t yStripEdep = 1e-3;//((ERQTelescopeSiDigi*)xDigi->At(itCorrectStripsX))->GetEdep();
          if (TMath::Abs(xStripEdep - yStripEdep) < fEdepDiffXY) {
            hitTelescopePoint.push_back(pair<Int_t, Int_t>(itCorrectStripsX, itCorrectStripsY));
          }
        }
      }
      LOG(DEBUG) << "Strs array pair " << itComponent.second.first << " " 
                                         << itComponent.second.second << FairLogger::endl;
      LOG(DEBUG) << "Hits count on pair " << hitTelescopePoint.size() << FairLogger::endl;
      for (const auto& itHitPoint : hitTelescopePoint) {
        const auto xStripIndex = itHitPoint.first;
        // const auto yStripIndex = itHitPoint.second;
        const auto* xStrip = dynamic_cast<ERQTelescopeSiDigi*>(xDigi->At(xStripIndex));
        // const auto* yStrip = dynamic_cast<ERQTelescopeSiDigi*>(yDigi->At(yStripIndex));
        if (!xStrip)
          continue;
        const auto xStripNb = xStrip->GetStripNb();
        const auto yStripNb = 8; // xStrip->GetStripNb();
        LOG(DEBUG) << "  Branch names X:" << xDigiBranchName 
                   << " Y: " << yDigiBranchName << FairLogger::endl;
        LOG(DEBUG) << "  Strips pair " << xStripIndex << " " << 0 << FairLogger::endl;
        LOG(DEBUG) << "  Strips pair numbers " << xStripNb << " " << yStripNb << FairLogger::endl;
        // Calc unknown coordinated using condition: target, hit on first station(closest) and
        // hit on second station lie on line :
        // {x1, y1, z1} = {fTargetX, fTargetY, fTargetZ} + k * ({x1, y1, z1} - {fTargetX, fTargetY, fTargetZ}).
        // const bool xStationIsClosest = fQTelSetup->GetStripGlobalZ(xDigiBranchName, xStripNb)
        //                                < fQTelSetup->GetStripGlobalZ(yDigiBranchName, yStripNb);
        // We know all about z coordinate, so 
        const double z = fQTelSetup->GetStripGlobalZ(xDigiBranchName, xStripNb);

        assert(z != fTargetZ);

        const double x = fQTelSetup->GetStripGlobalX(xDigiBranchName, xStripNb);
        const double y = fQTelSetup->GetStripGlobalY(xDigiBranchName, xStripNb);
        // const double k = (z2 - fTargetZ) / (z1 - fTargetZ);
        // double x1 = 0., x2 = 0., y1 = 0., y2 = 0.;
        // if (xStationIsClosest) { // find y1, x2 from equation
        //   y1 = (-1./k)*((1. - k)*fTargetY - y2);
        //   x2 = (1. - k)*fTargetX + k*x1;
        // } else { // find x1, y2 from equation
        //   x2 = fQTelSetup->GetStripGlobalX(xDigiBranchName, xStripNb);
        //   y1 = fQTelSetup->GetStripGlobalY(yDigiBranchName, yStripNb);
        //   x1 = (-1./k)*((1. - k)*fTargetX - x2);
        //   y2 = (1. - k)*fTargetY + k*y1;
        // }
        const auto xQTeleGlobHit = x;
        const auto yQTeleGlobHit = y;
        const auto zQTeleGlobHit = z;
        const auto xQTeleLocalHit = fQTelSetup->GetStripLocalX(xDigiBranchName, xStripNb);
        const auto yQTeleLocalHit = fQTelSetup->GetStripLocalX(xDigiBranchName, xStripNb);
        const auto zQTeleLocalHit = fQTelSetup->GetStripLocalZ(xDigiBranchName, xStripNb);
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
        // track->AddLink(FairLink(yDigiBranchName, yStripIndex));
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
