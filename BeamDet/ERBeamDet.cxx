// -------------------------------------------------------------------------
// -----                        ERBeamDet source file                   -----
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

#include "ERBeamDet.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERBeamDet::ERBeamDet() :
  ERDetector("ERBeamDet", kTRUE),
  fDetPoints(NULL)
{
  fDetPoints = new TClonesArray("ERBeamDetPoint");
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERBeamDet::ERBeamDet(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active,1),
  fDetPoints(NULL)
 {
  fDetPoints = new TClonesArray("ERBeamDetPoint");
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = verbose;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
ERBeamDet::~ERBeamDet() {
  if (fDetPoints) {
    fDetPoints->Delete();
    delete fDetPoints;
  }
}

void ERBeamDet::Initialize()
{
  FairDetector::Initialize();
}

void ERBeamDet::BeginEvent() {
}
// -------------------------------------------------------------------------
void ERBeamDet::EndOfEvent() {
  if (fVerboseLevel > 0)
        Print();
  Reset();
}
// -------------------------------------------------------------------------
void ERBeamDet::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
        Fatal("Init", "IO manager is not set");
  ioman->Register("BeamDetPoint","BeamDet", fDetPoints, kTRUE);
}
// ----------------------------------------------------------------------------
TClonesArray* ERBeamDet::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fDetPoints;
  else
    return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERBeamDet::Print(Option_t *option) const
{
  for (Int_t iPoint = 0; iPoint < fDetPoints->GetEntriesFast(); iPoint++){
    ERBeamDetPoint* point = (ERBeamDetPoint*)fDetPoints->At(iPoint);
    point->Print();
  }
}
// ----------------------------------------------------------------------------
void ERBeamDet::Reset() {
  fDetPoints->Clear();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERBeamDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  std::cout << "BeamDet: " << nEntries << " entries to add" << std::endl;
  TClonesArray& clref = *cl2;
  ERBeamDetPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERBeamDetPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERBeamDetPoint(*oldpoint);
  }
  std::cout << "BeamDet: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
}
// ----------------------------------------------------------------------------
  ERBeamDetPoint* ERBeamDet::AddPoint(Int_t pType, Int_t eventID, Int_t trackID,
                          Int_t mot0trackID,
                          Int_t pid,
                          TVector3 posIn,
                          TVector3 posOut, TVector3 momIn,
                          TVector3 momOut, Double_t time,
                          Double_t length, Double_t eLoss, Double_t lightYield){
  TClonesArray& clref = *fDetPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetPoint(pType,eventID, trackID, mot0trackID,pid,posIn,posOut,
                                     momIn,momOut,time,length,eLoss, lightYield);
}
// ----------------------------------------------------------------------------
Bool_t ERBeamDet::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("gas")) {
    return kTRUE;
  }
  if(volName.Contains("plate")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------
ClassImp(ERBeamDet)

Bool_t ERBeamDet::ProcessHits(FairVolume* vol) {
	        // Set constants for Birk's Law implentation
static const Double_t dP = 1.032 ;
static const Double_t BirkC1 =  0.013/dP;
static const Double_t BirkC2 =  9.6e-6/(dP * dP);

if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fELoss  = 0.;
    fLightYield = 0;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fPID = gMC->TrackPid();
}

fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

Double_t		  curLightYield = 0;


// Correction for all charge states
if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0;
    // Apply correction for higher charge states
      if (TMath::Abs(gMC->TrackCharge())>=2)
        BirkC1Mod=BirkC1*7.2/12.6;
      else
        BirkC1Mod=BirkC1;

    if (gMC->TrackStep()>0)
    {
      Double_t dedxcm=gMC->Edep()*1000./gMC->TrackStep(); //[MeV/cm]
      curLightYield=gMC->Edep()*1000./(1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
      curLightYield /= 1000.; //[GeV]
      fLightYield+=curLightYield;
    }
}

if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume
    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
    gMC->IsTrackDisappeared())
{
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    TString volName = gMC->CurrentVolName();

    if(volName.Contains("plate"))
        fPType = 0;    
    if(volName.Contains("gas"))
        fPType = 1;
    if (fELoss > 0.){
       	AddPoint(fPType, fEventID, fTrackID, fMot0TrackID, fPID,
        TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
        TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
        TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
        TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
        fTime, fLength, fELoss, fLightYield);
    }
}

    return kTRUE;
}