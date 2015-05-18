// -------------------------------------------------------------------------
// -----                        detector source file                   -----
// -----                  Created data  by developerName               -----
// -------------------------------------------------------------------------
#include "detector.h"
/*
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
*/

// -----   Default constructor   -------------------------------------------
detector::detector() : FairDetector("detector", kTRUE)
{
  LOG(INFO) << "  DETECTOR::detector()" << FairLogger::endl;
  /*
  ResetParameters();
  fDetectorPoints = new TClonesArray("detectorPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
  */
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
detector::detector(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
  {
  LOG(INFO) << "  DETECTOR::detector(const char* name, Bool_t active, Int_t verbose) " 
            << FairLogger::endl;
  /*
  ResetParameters();
  fDetectorPoints = new TClonesArray("detectorPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
  */
}

detector::~detector() {
  LOG(INFO) << "  DETECTOR::~detector()" << FairLogger::endl;
  /*
  if (fDetectorPoints) {
    fDetectorPoints->Delete();
    delete fDetectorPoints;
  }
  */
}

void detector::Initialize()
{
  LOG(INFO) << "  DETECTOR::Initialize()" << FairLogger::endl;
  FairDetector::Initialize();
}


Bool_t detector::ProcessHits(FairVolume* vol) {
  LOG(INFO) << "  DETECTOR::ProcessHits(FairVolume* vol)" << FairLogger::endl;
  /*
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
  */
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void detector::BeginEvent() {
  LOG(INFO) << "  DETECTOR::BeginEvent()" << FairLogger::endl;
}


void detector::EndOfEvent() {
  LOG(INFO) << "  DETECTOR::EndOfEvent()" << FairLogger::endl;
  /*
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
  */
}

// -----   Public method Register   -------------------------------------------
void detector::Register() {
  LOG(INFO) << "  DETECTOR::Register()" << FairLogger::endl;
  /*
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("detectorPoint","detector", fDetectorPoints, kTRUE);
  */
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* detector::GetCollection(Int_t iColl) const {
  LOG(INFO) << "  DETECTOR::GetCollection(Int_t iColl)" << FairLogger::endl;
  /*
  if (iColl == 0) 
    return fDetectorPoints;
  else 
    */
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void detector::Print(Option_t *option) const
{
  LOG(INFO) << "  DETECTOR::Print" << FairLogger::endl;
  /*
  for (Int_t i_point = 0; i_point < fDetectorPoints->GetEntriesFast(); i_point++){
    detectorPoint* point = (detectorPoint*)fDetectorPoints->At(i_point);
    point->Print();
  }
  */
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void detector::Reset() {
  LOG(INFO) << "  DETECTOR::Reset()" << FairLogger::endl;
  /*)
  fDetectorPoints->Clear();
  ResetParameters();
  */
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void detector::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(INFO) << "   DETECTOR::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)" 
            << FairLogger::endl;
  /*
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "decector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  detectorPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (detectorPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) detectorPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
  */
}
// ----------------------------------------------------------------------------
/*
// -----   Private method AddPoint   --------------------------------------------
detectorPoint* detector::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fDetectorPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) detectorPoint(eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss);
	
}
*/
// ----------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ----------------------------------
void detector::ConstructGeometry() {
  LOG(INFO) << "  DETECTOR::ConstructGeometry()" << FairLogger::endl;
  
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing detector geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t detector::CheckIfSensitive(std::string name)
{
  LOG(INFO) << "DETECTOR::CheckIfSensitive(std::string name)" << FairLogger::endl;
  /*
  TString volName = name;
  if(volName.Contains("module")) {
    return kTRUE;
  }
  */
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void detector::ResetParameters() {
  LOG(INFO) << "   DETECTOR::ResetParameters() " << FairLogger::endl;
};
// ----------------------------------------------------------------------------
ClassImp(detector)
