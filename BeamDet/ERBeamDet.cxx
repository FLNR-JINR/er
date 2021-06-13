/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERBeamDet.h"

//-------------------------------------------------------------------------------------------------
ERBeamDet::ERBeamDet()
  : ERDetector("ERBeamDet", kTRUE)
{
  //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
ERBeamDet::ERBeamDet(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active, 1)
{
 //Это нужно сделать для того, чтобы геометрия в симуляции автоматом писалась в файл runtime db
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = verbose;
}
//-------------------------------------------------------------------------------------------------
ERBeamDet::~ERBeamDet() {
  for (auto* collection : {fToFPoints, fMWPCPoints, fTargetPoints}) {
    if (collection) {
      collection->Delete();
      delete collection;
    }
  }
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::Initialize() {
  FairDetector::Initialize();
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::EndOfEvent() {
  if (fVerboseLevel > 0)
        Print();
  Reset();
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    Fatal("Init", "IO manager is not set");
  fToFPoints = new TClonesArray("ERBeamDetTOFPoint");
  fMWPCPoints = new TClonesArray("ERBeamDetMWPCPoint");
  if (fSensitiveTargetIsSet) {
    fTargetPoints = new TClonesArray("ERBeamDetTargetPoint");
  }
  ioman->Register("BeamDetToFPoint","BeamDet", fToFPoints, kTRUE);
  ioman->Register("BeamDetMWPCPoint","BeamDet", fMWPCPoints, kTRUE);
  if (fSensitiveTargetIsSet) {
    ioman->Register("BeamDetTargetPoint","BeamDet", fTargetPoints, kTRUE);
  }
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERBeamDet::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fToFPoints;
  if (iColl == 0)
    return fMWPCPoints;
  if (fSensitiveTargetIsSet) {
    if (iColl == 0)
      return fTargetPoints;
  }
  return NULL;
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::Print(Option_t *option) const
{
  if(fToFPoints->GetEntriesFast() > 0) {
    for (Int_t iPoint = 0; iPoint < fToFPoints->GetEntriesFast(); iPoint++){
      ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)fToFPoints->At(iPoint);
      point->Print();
    }
  }
  if(fMWPCPoints->GetEntriesFast() > 0) {
    for (Int_t iPoint = 0; iPoint < fMWPCPoints->GetEntriesFast(); iPoint++){
      ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)fMWPCPoints->At(iPoint);
      point->Print();
    }
  }
  if(fSensitiveTargetIsSet && fTargetPoints->GetEntriesFast() > 0) {
    for (Int_t iPoint = 0; iPoint < fTargetPoints->GetEntriesFast(); iPoint++) {
      ERBeamDetTargetPoint* point = (ERBeamDetTargetPoint*)fTargetPoints->At(iPoint);
      point->Print();
    }
  }
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::Reset() {
  for (auto* collection : {fTargetPoints, fMWPCPoints, fToFPoints}) {
    if (collection)
      collection->Clear();
  }
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(FATAL) << "BeamDet: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERBeamDetTOFPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (ERBeamDetTOFPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[cl2->GetEntriesFast()]) ERBeamDetTOFPoint(*oldpoint);
  }
  LOG(FATAL) << "BeamDet: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ERBeamDetTargetPoint* ERBeamDet::AddTargetPoint() {
  if (!fTargetPoints)
    return nullptr;
  TClonesArray& clref = *fTargetPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetTargetPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fLightYield);
}
//-------------------------------------------------------------------------------------------------
ERBeamDetMWPCPoint* ERBeamDet::AddMWPCPoint() {
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
//-------------------------------------------------------------------------------------------------
ERBeamDetTOFPoint* ERBeamDet::AddTOFPoint() {
  TClonesArray& clref = *fToFPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERBeamDetTOFPoint(fEventID, fTrackID, fMot0TrackID, fPID,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime,fLength,fELoss, fLightYield, fToFNb);
}
//-------------------------------------------------------------------------------------------------
Bool_t ERBeamDet::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.Contains("gasStrip")) {
    return kTRUE;
  }
  if(volName.Contains("plastic")) {
    return kTRUE;
  }
  if(fSensitiveTargetIsSet && volName.Contains("targetBodyH2")) {
    return kTRUE;
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERBeamDet::ConstructGeometry() {
  fSetup->ConstructGeometry();
  SetGeometryFileName("beamdet.temp.root");
  ConstructRootGeometry();
  fSensitiveTargetIsSet = fSetup->CheckIfTargetIsSet();
}
//-------------------------------------------------------------------------------------------------
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
  }
  const Double_t stepEloss = gMC->Edep() * 1000; // [MeV] //Return the energy lost in the current step
  fELoss += stepEloss; 
  Double_t stepLightYield = 0;
  // Correction for all charge states
  if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0;
    // Apply correction for higher charge states
    if (TMath::Abs(gMC->TrackCharge())>=2)
      BirkC1Mod=BirkC1*7.2/12.6;
    else
      BirkC1Mod=BirkC1;

    if (gMC->TrackStep()>0) {
      Double_t dedxcm = stepEloss / gMC->TrackStep(); //[MeV/cm]
      stepLightYield= stepEloss / (1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
      fLightYield += stepLightYield;
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
      if(volName.Contains("plastic")) {
        gMC->CurrentVolID(fToFNb);
        AddTOFPoint();
        if (fMot0TrackID != -1) {
          gMC->StopTrack();
        }
      }
      if(volName.Contains("gasStrip")) {
        gMC->CurrentVolOffID(0, fMWPCWireNb);
        gMC->CurrentVolOffID(1, fMWPCPlaneNb);
        gMC->CurrentVolOffID(3, fMWPCNb);
        AddMWPCPoint();
      }
      if (fSensitiveTargetIsSet) {
        if(volName.Contains("targetBodyH2")) {
          AddTargetPoint();
        }
      }
    }
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERBeamDet)