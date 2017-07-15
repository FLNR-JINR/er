// -------------------------------------------------------------------------
// -----                        ERRTelescope source file                   -----
// -----                  Created data  by developerName               -----
// -------------------------------------------------------------------------
#include "ERRTelescope.h"

#include <iostream>
using namespace std;

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
// -----   Default constructor   -------------------------------------------
ERRTelescope::ERRTelescope() : 
  FairDetector("ERRTelescope", kTRUE),
  fRTelescopePoints(NULL)
{
  ResetParameters();
  fRTelescopePoints = new TClonesArray("ERRTelescopePoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERRTelescope::ERRTelescope(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose),
  fRTelescopePoints(NULL)
  {
  ResetParameters();
  fRTelescopePoints = new TClonesArray("ERRTelescopePoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
}

ERRTelescope::~ERRTelescope() {
  if (fRTelescopePoints) {
    fRTelescopePoints->Delete();
    delete fRTelescopePoints;
  }
}

void ERRTelescope::Initialize()
{
  FairDetector::Initialize();
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
    fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    gMC->CurrentVolID(fSensorNb);
    gMC->CurrentVolOffID(1, fSectorNb);
  }
  
  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
	  if (fELoss > 0.){
      AddPoint();
    }
  }
  return kTRUE;
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
  ioman->Register("RTelescopePoint","RTelescope", fRTelescopePoints, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERRTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fRTelescopePoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ERRTelescope::Print(Option_t *option) const
{
  for (Int_t i_point = 0; i_point < fRTelescopePoints->GetEntriesFast(); i_point++){
    ERRTelescopePoint* point = (ERRTelescopePoint*)fRTelescopePoints->At(i_point);
    point->Print();
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERRTelescope::Reset() {
  LOG(INFO) << "  ERRTelescope::Reset()" << FairLogger::endl;
  fRTelescopePoints->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERRTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "RTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERRTelescopePoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERRTelescopePoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERRTelescopePoint(*oldpoint);
  }
  LOG(INFO) << "RTelescope: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERRTelescopePoint* ERRTelescope::AddPoint() {
  TClonesArray& clref = *fRTelescopePoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERRTelescopePoint(fEventID, fTrackID, fMot0TrackID, fMass,
                TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, 
              fSectorNb, fSensorNb);
	
}
// ----------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ----------------------------------
void ERRTelescope::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    cout << "Constructing ERRTelescope geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    cerr << "Geometry file name is not set" << FairLogger::endl;
  }
  
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
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescope::ResetParameters() {
  LOG(INFO) << "   ERRTelescope::ResetParameters() " << FairLogger::endl;
};
// ----------------------------------------------------------------------------
ClassImp(ERRTelescope)
