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
#include "ERDigi.h"

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
    if (bFullName.Contains("Digi") && bFullName.Contains("Telescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
    }
  }
  // Register output track branches only for stations that are setted by interface SetStation(){
  for (const auto itSubassemblies : fSiHitStationsPair) {
    for (const auto itComponent : itSubassemblies.second) {
      fQTelescopeTrack[itComponent.first] = new TClonesArray("ERQTelescopeTrack");
      ioman->Register("TelescopeTrack_" + itComponent.first, "Telescope", 
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
      if (fQTelescopeSetup->GetStationType(xDigiBranchName) != fQTelescopeSetup->GetStationType(yDigiBranchName)) {
        LOG(FATAL) << "Do not mix R and Q telescopes for track finding" << FairLogger::endl;
      }
      const TClonesArray* xDigi = fQTelescopeDigi[xDigiBranchName];
      const TClonesArray* yDigi = fQTelescopeDigi[yDigiBranchName];
      if ( !xDigi || !yDigi) {
        continue;
      }
      if (xDigi->GetEntriesFast() == 0 || yDigi->GetEntriesFast()==0) {
        continue;
      }
      for (Int_t iXDigi  = 0; iXDigi < xDigi->GetEntriesFast(); iXDigi++) {
        const Double_t xStripEdep = ((ERDigi*)xDigi->At(iXDigi))->Edep();
        if (xStripEdep > fSiDigiEdepMin && xStripEdep < fSiDigiEdepMax) {
          correctStripsX.push_back(iXDigi);
        }
      }
      for (Int_t iYDigi  = 0; iYDigi < yDigi->GetEntriesFast(); iYDigi++) {
        const Double_t yStripEdep = ((ERDigi*)yDigi->At(iYDigi))->Edep();
        if (yStripEdep > fSiDigiEdepMin && yStripEdep < fSiDigiEdepMax) {
          correctStripsY.push_back(iYDigi);
        }
      }
      for (const auto itCorrectStripsX : correctStripsX) {
        const Double_t xStripEdep = ((ERDigi*)xDigi->At(itCorrectStripsX))->Edep();
        for (const auto itCorrectStripsY : correctStripsY) {
          const Double_t yStripEdep = ((ERDigi*)yDigi->At(itCorrectStripsY))->Edep();
          if (TMath::Abs(xStripEdep - yStripEdep) < fEdepDiffXY) {
            hitTelescopePoint.push_back(std::pair<Int_t, Int_t>(itCorrectStripsX, itCorrectStripsY));
          }
        }
      }
      LOG(DEBUG) << "[ERQTelescopeTrackFinder] Strips array pair " << itComponent.second.first << " " 
                                         << itComponent.second.second << FairLogger::endl;
      LOG(DEBUG) << "[ERQTelescopeTrackFinder] Hits count on pair " << hitTelescopePoint.size() << FairLogger::endl;
      for (const auto& itHitPoint : hitTelescopePoint) {
        const auto xChannelIndex = itHitPoint.first;
        const auto yChannelIndex = itHitPoint.second;
        const auto* xStrip = dynamic_cast<ERDigi*>(xDigi->At(xChannelIndex));
        const auto* yStrip = dynamic_cast<ERDigi*>(yDigi->At(yChannelIndex));
        if (!xStrip || !yStrip)
          continue;
        const auto xChannel = xStrip->Channel();
        const auto yChannel = yStrip->Channel();
        if (fQTelescopeSetup->GetStationType(xDigiBranchName) == ERQTelescopeSetup::StationType::QStation) {
          CreateTrackInQTelescope(xChannelIndex, yChannelIndex, xChannel, yChannel, 
                                  xStrip->Edep(), yStrip->Edep(), xDigiBranchName, yDigiBranchName,
                                  itComponent.first);
        } else {
          CreateTrackInRTelescope(xChannelIndex, yChannelIndex, xChannel, yChannel, 
                                  xStrip->Edep(), yStrip->Edep(), xDigiBranchName, yDigiBranchName,
                                  itComponent.first);
        }
      }
    }
  }
  LOG(DEBUG) << "[ERQTelescopeTrackFinder]------------Finished--------------------------------------"
             << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::CreateTrackInQTelescope(
    const Int_t xChannelIndex, const Int_t yChannelIndex, const Int_t xChannel, const Int_t yChannel,
    const Double_t xEdep, const Double_t yEdep, const TString& xDigiBranchName, const TString& yDigiBranchName,
    const TString& trackBranchName) {
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] Branch names X:" << xDigiBranchName 
              << " Y: " << yDigiBranchName << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] Strips pair numbers " << xChannel << " " 
              << yChannel << FairLogger::endl;
  // Calc unknown coordinated using condition: target, hit on first station(closest) and
  // hit on second station lie on line :
  // {x2, y2, z2} = {fTargetX, fTargetY, fTargetZ} + k * ({x1, y1, z1} - {fTargetX, fTargetY, fTargetZ}).
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
    x1 = fQTelescopeSetup->GetStripGlobalX(xDigiBranchName, xChannel);
    y2 = fQTelescopeSetup->GetStripGlobalY(yDigiBranchName, yChannel);
    LOG(DEBUG) << "[ERQTelescopeTrackFinder] Coordinates from strips. x1 = " << x1 
                << " y2 = " << y2 << " z1 = " << z1 << " z2 = " << z2 << FairLogger::endl;
    y1 = (-1./k)*((1. - k)*fTargetY - y2);
    x2 = (1. - k)*fTargetX + k*x1;
  } else { // find x1, y2 from equation
    x2 = fQTelescopeSetup->GetStripGlobalX(xDigiBranchName, xChannel);
    y1 = fQTelescopeSetup->GetStripGlobalY(yDigiBranchName, yChannel);
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
                          xStationLocalVertex, yStationLocalVertex, xChannel, yChannel, xEdep, yEdep,
                          trackBranchName);
  track->AddLink(FairLink(xDigiBranchName, xChannelIndex));
  track->AddLink(FairLink(yDigiBranchName, yChannelIndex));
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeTrackFinder::CreateTrackInRTelescope(
    const Int_t phiChannelIndex, const Int_t rChannelIndex, const Int_t phiChannel, const Int_t rChannel,
    const Double_t phiEdep, const Double_t rEdep, const TString& phiDigiBranchName, const TString& rDigiBranchName,
    const TString& trackBranchName) {
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] Branch names phi:" << phiDigiBranchName 
              << " R: " << rDigiBranchName << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] phi channel = " << phiChannel << " r channel = " 
              << rChannel << FairLogger::endl;
  // Calc unknown coordinated using condition: target, hit on first station(closest) and
  // hit on second station lie on line :
  // {x2, y2, z2} = {fTargetX, fTargetY, fTargetZ} + k * ({x1, y1, z1} - {fTargetX, fTargetY, fTargetZ}).
  // x = x_station + r * cos(phi); y = y_station + r * sin(phi)
  // Lets 1 - phi station(we know phi1), 2 - r station (we know r2)
  // r2 cos(phi2) = fTargetX - x_station2 + k(x_station1 + r1 cos(phi1) - fTargetX)
  // r2 sin(phi2) = fTargetY - y_station2 + k(y_station1 + r1 sin(phi1) - fTargetX)
  // k = (z2 - fTargetZ) / (z1 - fTargetZ)
  // ----
  // r1: r2^2 = (fTargetX - x_station2 + k(x_station1 + r1 cos(phi1) - fTargetX))^2 
  //            + (fTargetY - y_station2 + k(y_station1 + r1 sin(phi1) - fTargetY))^2
  // A = fTargetX - x_station2 + k(x_station1 - fTargetX)
  // B = k cos(phi1)
  // C = fTargetY - y_station2 + k(y_station1 - fTargetY)
  // D = k sin(phi1)
  // r2^2 = (A + Br1)^2 + (C + Dr1)^2
  // r1 = -/+(sqrt(D^2(r2^2 - A^2)+2ABCD +B^2(r2^2 - C^2)) +- AB +/- CD) / (B^2 +D^2)
  // r1 = -/+(sqrt(D^2(r2^2 - A^2)+2ABCD +B^2(r2^2 - C^2)) +- AB +/- CD) / k^2
  const TVector3 station1 = fQTelescopeSetup->GetStationTranslation(phiDigiBranchName);
  const TVector3 station2 = fQTelescopeSetup->GetStationTranslation(rDigiBranchName);
  const TVector3 target(fTargetX, fTargetY, fTargetZ);
  const Double_t phi1 = fQTelescopeSetup->GetStripPhi(phiDigiBranchName, phiChannel);
  const Double_t r2 = fQTelescopeSetup->GetStripR(rDigiBranchName, rChannel);
  const Double_t k = (station2.Z() - fTargetZ) / (station1.Z() - fTargetZ);
  const Double_t A = fTargetX - station2.X() + k * (station1.X() - fTargetX); 
  const Double_t B = k * TMath::Cos(phi1*TMath::RadToDeg());
  const Double_t C = fTargetY - station2.Y() + k * (station1.Y() - fTargetY);
  const Double_t D = k * TMath::Sin(phi1*TMath::RadToDeg());
  const Double_t r1 = (TMath::Sqrt(D*D*(r2*r2 - A*A) + 2*A*B*C*D +B*B*(r2*r2 - C*C)) - A*B - C*D) / (k*k);
  const TVector3 local_vertex1(r1 * TMath::Cos(phi1*TMath::DegToRad()), r1 * TMath::Sin(phi1*TMath::DegToRad()), 0.);
  const TVector3 global_vertex1 = station1 + local_vertex1;
  const TVector3 global_vertex2 = target + k * (global_vertex1 - target);
  const TVector3 local_vertex2 = global_vertex2 - station2;
  const Double_t phi2 = TMath::ACos(local_vertex2.X() / r2) * TMath::RadToDeg();
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] phi station: phi = " << phi1 << " r = " << r1 << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] r station: phi = " << phi2 << " r = " << r2 << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] phi station: local vertex = (" << local_vertex1.x() << " " << local_vertex1.y() 
             << " " << local_vertex1.z() << ")" << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] r station: local vertex = (" << local_vertex2.x() << " " << local_vertex2.y() 
             << " " << local_vertex2.z() << ")" << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] phi station: global vertex = (" << global_vertex1.x() << " " << global_vertex1.y() 
             << " " << global_vertex1.z() << ")" << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeTrackFinder] r station: global vertex = (" << global_vertex2.x() << " " << global_vertex2.y() 
             << " " << global_vertex2.z() << ")" << FairLogger::endl;
  auto* track = AddTrack(TVector3(fTargetX, fTargetY, fTargetZ), global_vertex1, global_vertex2,
                          local_vertex1, local_vertex2, phiChannel, rChannel, phiEdep, rEdep,
                          trackBranchName);
  track->AddLink(FairLink(phiDigiBranchName, phiChannelIndex));
  track->AddLink(FairLink(rDigiBranchName, rChannelIndex));
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
