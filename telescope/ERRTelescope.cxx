// -------------------------------------------------------------------------
// -----                        ERRTelescope source file                   -----
// -----                  Created data  by developerName               -----
// -------------------------------------------------------------------------
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "ERRTelescope.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERRTelescope::ERRTelescope() : 
  ERDetector("ERRTelescope", kTRUE),
  fSiPoints(NULL),
  fCsIPoints(NULL)
{
  fSiPoints = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints = new TClonesArray("ERRTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERRTelescope::ERRTelescope(const char* name, Bool_t active, Int_t verbose) : 
  ERDetector(name, active, 1),
  fSiPoints(NULL),
  fCsIPoints(NULL)
{
  fSiPoints = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints = new TClonesArray("ERRTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = verbose;
}

ERRTelescope::~ERRTelescope() {
  if (fSiPoints) {
    fSiPoints->Delete();
    delete fSiPoints;
  }
  if (fCsIPoints) {
    fCsIPoints->Delete();
    delete fCsIPoints;
  }
}

void ERRTelescope::Initialize()
{
  FairDetector::Initialize();
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERRTelescope::BeginEvent() {
}


void ERRTelescope::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}

// -----   Public method Register   -------------------------------------------
void ERRTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    Fatal("Init", "IO manager is not set"); 
  ioman->Register("RTelescopeSiPoint","RTelescope", fSiPoints, kTRUE);
  ioman->Register("RTelescopeCsIPoint","RTelescope", fCsIPoints, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERRTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fSiPoints;
  if (iColl == 0)
    return fCsIPoints;
  return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERRTelescope::Print(Option_t *option) const
{
  if(fSiPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== Si Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints->GetEntriesFast(); iPoint++){
      ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fSiPoints->At(iPoint);
      point->Print();
    }
  }
  if(fCsIPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== CsI Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoints->GetEntriesFast(); iPoint++){
      ERRTelescopeCsIPoint* point = (ERRTelescopeCsIPoint*)fCsIPoints->At(iPoint);
      point->Print();
    }
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERRTelescope::Reset() {
  LOG(INFO) << "  ERRTelescope::Reset()" << FairLogger::endl;
  fSiPoints->Clear();
  fCsIPoints->Clear();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERRTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "RTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERRTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERRTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERRTelescopeSiPoint(*oldpoint);
  }
  LOG(INFO) << "RTelescope: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERRTelescopeSiPoint* ERRTelescope::AddSiPoint() {
  TClonesArray& clref = *fSiPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERRTelescopeSiPoint(fEventID, fTrackID, fTelescopeNb, fDetectorNb,fMot0TrackID, fPID,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, 
              fSectorNb, fSensorNb);
  
}

ERRTelescopeCsIPoint* ERRTelescope::AddCsIPoint() {
  TClonesArray& clref = *fCsIPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERRTelescopeCsIPoint(fEventID, fTrackID, fTelescopeNb, fDetectorNb,fMot0TrackID, fPID,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, 
              fCrystallNb);
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERRTelescope::CheckIfSensitive(std::string name)
{
  //cout << name << endl;
  TString volName = name;
  if(volName.Contains("sensor")) {
    return kTRUE;
  }
  if(volName.Contains("crystall")) {
    return kTRUE;
  }
  return kFALSE;
}
Bool_t ERRTelescope::ProcessHits(FairVolume* vol) {  
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fELoss  = 0.;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fPID = gMC->TrackPid(); // GeV/c2
  }
  
  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    TString volName = gMC->CurrentVolName();
	  if (fELoss > 0.){
      if(volName.Contains("sensor"))
      {
        gMC->CurrentVolID(fSensorNb);
        gMC->CurrentVolOffID(1, fSectorNb);
        gMC->CurrentVolOffID(2, fDetectorNb);
        gMC->CurrentVolOffID(3, fTelescopeNb);
        AddSiPoint();
      }
      if(volName.Contains("crystall"))
      {
        gMC->CurrentVolID(fCrystallNb);
        gMC->CurrentVolOffID(1, fDetectorNb);
        gMC->CurrentVolOffID(2, fTelescopeNb);
        AddCsIPoint();
      }
    }
  }
  return kTRUE;
}



// ----------------------------------------------------------------------------
ClassImp(ERRTelescope)
