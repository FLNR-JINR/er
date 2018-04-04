/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescope.h"

#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope() :
  ERDetector("ERQTelescope", kTRUE)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVerboseLevel = 1;
  fVersion = 1;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::ERQTelescope(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active,1)
{
  ResetParameters();
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fVersion = 1;
  fVerboseLevel = verbose;
}
//-------------------------------------------------------------------------------------------------
ERQTelescope::~ERQTelescope() {
/*  if (fSiPoint) {
    fSiPoint->Delete();
    delete fSiPoint;
  }
  if (fCsIPoint) {
    fCsIPoint->Delete();
    delete fCsIPoint;
  }
  */
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Initialize() {
  FairDetector::Initialize();
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiPoint* ERQTelescope::AddSiPoint(TClonesArray& clref) {
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeSiPoint(fEventID, fTrackID, fMot0TrackID, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fLength, fEloss, fSiStationNb, fSiStripNb);
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIPoint* ERQTelescope::AddCsIPoint(TClonesArray& clref) {
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeCsIPoint(fEventID, fTrackID, fMot0TrackID, fMass,
    TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
    TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
    TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
    TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
    fTime, fLength, fEloss, fCsIStationNb, fCsIBoxNb);
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ConstructGeometry() {
  fQTelescopeSetup->ConstructGeometry();
  SetGeometryFileName("QTelescope.temp.root");
  ConstructRootGeometry();
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescope::ProcessHits(FairVolume* vol) {
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fEloss  = 0.;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    // gMC->CurrentVolID(sensor);
    // gMC->CurrentVolOffID(1, sector);
  }

  fEloss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared())
	{
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    TString volName = gMC->CurrentVolName();
	  if (fEloss > 0.){
      if (volName.Contains("DoubleSi")) {
        Int_t xStripNb;
        Int_t yStripNb;
        gMC->CurrentVolOffID(0, yStripNb);
        gMC->CurrentVolOffID(1, xStripNb);
        gMC->CurrentVolOffID(2, fSiStationNb);
        if (volName.EndsWith("X")) {
          // swap X and Y strip numbers
          xStripNb = xStripNb ^ yStripNb;
          yStripNb = xStripNb ^ yStripNb;
          xStripNb = xStripNb ^ yStripNb;
        }
        fSiStripNb = xStripNb;
        AddSiPoint(*(fDoubleSiXPoints[fSiStationNb]));
        fSiStripNb = yStripNb;
        AddSiPoint(*(fDoubleSiYPoints[fSiStationNb]));
      }
      if (volName.Contains("SingleSi")) {
        gMC->CurrentVolOffID(0, fSiStripNb) ;
        gMC->CurrentVolOffID(1, fSiStationNb);
        AddSiPoint(*(fSingleSiPoints[fSiStationNb]));
      }
      if (volName.Contains("CsI")) {
        gMC->CurrentVolID(fCsIBoxNb);
        gMC->CurrentVolOffID(1, fCsIStationNb);
        AddCsIPoint(*(fCsIPoints[fCsIStationNb]));
      }
    }
  }
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::BeginEvent() {
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  TString branchName;
  if (!ioman)
    Fatal("Init", "IO manager is not set");
  Int_t iDoubleSi = 0; 
  Int_t iSingleSi = 0; 
  Int_t iCsI      = 0; 
  vector<TString>* sensVolumes = fQTelescopeSetup->GetComponentNames();
  for (Int_t i = 0; i < sensVolumes->size(); i++) {
    if (sensVolumes->at(i).BeginsWith("DoubleSi")) {
      fDoubleSiXPoints.push_back(new TClonesArray("ERQTelescopeSiPoint"));
      fDoubleSiYPoints.push_back(new TClonesArray("ERQTelescopeSiPoint"));
      branchName = "ERQTelescopeSiPoint_" + sensVolumes->at(i) + "_" + TString::Itoa(iDoubleSi, 10) + "_X";
      ioman->Register(branchName, "QTelescope", fDoubleSiXPoints.back(), kTRUE);
      branchName = "ERQTelescopeSiPoint_" + sensVolumes->at(i) + "_" + TString::Itoa(iDoubleSi++, 10) + "_Y";
      ioman->Register(branchName, "QTelescope", fDoubleSiYPoints.back(), kTRUE);
    }
    if (sensVolumes->at(i).BeginsWith("SingleSi")) {
      fSingleSiPoints.push_back(new TClonesArray("ERQTelescopeSiPoint"));
      branchName = "ERQTelescopeSiPoint_" + sensVolumes->at(i) + "_" + TString::Itoa(iSingleSi++, 10);
      ioman->Register(branchName, "QTelescope", fSingleSiPoints.back(), kTRUE);
    }
    if (sensVolumes->at(i).BeginsWith("CsI")) {
      fCsIPoints.push_back(new TClonesArray("ERQTelescopeCsIPoint"));
      branchName = "ERQTelescopeCsIPoint_" + sensVolumes->at(i) + "_" + TString::Itoa(iCsI++, 10);      
      ioman->Register(branchName, "QTelescope", fCsIPoints.back(), kTRUE);

    }
  }
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERQTelescope::GetCollection(Int_t iColl) const {
 /* if (iColl == 0)
    return fSiPoint;
  if (iColl == 0)
    return fCsIPoint;
*/
  return NULL;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Print(Option_t *option) const {
/*  if(fSiPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== Si Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fSiPoint->At(iPoint);
      point->Print();
    }
  }
  if(fCsIPoint->GetEntriesFast() > 0) {
    LOG(DEBUG) << "======== CsI Points ==================" << FairLogger::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeCsIPoint* point = (ERQTelescopeCsIPoint*)fCsIPoint->At(iPoint);
      point->Print();
    }
  }
*/
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::Reset() {
  LOG(INFO) << "  ERQTelescope::Reset()" << FairLogger::endl;
  for(auto &itDobleSiXPoints : fDoubleSiXPoints) {
    itDobleSiXPoints->Clear();
  }
  for(auto &itDobleSiYPoints : fDoubleSiYPoints) {
    itDobleSiYPoints->Clear();
  }
  for(auto &itSingleSiPoints : fSingleSiPoints) {
    itSingleSiPoints->Clear();
  }
  for(auto &itCsIPoints : fCsIPoints) {
    itCsIPoints->Clear();
  }
  ResetParameters();
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
/*  LOG(INFO) << "   ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)"
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "QTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERQTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERQTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERQTelescopeSiPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
*/
}
//-------------------------------------------------------------------------------------------------
Bool_t ERQTelescope::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.BeginsWith("Sensitive")) {
    return kTRUE;
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERQTelescope::ResetParameters() {
  LOG(INFO) << "   ERQTelescope::ResetParameters() " << FairLogger::endl;
};
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescope)

