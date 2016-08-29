// -------------------------------------------------------------------------
// -----                        ERND source file                   -----
// -----                  Created data  by developerName               -----
// -------------------------------------------------------------------------
#include "ERND.h"

#include <iostream>
using namespace std;

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"


// -----   Default constructor   -------------------------------------------
ERND::ERND() : 
  FairDetector("ERND", kTRUE),
  fNDPoints(NULL)
{
  LOG(INFO) << "  ERND::ERND()" << FairLogger::endl;
  ResetParameters();
  fNDPoints = new TClonesArray("ERNDPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERND::ERND(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose),
  fNDPoints(NULL)
  {
  ResetParameters();
  fNDPoints = new TClonesArray("ERNDPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
}

ERND::~ERND() {
  if (fNDPoints) {
    fNDPoints->Delete();
    delete fNDPoints;
  }
}

void ERND::Initialize()
{
  FairDetector::Initialize();
}


Bool_t ERND::ProcessHits(FairVolume* vol) {  
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn, posOut;    //!  position
  static TLorentzVector momIn, momOut;    //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss

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
    cerr << "Start point" << endl;
  }
  
  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    
	  if (eLoss > 0.){
      cerr << "End point" << endl;
      AddPoint( eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss);
    }
  }
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERND::BeginEvent() {
}


void ERND::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}

// -----   Public method Register   -------------------------------------------
void ERND::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("ERNDPoint","ERND", fNDPoints, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERND::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNDPoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ERND::Print(Option_t *option) const
{
  for (Int_t i_point = 0; i_point < fNDPoints->GetEntriesFast(); i_point++){
    ERNDPoint* point = (ERNDPoint*)fNDPoints->At(i_point);
    point->Print();
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERND::Reset() {
  LOG(INFO) << "  ERND::Reset()" << FairLogger::endl;
  fNDPoints->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERND::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(INFO) << "   ERND::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)" 
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "decector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERNDPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERNDPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERNDPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERNDPoint* ERND::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fNDPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERNDPoint(eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss);
	
}
// ----------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ----------------------------------
void ERND::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    cout << "Constructing ERND geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    cerr << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERND::CheckIfSensitive(std::string name)
{
  cout << name << endl;
  TString volName = name;
  if(volName.Contains("Stilbene")) {
    return kTRUE;
  }

  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERND::ResetParameters() {
  LOG(INFO) << "   ERND::ResetParameters() " << FairLogger::endl;
};
// ----------------------------------------------------------------------------
ClassImp(ERND)
