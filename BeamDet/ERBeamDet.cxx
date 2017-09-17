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
  fTOFPoints(NULL), 
  fMWPCPoints(NULL),
  fTargetPoints(NULL)
{
  fTOFPoints    = new TClonesArray("ERBeamDetTOFPoint");
  fMWPCPoints   = new TClonesArray("ERBeamDetMWPCPoint");
  fTargetPoints = new TClonesArray("ERBeamDetTargetPoint");

  fBeamDetMCProjectile = new ERBeamDetParticle(); 
  fBeamDetMCTrack      = new ERBeamDetTrack();
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERBeamDet::ERBeamDet(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active, 1),
    fTOFPoints(NULL), 
    fMWPCPoints(NULL),
    fTargetPoints(NULL)
{
  fTOFPoints    = new TClonesArray("ERBeamDetTOFPoint");
  fMWPCPoints   = new TClonesArray("ERBeamDetMWPCPoint");
  fTargetPoints = new TClonesArray("ERBeamDetTargetPoint");

  fBeamDetMCProjectile = new ERBeamDetParticle(); 
  fBeamDetMCTrack      = new ERBeamDetTrack();
 //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());

  fVerboseLevel = verbose;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
ERBeamDet::~ERBeamDet() {
  if (fTOFPoints) {
    fTOFPoints->Delete();
    delete fTOFPoints;
  }
  if (fMWPCPoints) {
    fMWPCPoints->Delete();
    delete fMWPCPoints;
  } 
  if (fTargetPoints) {
    fTargetPoints->Delete();
    delete fTargetPoints;
  }
  if(fBeamDetMCTrack) {
    delete fBeamDetMCTrack;
  }   
  if(fBeamDetMCProjectile) {
    delete fBeamDetMCProjectile;
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
  ioman->Register("BeamDetTOFPoint","BeamDet", fTOFPoints, kTRUE);
  ioman->Register("BeamDetMWPCPoint","BeamDet", fMWPCPoints, kTRUE);
  ioman->Register("BeamDetTargetPoint","BeamDet", fTargetPoints, kTRUE);

  ioman->Register("BeamDetMCParticle.", "BeamDet MC Particle", fBeamDetMCProjectile, kTRUE);
  ioman->Register("BeamDetMCTrack.", "BeamDet MC track", fBeamDetMCTrack, kTRUE);
}
// ----------------------------------------------------------------------------
TClonesArray* ERBeamDet::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fTOFPoints;
  if (iColl == 0)
    return fMWPCPoints;
  if (iColl == 0)
    return fTargetPoints;
  return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERBeamDet::Print(Option_t *option) const
{
  if(fTOFPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== TOF Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fTOFPoints->GetEntriesFast(); iPoint++){
      ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)fTOFPoints->At(iPoint);
      point->Print();
    }
  }
  if(fMWPCPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== MWPC Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fMWPCPoints->GetEntriesFast(); iPoint++){
      ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)fMWPCPoints->At(iPoint);
      point->Print();
    }
  }
  if(fTargetPoints->GetEntriesFast() > 0)
  {
    std::cout << "======== Target Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fTargetPoints->GetEntriesFast(); iPoint++){
      ERBeamDetTargetPoint* point = (ERBeamDetTargetPoint*)fTargetPoints->At(iPoint);
      point->Print();
    }
  }
}
// ----------------------------------------------------------------------------
void ERBeamDet::Reset() {
  fTOFPoints->Clear();
  fMWPCPoints->Clear();
  fTargetPoints->Clear();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERBeamDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  std::cout << "BeamDet: " << nEntries << " entries to add" << std::endl;
  TClonesArray& clref = *cl2;
  ERBeamDetTOFPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERBeamDetTOFPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERBeamDetTOFPoint(*oldpoint);
  }
  std::cout << "BeamDet: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
}

ERBeamDetTargetPoint* ERBeamDet::AddTargetPoint()
{
  TClonesArray& clref = *fTargetPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetTargetPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fLightYield);
}

ERBeamDetMWPCPoint* ERBeamDet::AddMWPCPoint()
{
  TClonesArray& clref = *fMWPCPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetMWPCPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fLightYield,
              fMWPCNb, fMWPCPlaneNb, fMWPCWireNb); 
}

ERBeamDetTOFPoint* ERBeamDet::AddTOFPoint()
{
  TClonesArray& clref = *fTOFPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetTOFPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fLightYield, fTofNb);
}
// ----------------------------------------------------------------------------
Bool_t ERBeamDet::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("gasStrip")) {
    return kTRUE;
  }
  if(volName.Contains("plastic")) {
    return kTRUE;
  }
  if(volName.Contains("targetH2")) {
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

      TString volName = gMC->CurrentVolName();
      if(fIonPIDIsSet && volName.Contains("targetH2") && fPID == fIonPID) {
        fBeamDetMCProjectile->AddParameters(fPID, fMomIn, 1);
        fBeamDetMCTrack->AddParameters(fPosIn.X(), fPosIn.Y(), fPosIn.Z(), fPosIn.Vect());   
      }
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
    if (fELoss > 0.){
      if(volName.Contains("plastic"))
      {
        gMC->CurrentVolID(fTofNb);
        AddTOFPoint();
      }
      if(volName.Contains("gasStrip"))
      {
        gMC->CurrentVolOffID(0, fMWPCWireNb);
        gMC->CurrentVolOffID(1, fMWPCPlaneNb);
        gMC->CurrentVolOffID(3, fMWPCNb);
        AddMWPCPoint();
      }
      if(volName.Contains("targetH2"))
      {
        AddTargetPoint();
      }
    }
  }
  return kTRUE;
}