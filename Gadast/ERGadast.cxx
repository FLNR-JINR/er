// -------------------------------------------------------------------------
// -----                        ERGadast source file                   -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------
#include "ERGadast.h"

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"


// -----   Default constructor   -------------------------------------------
ERGadast::ERGadast() : FairDetector("ERGadast", kTRUE)
{
  LOG(INFO) << "  DETECTOR::ERGadast()" << FairLogger::endl;
  
  ResetParameters();
  fDetectorPoints = new TClonesArray("ERGadastPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
  
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERGadast::ERGadast(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
  {
  LOG(INFO) << "  DETECTOR::ERGadast(const char* name, Bool_t active, Int_t verbose) " 
            << FairLogger::endl;
  
  ResetParameters();
  fDetectorPoints = new TClonesArray("ERGadastPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
  
}

ERGadast::~ERGadast() {
  LOG(INFO) << "  DETECTOR::~ERGadast()" << FairLogger::endl;
  
  if (fDetectorPoints) {
    fDetectorPoints->Delete();
    delete fDetectorPoints;
  }
  
}

void ERGadast::Initialize()
{
  LOG(INFO) << "  DETECTOR::Initialize()" << FairLogger::endl;
  FairDetector::Initialize();
}


Bool_t ERGadast::ProcessHits(FairVolume* vol) {
  LOG(INFO) << "  DETECTOR::ProcessHits(FairVolume* vol)" << FairLogger::endl;
  
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
  }
  
  eLoss += gMC->Edep() * 1E+6; // keV //Return the energy lost in the current step
  
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
                time, length, eLoss);
    }
  }
  
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERGadast::BeginEvent() {
  LOG(INFO) << "  DETECTOR::BeginEvent()" << FairLogger::endl;
}


void ERGadast::EndOfEvent() {
  LOG(INFO) << "  DETECTOR::EndOfEvent()" << FairLogger::endl;
  
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
  
}

// -----   Public method Register   -------------------------------------------
void ERGadast::Register() {
  LOG(INFO) << "  DETECTOR::Register()" << FairLogger::endl;
  
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("ERGadastPoint","ERGadast", fDetectorPoints, kTRUE);
  
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERGadast::GetCollection(Int_t iColl) const {
  LOG(INFO) << "  DETECTOR::GetCollection(Int_t iColl)" << FairLogger::endl;
  
  if (iColl == 0) 
    return fDetectorPoints;
  else
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ERGadast::Print(Option_t *option) const
{
  LOG(INFO) << "  DETECTOR::Print" << FairLogger::endl;
  
  for (Int_t i_point = 0; i_point < fDetectorPoints->GetEntriesFast(); i_point++){
    ERGadastPoint* point = (ERGadastPoint*)fDetectorPoints->At(i_point);
    point->Print();
  }
  
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERGadast::Reset() {
  LOG(INFO) << "  DETECTOR::Reset()" << FairLogger::endl;
  
  fDetectorPoints->Clear();
  ResetParameters();
  
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERGadast::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(INFO) << "   DETECTOR::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)" 
            << FairLogger::endl;
  
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "ERGadast: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERGadastPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERGadastPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERGadastPoint(*oldpoint);
  }
  LOG(INFO) << "ERGadast: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
  
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERGadastPoint* ERGadast::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fDetectorPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastPoint(eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss);
	
}

// ----------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ----------------------------------
void ERGadast::ConstructGeometry() {
  LOG(INFO) << "  DETECTOR::ConstructGeometry()" << FairLogger::endl;
  
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing ERGadast geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERGadast::CheckIfSensitive(std::string name)
{
  LOG(INFO) << "DETECTOR::CheckIfSensitive(std::string name)" << FairLogger::endl;
  
  TString volName = name;
  if(volName.Contains("module")) {
    return kTRUE;
  }
  
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERGadast::ResetParameters() {
  LOG(INFO) << "   DETECTOR::ResetParameters() " << FairLogger::endl;
};
// ----------------------------------------------------------------------------
ClassImp(ERGadast)
