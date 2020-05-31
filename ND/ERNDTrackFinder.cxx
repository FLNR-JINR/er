/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNDTrackFinder.h"

#include "FairLogger.h"

#include "ERBeamDetTrack.h"
#include "ERNDDigi.h"

ERNDTrackFinder::ERNDTrackFinder()
  : ERTask("ER ND track finder") 
{
    fAvailibleRunManagers.push_back("ERRunAna");
    fAvailibleRunManagers.push_back("FairRunAna");
}

InitStatus ERNDTrackFinder::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fNDDigis = (TClonesArray*) ioman->GetObject("NDDigi");
  if (!fNDDigis) Fatal("Init", "Can`t find collection NDDigi!"); 
  fNDTracks = new TClonesArray("ERTrack",1000);
  ioman->Register("NDTrack", "ND track", fNDTracks, kTRUE);
  fSetup = ERNDSetup::Instance();
  if (!fUserTargetVertexIsSet) {
    fBeamDetTrack = (TClonesArray*) ioman->GetObject("BeamDetTrack");   
    if (!fBeamDetTrack) {
    LOG(DEBUG) << "ERNDTrackFinder: target point not initialized by user " 
                << "(by means of SetTargetVertex()) and there is no ERBeamDetTrack branch" 
                <<FairLogger::endl;
    return kFATAL;
    } 
  }
  fSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
}

void ERNDTrackFinder::Exec(Option_t* opt) {
  Reset();
  if (!fUserTargetVertexIsSet) {
    ERBeamDetTrack* trackFromMWPC = (ERBeamDetTrack*)fBeamDetTrack->At(0);
    if (!trackFromMWPC) {
        //fRun->MarkFill(kFALSE);
        return ;
    }
    fTargetVertex = TVector3(trackFromMWPC->GetTargetX(), trackFromMWPC->GetTargetY(),
                             trackFromMWPC->GetTargetZ());
  }
  for (Int_t iDigi(0); iDigi < fNDDigis->GetEntriesFast(); iDigi++) {
    const auto* digi = static_cast<ERNDDigi*>(fNDDigis->At(iDigi));
    AddTrack(fSetup->Pos(digi->Channel()), fTargetVertex, digi->Edep());
  }
}

void ERNDTrackFinder::Reset() {
    fNDTracks->Clear();
}

void ERNDTrackFinder::SetTargetVertex(const TVector3& vertex) {
  fTargetVertex = vertex;
  fUserTargetVertexIsSet = true;
}

ERTrack* ERNDTrackFinder::AddTrack(const TVector3& detectorVertex, 
                                   const TVector3& targetVertex, Double_t edep) {
    return new((*fNDTracks)[fNDTracks->GetEntriesFast()])
        ERTrack(detectorVertex, targetVertex, edep);
}