#include "Det.h"

#include <iostream>
using namespace std;

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
// -----   Default constructor   -------------------------------------------
Det::Det() : FairDetector("Det", kTRUE)
{
  fPoints = new TClonesArray("DetPoint",1000);
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
Det::Det(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
{
  fPoints = new TClonesArray("DetPoint",1000);
}

Det::~Det(){
  if (fPoints) {
    fPoints->Delete();
    delete fPoints;
  }
}

void Det::Initialize()
{
  FairDetector::Initialize();
}

 Bool_t Det::ProcessHits(FairVolume* vol) {
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

  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step

if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the thresho
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
void Det::BeginEvent() {
}

void Det::EndOfEvent() {
  Print();
  Reset();
}

// -----   Public method Register   -------------------------------------------
void Det::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");
  ioman->Register("DetPoints","Det", fPoints, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* Det::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fPoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void Det::Print(Option_t *option) const
{
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void Det::Reset() {
  LOG(INFO) << "  Det::Reset()" << FairLogger::endl;
  fPoints->Clear();

}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void Det::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
}
// ----------------------------------------------------------------------------
// -----   Public method ConstructGeometry   ----------------------------------
void Det::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    cout << "Constructing Det geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    cerr << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t Det::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("station")) {
    return kTRUE;
  }
  return kFALSE;

}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
DetPoint* Det::AddPoint(Int_t eventID, Int_t trackID,
            Int_t mot0trackID,
            Double_t mass,
            TVector3 posIn,
            TVector3 posOut, TVector3 momIn,
            TVector3 momOut, Double_t time,
            Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) DetPoint(eventID, trackID, mot0trackID, mass,
            posIn, posOut, momIn, momOut, time, length, eLoss);
  
}

ClassImp(Det)
