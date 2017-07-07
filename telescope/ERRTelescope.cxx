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
    eLoss  = 0.;
    eventID = gMC->CurrentEvent();
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID  = gMC->GetStack()->GetCurrentTrackNumber();
    time   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    gMC->CurrentVolID(sensor);
    gMC->CurrentVolOffID(1, sector);
  }
  
  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
	  if (eLoss > 0.){
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
  LOG(INFO) << "   ERRTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)" 
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "decector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERRTelescopePoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERRTelescopePoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERRTelescopePoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERRTelescopePoint* ERRTelescope::AddPoint() {
  TClonesArray& clref = *fRTelescopePoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERRTelescopePoint(eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, sector, sensor);
	
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
