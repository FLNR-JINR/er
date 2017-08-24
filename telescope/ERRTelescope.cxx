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
#include "ERRTelescopeSiPoint.h"
#include "ERRTelescopeCsIPoint.h"

#include "ERRTelescope.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERRTelescope::ERRTelescope() :
  ERDetector("ERRTelescope", kTRUE),
  fSiPoints11(NULL),
  fSiPoints12(NULL),
  fCsIPoints1(NULL),
  fSiPoints21(NULL),
  fSiPoints22(NULL),
  fCsIPoints2(NULL)
{
  fSiPoints11 = new TClonesArray("ERRTelescopeSiPoint");
  fSiPoints12 = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints1 = new TClonesArray("ERRTelescopeCsIPoint");
  fSiPoints21 = new TClonesArray("ERRTelescopeSiPoint");
  fSiPoints22 = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints2 = new TClonesArray("ERRTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERRTelescope::ERRTelescope(const char* name, Bool_t active, Int_t verbose) :
  ERDetector(name, active, 1),
  fSiPoints11(NULL),
  fSiPoints12(NULL),
  fCsIPoints1(NULL),
  fSiPoints21(NULL),
  fSiPoints22(NULL),
  fCsIPoints2(NULL)
{
  fSiPoints11 = new TClonesArray("ERRTelescopeSiPoint");
  fSiPoints12 = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints1 = new TClonesArray("ERRTelescopeCsIPoint");
  fSiPoints21 = new TClonesArray("ERRTelescopeSiPoint");
  fSiPoints22 = new TClonesArray("ERRTelescopeSiPoint");
  fCsIPoints2 = new TClonesArray("ERRTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = verbose;
}

ERRTelescope::~ERRTelescope() {
  if (fSiPoints11) {
    fSiPoints11->Delete();
    delete fSiPoints11;
  }
  if (fSiPoints21) {
    fSiPoints12->Delete();
    delete fSiPoints12;
  }
  if (fCsIPoints1) {
    fCsIPoints1->Delete();
    delete fCsIPoints1;
  }
  if (fSiPoints21) {
    fSiPoints21->Delete();
    delete fSiPoints21;
  }
  if (fSiPoints22) {
    fSiPoints22->Delete();
    delete fSiPoints22;
  }
  if (fCsIPoints2) {
    fCsIPoints2->Delete();
    delete fCsIPoints2;
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
  ioman->Register("RTelescope1Si1Point","RTelescope", fSiPoints11, kTRUE);
  ioman->Register("RTelescope1Si2Point","RTelescope", fSiPoints12, kTRUE);
  ioman->Register("RTelescope1CsIPoint","RTelescope", fCsIPoints1, kTRUE);
  ioman->Register("RTelescope2Si1Point","RTelescope", fSiPoints21, kTRUE);
  ioman->Register("RTelescope2Si2Point","RTelescope", fSiPoints22, kTRUE);
  ioman->Register("RTelescope2CsIPoint","RTelescope", fCsIPoints2, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERRTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fSiPoints11;
  if (iColl == 0)
    return fSiPoints12;
  if (iColl == 0)
    return fCsIPoints1;
  if (iColl == 0)
    return fSiPoints21;
  if (iColl == 0)
    return fSiPoints22;
  if (iColl == 0)
    return fCsIPoints2;
  return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERRTelescope::Print(Option_t *option) const
{
  if(fSiPoints11->GetEntriesFast() > 0)
  {
    std::cout << "======== Si 11 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints11->GetEntriesFast(); iPoint++){
      ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fSiPoints11->At(iPoint);
      point->Print();
    }
  }

  if(fSiPoints12->GetEntriesFast() > 0)
  {
    std::cout << "======== Si 12 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints12->GetEntriesFast(); iPoint++){
      ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fSiPoints12->At(iPoint);
      point->Print();
    }
  }

  if(fCsIPoints1->GetEntriesFast() > 0)
  {
    std::cout << "======== CsI 1 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoints1->GetEntriesFast(); iPoint++){
      ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fCsIPoints1->At(iPoint);
      point->Print();
    }
  }

  if(fSiPoints21->GetEntriesFast() > 0)
  {
    std::cout << "======== Si 21 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints21->GetEntriesFast(); iPoint++){
      ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fSiPoints21->At(iPoint);
      point->Print();
    }
  }

  if(fSiPoints22->GetEntriesFast() > 0)
  {
    std::cout << "======== Si 22 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints22->GetEntriesFast(); iPoint++){
      ERRTelescopeCsIPoint* point = (ERRTelescopeCsIPoint*)fSiPoints22->At(iPoint);
      point->Print();
    }
  }

  if(fCsIPoints2->GetEntriesFast() > 0)
  {
    std::cout << "======== CsI 2 Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoints2->GetEntriesFast(); iPoint++){
      ERRTelescopeCsIPoint* point = (ERRTelescopeCsIPoint*)fCsIPoints2->At(iPoint);
      point->Print();
    }
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERRTelescope::Reset() {
  LOG(INFO) << "  ERRTelescope::Reset()" << FairLogger::endl;
  fSiPoints11->Clear();
  fSiPoints12->Clear();
  fCsIPoints1->Clear();
  fSiPoints21->Clear();
  fSiPoints22->Clear();
  fCsIPoints2->Clear();
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
  Int_t size;
  if(fTelNb == 1){
    TClonesArray& clref = (fDetNb ==1) ? *fSiPoints11 : *fSiPoints12;
    if(fDetNb == 2)
      fSensorNb = -1;
    size = clref.GetEntriesFast();
    return new(clref[size]) ERRTelescopeSiPoint(fEventID, fTrackID, fTelNb, fDetNb,fMot0TrackID, fPID,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss,
              fSectorNb, fSensorNb);
  }
  if(fTelNb == 2){
    TClonesArray& clref = (fDetNb ==1) ? *fSiPoints21 : *fSiPoints22;
    if(fDetNb == 2)
      fSensorNb = -1;
    size = clref.GetEntriesFast();
    return new(clref[size]) ERRTelescopeSiPoint(fEventID, fTrackID, fTelNb, fDetNb,fMot0TrackID, fPID,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss,
              fSectorNb, fSensorNb);
  }
}

ERRTelescopeCsIPoint* ERRTelescope::AddCsIPoint() {
  Int_t size;
  if(fDetNb == 3){
    TClonesArray& clref = (fTelNb ==1) ? *fCsIPoints1 : *fCsIPoints2;
    size = clref.GetEntriesFast();
    return new(clref[size]) ERRTelescopeCsIPoint(fEventID, fTrackID, fTelNb, fMot0TrackID, fPID,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss,
              fCrystallNb);
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERRTelescope::CheckIfSensitive(std::string name)
{
  //cout << name << endl; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!change this method!!!!!!!!!!!!!!!!!!
  TString volName = name;
  if(volName.Contains("sensor")) {
    return kTRUE;
  }
  if(volName.Contains("sector")) {
    return kTRUE;
  }
  if(volName.Contains("crystall")) {
    return kTRUE;
  }
  return kFALSE;
}
Bool_t ERRTelescope::ProcessHits(FairVolume* vol) {
  Int_t telnb;
  Int_t detnb;
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
        gMC->CurrentVolOffID(3, fDetNb);
        gMC->CurrentVolOffID(4, fTelNb);
        AddSiPoint();
      }
      if(volName.Contains("sector"))
      {
        gMC->CurrentVolID(fSectorNb);
        gMC->CurrentVolOffID(2, fDetNb);
        gMC->CurrentVolOffID(3, fTelNb);
        AddSiPoint();
      }
      if(volName.Contains("crystall"))
      {
        gMC->CurrentVolID(fCrystallNb);
        // gMC->CurrentVolOffID(2, fDetNb);
        gMC->CurrentVolOffID(3, fTelNb);
        AddCsIPoint();
      }
    }
  }
  return kTRUE;
}



// ----------------------------------------------------------------------------
ClassImp(ERRTelescope)
