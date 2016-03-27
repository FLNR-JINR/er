// -------------------------------------------------------------------------
// -----                        ERmuSi source file                   -----
// -----                  Created data  by developerName               -----
// -------------------------------------------------------------------------
#include "ERmuSi.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"


// -----   Default constructor   -------------------------------------------
ERmuSi::ERmuSi() : FairDetector("ERmuSi", kTRUE)
{
  ResetParameters();
  fERmuSiPoints = new TClonesArray("ERmuSiPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERmuSi::ERmuSi(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
  {
  ResetParameters();
  fERmuSiPoints = new TClonesArray("ERmuSiPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
}

ERmuSi::~ERmuSi() {
  if (fERmuSiPoints) {
    fERmuSiPoints->Delete();
    delete fERmuSiPoints;
  }
}

void ERmuSi::Initialize()
{
  FairDetector::Initialize();
}


Bool_t ERmuSi::ProcessHits(FairVolume* vol) {
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn, posOut;    //!  position
  static TLorentzVector momIn, momOut;    //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss
  static Int_t          station;

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
    Int_t curVolId =  gMC->CurrentVolID(station);
  }
  
  eLoss += gMC->Edep(); // keV //Return the energy lost in the current step
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    
	  if (eLoss > 0.){
      AddPoint( eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, station);
    }
  }
  
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERmuSi::BeginEvent() {
}


void ERmuSi::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
  
}

// -----   Public method Register   -------------------------------------------
void ERmuSi::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("muSiPoint","ERmuSi", fERmuSiPoints, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERmuSi::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fERmuSiPoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ERmuSi::Print(Option_t *option) const
{
  /*
  for (Int_t i_point = 0; i_point < fERmuSiPoints->GetEntriesFast(); i_point++){
    ERmuSiPoint* point = (ERmuSiPoint*)fERmuSiPoints->At(i_point);
    point->Print();
  }
  */
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERmuSi::Reset() {
 
  fERmuSiPoints->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERmuSi::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "decector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERmuSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERmuSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERmuSiPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
  
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERmuSiPoint* ERmuSi::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss, Int_t station) {
  TClonesArray& clref = *fERmuSiPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERmuSiPoint(size, eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss, station);
	
}

// ----------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ----------------------------------
void ERmuSi::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing ERmuSi geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERmuSi::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("station")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSi::ResetParameters() {
};
// ----------------------------------------------------------------------------
ClassImp(ERmuSi)
