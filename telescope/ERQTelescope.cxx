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
#include "FairLogger.h"

//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope() :
  ERDetector("ERQTelescope", kTRUE),
  fSiPoint(NULL),
  fCsIPoint(NULL)
{
  ResetParameters();
  fSiPoint =  new TClonesArray("ERQTelescopeSiPoint");
  fCsIPoint = new TClonesArray("ERQTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVerboseLevel = 1;
  fVersion = 1;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active,1),
  fSiPoint(NULL),
  fCsIPoint(NULL)
{
  ResetParameters();
  fSiPoint =  new TClonesArray("ERQTelescopeSiPoint");
  fCsIPoint = new TClonesArray("ERQTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVersion = 1;
  fVerboseLevel = verbose;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::~ERQTelescope() {
  if (fSiPoint) {
    fSiPoint->Delete();
    delete fSiPoint;
  }
  if (fCsIPoint) {
    fCsIPoint->Delete();
    delete fCsIPoint;
  }
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Initialize() {
  FairDetector::Initialize();
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiPoint* ERQTelescope::Add_SiPoint() {
  TClonesArray& clref = *fSiPoint;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeSiPoint(fEventID, fTrackID, fMot0TrackID, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fLength, fEloss, fN_Station, fX_Strip, fY_Strip);

}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint* ERQTelescope::Add_CsIPoint() {
  TClonesArray& clref = *fCsIPoint;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeCsIPoint(fEventID, fTrackID, fMot0TrackID, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fLength, fEloss, fN_Wall, fN_Block);
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ConstructGeometry() {
  fQTelescopeSetup->ConstructGeometry();
  SetGeometryFileName("QTelescope.temp.root");
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
    fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    // gMC->CurrentVolID(sensor);
    // gMC->CurrentVolOffID(1, sector);
  }

  fEloss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared())
	{
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    TString volName = gMC->CurrentVolName();
	  if (fEloss > 0.){
      if (volName.Contains("Si")) {
        gMC->CurrentVolOffID(0, fX_Strip);
        gMC->CurrentVolOffID(1, fY_Strip);
        gMC->CurrentVolOffID(2, fN_Station);
        Add_SiPoint();
      }
      if (volName.Contains("CsI")) {
        gMC->CurrentVolID(fN_Block);
        gMC->CurrentVolOffID(1, fN_Wall);
        Add_CsIPoint();
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
  ioman->Register("ERQTelescopeSiPoint","QTelescope", fSiPoint, kTRUE);
  ioman->Register("ERQTelescopeCsIPoint","QTelescope", fCsIPoint, kTRUE);
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERQTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fSiPoint;
  if (iColl == 0)
    return fCsIPoint;
  return NULL;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Print(Option_t *option) const {
  if(fSiPoint->GetEntriesFast() > 0) {
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
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Reset() {
  LOG(INFO) << "  ERQTelescope::Reset()" << FairLogger::endl;
  fSiPoint->Clear();
  fCsIPoint->Clear();
  ResetParameters();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(INFO) << "   ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)"
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
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescope::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.Contains("box")) {
    return kTRUE;
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ResetParameters() {
  LOG(INFO) << "   ERQTelescope::ResetParameters() " << FairLogger::endl;
};
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescope)

