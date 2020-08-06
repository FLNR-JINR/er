/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescope.h"

#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERTelescopeGeoComponentSensetive.h"
//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope() :
  ERDetector("ERQTelescope", kTRUE)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVerboseLevel = 1;
  fVersion = 1;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active,1)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVersion = 1;
  fVerboseLevel = verbose;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::~ERQTelescope() {
/*  if (fSiPoint) {
    fSiPoint->Delete();
    delete fSiPoint;
  }
  if (fCsIPoint) {
    fCsIPoint->Delete();
    delete fCsIPoint;
  }
  */
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Initialize() {
  FairDetector::Initialize();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::AddPoint(TClonesArray& clref) {
  new(clref[clref.GetEntriesFast()]) ERQTelescopeSiPoint(
    fEventID, fTrackID, fMot0TrackID, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fLength, fEloss, fChannel,fPDG);
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ConstructGeometry() {
  fQTelescopeSetup->ConstructGeometry();
  SetGeometryFileName(fQTelescopeSetup->GetGeoFileName());
  ConstructRootGeometry();
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescope::ProcessHits(FairVolume* vol) {
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fEloss  = 0.;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fMass = gMC->ParticleMass(gMC->TrackPid()) * 1000; // MeV
    fPDG = gMC->TrackPid();
  }
  fEloss += gMC->Edep() * 1000.; // MeV //Return the energy lost in the current step
  if ((gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume
       gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
       gMC->IsTrackDisappeared()) && fEloss > 0.) {
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    const TString path = gMC->CurrentVolPath();
    const auto* component = dynamic_cast<ERTelescopeGeoComponentSensetive*>(fQTelescopeSetup->GetComponent(path));
    if (!component)
      LOG(FATAL) << "[ERQTelescope] Not found setup component for sensetive volume path" 
                  << path << FairLogger::endl;
    for (const auto orientation : component->GetOrientationsAroundZ()) {
      for (const auto channelSide : component->GetChannelSides()) {
        fChannel = component->GetChannelFromSensetiveNodePath(path, orientation);
        auto* pointCollection = 
            fPoints[component->GetVolumeName()][component->GetBranchName(ERDataObjectType::Point, orientation, channelSide)];
        AddPoint(*pointCollection);
      }
    }
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::BeginEvent() {
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    Fatal("Init", "IO manager is not set");
  for (const auto* component : fQTelescopeSetup->GetAllComponents()) {
    if (!dynamic_cast<const ERTelescopeGeoComponentSensetive*>(component))
      continue;
    for (const auto branchName : component->GetBranchNames(ERDataObjectType::Point)) {
      LOG(DEBUG) << "[ERQTelescope] Register branch " << branchName 
                 << " for component " << component->GetVolumeName() << FairLogger::endl;
      fPoints[component->GetVolumeName()][branchName] = new TClonesArray("ERQTelescopeSiPoint");
      ioman->Register(branchName, "Telescope", fPoints[component->GetVolumeName()][branchName], kTRUE);
    }
  }
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERQTelescope::GetCollection(Int_t iColl) const {
 /* if (iColl == 0)
    return fSiPoint;
  if (iColl == 0)
    return fCsIPoint;
*/
  return NULL;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Print(Option_t *option) const {
/*  if(fSiPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== Si Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fSiPoint->At(iPoint);
      point->Print();
    }
  }
  if(fCsIPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== CsI Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeCsIPoint* point = (ERQTelescopeCsIPoint*)fCsIPoint->At(iPoint);
      point->Print();
    }
  }
*/
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Reset() {
  for(auto& componentPoints : fPoints) {
    for (auto& branchPoints : componentPoints.second) {
      branchPoints.second->Clear();
    }
  }
  ResetParameters();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
/*  LOG(INFO) << "   ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)"
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "QTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERQTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERQTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERQTelescopeSiPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
*/
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescope::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.BeginsWith("Sensitive")) {
    return kTRUE;
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ResetParameters() {
};
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescope)

