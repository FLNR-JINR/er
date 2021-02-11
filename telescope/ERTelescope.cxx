/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTelescope.h"

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
#include "ERPoint.h"
//-------------------------------------------------------------------------------------------------
ERTelescope::ERTelescope() :
  ERDetector("ERTelescope", kTRUE)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERTelescopeSetup::Instance();
  fVerboseLevel = 1;
  fVersion = 1;
}
//-------------------------------------------------------------------------------------------------
ERTelescope::ERTelescope(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active,1)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERTelescopeSetup::Instance();
  fVersion = 1;
  fVerboseLevel = verbose;
}
//-------------------------------------------------------------------------------------------------
ERTelescope::~ERTelescope() {
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
void ERTelescope::Initialize() {
  FairDetector::Initialize();
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::AddPoint(TClonesArray& clref) {
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  TVector3 pos_in_local_cs;
  pos_in_local_cs.SetXYZ(localPos[0],localPos[1],localPos[2]);
  new(clref[clref.GetEntriesFast()]) ERPoint(
    fEventID, fTrackID, fMot0TrackID, fChannel, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),  fPosIn.Z()),
    pos_in_local_cs,
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fTime, fLength, fEloss, -1 /*light yield*/,fPDG, -1 /*charge*/);
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::ConstructGeometry() {
  fQTelescopeSetup->ConstructGeometry();
  SetGeometryFileName(fQTelescopeSetup->GetGeoFileName());
  ConstructRootGeometry();
}
//-------------------------------------------------------------------------------------------------
Bool_t ERTelescope::ProcessHits(FairVolume* vol) {
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
      LOG(FATAL) << "[ERTelescope] Not found setup component for sensetive volume path" 
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
void ERTelescope::BeginEvent() {
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    Fatal("Init", "IO manager is not set");
  for (const auto* component : fQTelescopeSetup->GetAllComponents()) {
    if (!dynamic_cast<const ERTelescopeGeoComponentSensetive*>(component))
      continue;
    for (const auto branchName : component->GetBranchNames(ERDataObjectType::Point)) {
      LOG(DEBUG) << "[ERTelescope] Register branch " << branchName 
                 << " for component " << component->GetVolumeName() << FairLogger::endl;
      fPoints[component->GetVolumeName()][branchName] = 
          new TClonesArray("ERPoint", consts::approx_telescope_point_number);
      ioman->Register(branchName, "Telescope", fPoints[component->GetVolumeName()][branchName], kTRUE);
    }
  }
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERTelescope::GetCollection(Int_t iColl) const {
 /* if (iColl == 0)
    return fSiPoint;
  if (iColl == 0)
    return fCsIPoint;
*/
  return NULL;
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::Print(Option_t *option) const {
/*  if(fSiPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== Si Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoint->GetEntriesFast(); iPoint++){
      ERTelescopeSiPoint* point = (ERTelescopeSiPoint*)fSiPoint->At(iPoint);
      point->Print();
    }
  }
  if(fCsIPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== CsI Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoint->GetEntriesFast(); iPoint++){
      ERTelescopeCsIPoint* point = (ERTelescopeCsIPoint*)fCsIPoint->At(iPoint);
      point->Print();
    }
  }
*/
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::Reset() {
  for(auto& componentPoints : fPoints) {
    for (auto& branchPoints : componentPoints.second) {
      branchPoints.second->Clear();
    }
  }
  ResetParameters();
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
/*  LOG(INFO) << "   ERTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)"
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "QTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERTelescopeSiPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
*/
}
//-------------------------------------------------------------------------------------------------
Bool_t ERTelescope::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.BeginsWith("Sensitive")) {
    return kTRUE;
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERTelescope::ResetParameters() {
};
//-------------------------------------------------------------------------------------------------
ClassImp(ERTelescope)

