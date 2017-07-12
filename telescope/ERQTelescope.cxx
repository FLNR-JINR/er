// -------------------------------------------------------------------------
// -----                        ERQTelescope source file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "ERQTelescope.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERQTelescope::ERQTelescope() :
  ERDetector("ERQTelescope", kTRUE),
  fSiPoints(NULL)
{
  fSiPoints = new TClonesArray("ERQTelescopeSiPoint");
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERQTelescope::ERQTelescope(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active, 1),
    fSiPoints(NULL)
{
  fSiPoints = new TClonesArray("ERQTelescopeSiPoint");
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = verbose;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
ERQTelescope::~ERQTelescope() {
  if (fSiPoints) {
    fSiPoints->Delete();
    delete fSiPoints;
  } 
}

void ERQTelescope::Initialize()
{
  FairDetector::Initialize();
}

void ERQTelescope::BeginEvent() {
}
// -------------------------------------------------------------------------
void ERQTelescope::EndOfEvent() {
  if (fVerboseLevel > 0)
        Print();
  Reset();
}
// -------------------------------------------------------------------------
void ERQTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
        Fatal("Init", "IO manager is not set");
  ioman->Register("QTelescopeSiPoint","QTelescope", fSiPoints, kTRUE);
}
// ----------------------------------------------------------------------------
TClonesArray* ERQTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fSiPoints;
  return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERQTelescope::Print(Option_t *option) const
{
  if(fSiPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== Si Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoints->GetEntriesFast(); iPoint++){
      ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fSiPoints->At(iPoint);
      point->Print();
    }
  }
}
// ----------------------------------------------------------------------------
void ERQTelescope::Reset() {
  fSiPoints->Clear();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  std::cout << "QTelescope: " << nEntries << " entries to add" << std::endl;
  TClonesArray& clref = *cl2;
  ERQTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERQTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERQTelescopeSiPoint(*oldpoint);
  }
  std::cout << "QTelescope: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
}
// ----------------------------------------------------------------------------
ERQTelescopeSiPoint* ERQTelescope::AddSiPoint()
{
  TClonesArray& clref = *fSiPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeSiPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fStationNb,fStripNb);}
// ----------------------------------------------------------------------------
Bool_t ERQTelescope::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("strip")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------
Bool_t ERQTelescope::ProcessHits(FairVolume* vol) {
	        // Set constants for Birk's Law implentation
static const Double_t dP = 1.032 ;
static const Double_t BirkC1 =  0.013/dP;
static const Double_t BirkC2 =  9.6e-6/(dP * dP);

if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fELoss  = 0.;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fPID = gMC->TrackPid();
    gMC->CurrentVolID(fStripNb);
    gMC->CurrentVolOffID(1, fStationNb);
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
      AddSiPoint();
  }
}

    return kTRUE;
}

// ----------------------------------------------------------------------------
ClassImp(ERQTelescope)