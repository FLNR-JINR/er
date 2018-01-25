/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERGadast.h"

#include "TClonesArray.h"
#include "TVirtualMC.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERGadastGeoPar.h"

using namespace std;


//--------------------------------------------------------------------------------------------------
ERGadast::ERGadast(): 
  ERDetector("ERGadast", kTRUE),
  fCsIPoints(new TClonesArray("ERGadastCsIPoint")),
  fLaBrPoints(new TClonesArray("ERGadastLaBrPoint")),
  fGadastSteps(new TClonesArray("ERGadastStep")),
  fStoreSteps(kFALSE)
{
  flGeoPar->SetName( GetName());
  flGeoPar = new TList();
  fVerboseLevel = 1;
}

//--------------------------------------------------------------------------------------------------
ERGadast::ERGadast(const char* name, Bool_t active):
  ERDetector(name, active),
  fCsIPoints(new TClonesArray("ERGadastCsIPoint")),
  fLaBrPoints(new TClonesArray("ERGadastLaBrPoint")),
  fGadastSteps(new TClonesArray("ERGadastStep")),
  fStoreSteps(kFALSE)
{ 
  flGeoPar->SetName( GetName());
  flGeoPar = new TList();
  fVerboseLevel = 1;
}

//--------------------------------------------------------------------------------------------------
ERGadast::~ERGadast() {
  if (fCsIPoints) {
    fCsIPoints->Delete();
    delete fCsIPoints;
  }
  if (fLaBrPoints) {
    fLaBrPoints->Delete();
    delete fLaBrPoints;
  }
  if (fGadastSteps){
    fGadastSteps->Delete();
    delete fGadastSteps;
  }
}

//--------------------------------------------------------------------------------------------------
void ERGadast::Initialize() {
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  ERGadastGeoPar* par=(ERGadastGeoPar*)(rtdb->getContainer("ERGadastGeoPar"));
}

//--------------------------------------------------------------------------------------------------
Bool_t ERGadast::ProcessHits(FairVolume* vol) {
  //Start point
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in sen. vol.
    StartPoint();
  }

  if (fStoreSteps) {
    AddStep();
    return kTRUE;
  }
  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step
  
  //finish point
  if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) { 
    FinishPoint();
  }
  fStepNr++;
}

//--------------------------------------------------------------------------------------------------
void ERGadast::StartPoint() {
  fELoss  = 0.;
  fEventID = gMC->CurrentEvent();

  TLorentzVector pos, mom;

  gMC->TrackPosition(pos);
  gMC->TrackMomentum(mom);
  fPosIn = pos.Vect();
  fMomIn = mom.Vect();

  fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
  fTime   = gMC->TrackTime() * 1.0e09;// Return the current time track being transported
  fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
  fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2

  if (TString(gMC->CurrentVolName()).Contains("LaBrcell_cell"))
    fDetectorType = LaBr;
  else
    fDetectorType = CsI;

  fPDG = gMC->TrackPid();
  fStepNr = 0;
  if(fDetectorType == CsI){
    gMC->CurrentVolOffID(1, fCsICell);  
    gMC->CurrentVolOffID(2, fCsIBlock);
    gMC->CurrentVolOffID(3, fCsIWall);
  }
  else{
    gMC->CurrentVolID(fLaBrCell);
  }
}

//--------------------------------------------------------------------------------------------------
void ERGadast::FinishPoint() {
  TLorentzVector pos, mom;

  gMC->TrackPosition(pos);
  gMC->TrackMomentum(mom);
  fPosOut = pos.Vect();
  fMomOut = mom.Vect();

  if (fELoss > 0.){
    if(fDetectorType == 0)
      AddCsIPoint();
    if(fDetectorType == 1)
      AddLaBrPoint();
  }
}

//--------------------------------------------------------------------------------------------------
void ERGadast::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }

  Reset();
}

//--------------------------------------------------------------------------------------------------
void ERGadast::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "IO manager is not set" << FairLogger::endl;  
  ioman->Register("GadastCsIPoint","ERGadast", fCsIPoints, kTRUE);
  ioman->Register("GadastLaBrPoint","ERGadast", fLaBrPoints, kTRUE);
  ioman->Register("GadastStep","ERGadast", fGadastSteps, kTRUE);
}

//--------------------------------------------------------------------------------------------------
TClonesArray* ERGadast::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fCsIPoints;
  if (iColl == 1)
    return fLaBrPoints;
  return NULL;
}

//--------------------------------------------------------------------------------------------------
void ERGadast::Print(Option_t *option) const {
  if (fCsIPoints->GetEntriesFast() > 0){
    LOG(INFO) << "======== CsI Points ==================" << FairLogger::endl;
    for (Int_t i_point = 0; i_point < fCsIPoints->GetEntriesFast(); i_point++){
      ERGadastCsIPoint* point = (ERGadastCsIPoint*)fCsIPoints->At(i_point);
      point->Print();
    }
  }

  if (fLaBrPoints->GetEntriesFast() > 0){
    LOG(INFO) << "======== LaBr Points =================" << FairLogger::endl;
    for (Int_t i_point = 0; i_point < fLaBrPoints->GetEntriesFast(); i_point++){
      ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fLaBrPoints->At(i_point);
      point->Print();
    }
  }
}

//--------------------------------------------------------------------------------------------------
void ERGadast::Reset() {
  fCsIPoints->Clear();
  fLaBrPoints->Clear();
  fGadastSteps->Clear();
}

//--------------------------------------------------------------------------------------------------
void ERGadast::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "ERGadast: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERGadastCsIPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERGadastCsIPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERGadastCsIPoint(*oldpoint);
  }
  LOG(DEBUG) << "ERGadast: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}

//--------------------------------------------------------------------------------------------------
ERGadastCsIPoint* ERGadast::AddCsIPoint() {
  TClonesArray& clref = *fCsIPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastCsIPoint(fEventID, fTrackID, fMot0TrackID, fMass,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, fPDG, fCsIWall, fCsIBlock, fCsICell);
  
}

//--------------------------------------------------------------------------------------------------
ERGadastLaBrPoint* ERGadast::AddLaBrPoint() {
  TClonesArray& clref = *fLaBrPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastLaBrPoint(fEventID, fTrackID, fMot0TrackID, fMass,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, fPDG, fLaBrCell);
              
  
}

//--------------------------------------------------------------------------------------------------
ERGadastStep* ERGadast::AddStep(){
  
  TClonesArray& clref = *fGadastSteps;
  //Fill step info
  ExpertTrackingStatus trackStatus = ERGadastStep::GetTrackStatus();
  TArrayI processesID;
  gMC->StepProcesses(processesID);
  TLorentzVector curPosIn;
  TLorentzVector curMomIn;
  gMC->TrackPosition(curPosIn);
  gMC->TrackMomentum(curMomIn);


  return new(clref[fGadastSteps->GetEntriesFast()]) 
                                      ERGadastStep(fEventID, fStepNr, fTrackID, fMot0TrackID, 0,
                                      TVector3(curPosIn.X(),   curPosIn.Y(),   curPosIn.Z()),
                                      TVector3(curMomIn.X(),   curMomIn.Y(),   curMomIn.Z()),  
                                      gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(),
                                      fMass,trackStatus,gMC->Edep(),gMC->TrackCharge(),processesID);                                  
}

//--------------------------------------------------------------------------------------------------
Bool_t ERGadast::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.Contains("LaBrcell_cell")) {
    return kTRUE;
  }
  if(volName.Contains("onecell_cell")) { //CsI
    return kTRUE;
  }
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
ClassImp(ERGadast)
