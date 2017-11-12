// -------------------------------------------------------------------------
// -----                        ERGadast source file                   -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------
#include "ERGadast.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
#include "TGeoMatrix.h"
#include "TLorentzVector.h"
#include "ERGadastGeoPar.h"

#include <iostream>
using namespace std;


// -----   Default constructor   -------------------------------------------
ERGadast::ERGadast(): 
  ERDetector("ERGadast", kTRUE),
  fCsIPoints(new TClonesArray("ERGadastCsIPoint")),
  fLaBrPoints(new TClonesArray("ERGadastLaBrPoint")),
  fGadastSteps(new TClonesArray("ERGadastStep")),
  fVersion(1),
  fStoreSteps(kFALSE),
  fHCsIElossInEvent(NULL),
  fHLaBrElossInEvent(NULL)
{
  ResetParameters();
  flGeoPar->SetName( GetName());
  flGeoPar = new TList();
  fVerboseLevel = 1;
}

//-------------------------------------------------------------------------

//-----   Standard constructor   ------------------------------------------
ERGadast::ERGadast(const char* name, Bool_t active) 
  : ERDetector(name, active),
  fCsIPoints(new TClonesArray("ERGadastCsIPoint")),
  fLaBrPoints(new TClonesArray("ERGadastLaBrPoint")),
  fGadastSteps(new TClonesArray("ERGadastStep")),
  fVersion(1),
  fStoreSteps(kFALSE),
  fHCsIElossInEvent(NULL),
  fHLaBrElossInEvent(NULL)
{ 
  ResetParameters();
  flGeoPar->SetName( GetName());
  flGeoPar = new TList();
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
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
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void ERGadast::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  ERGadastGeoPar* par=(ERGadastGeoPar*)(rtdb->getContainer("ERGadastGeoPar"));

  fHCsIElossInEvent = new TH1F("fHCsIElossInEvent", "fHCsIElossInEvent",1000, 0., 0.005);
  fHLaBrElossInEvent = new TH1F("fHLaBrElossInEvent", "fHLaBrElossInEvent",1000, 0., 0.01);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Bool_t ERGadast::ProcessHits(FairVolume* vol) {

  //Start point
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    StartPoint();
  }

  if (fStoreSteps){
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

// -----   Private method StartPoint   -----------------------------------------
void ERGadast::StartPoint(){
    fELoss  = 0.;
    fEventID = gMC->CurrentEvent();

    TLorentzVector pos, mom;

    gMC->TrackPosition(pos);
    gMC->TrackMomentum(mom);
    fPosIn = pos.Vect();
    fMomIn = mom.Vect();

    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
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
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ERGadast::FinishPoint(){
  TLorentzVector pos, mom;

  gMC->TrackPosition(pos);
  gMC->TrackMomentum(mom);
  fPosOut = pos.Vect();
  fMomOut = mom.Vect();

  if (fELoss > 0.){
    if(fDetectorType == 0){
      AddCsIPoint();
      fCsIElossInEvent += fELoss;
    }
    if(fDetectorType == 1){
      AddLaBrPoint();
      fLaBrElossInEvent += fELoss;
    }
  }
}
//------------------------------------------------------------------------------

// -----   Public method BeginOfEvent   -----------------------------------------
void ERGadast::BeginEvent() {
  fCsIElossInEvent = 0;
  fLaBrElossInEvent = 0;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERGadast::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }

  if (fCsIElossInEvent > 0)
    fHCsIElossInEvent->Fill(fCsIElossInEvent);

  if (fLaBrElossInEvent > 0)
    fHLaBrElossInEvent->Fill(fLaBrElossInEvent);

  Reset();
}

// -----   Public method Register   -------------------------------------------
void ERGadast::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	 Fatal("Init", "IO manager is not set");	
  ioman->Register("GadastCsIPoint","ERGadast", fCsIPoints, kTRUE);
  ioman->Register("GadastLaBrPoint","ERGadast", fLaBrPoints, kTRUE);
  ioman->Register("GadastStep","ERGadast", fGadastSteps, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERGadast::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fCsIPoints;
  if (iColl == 1)
    return fLaBrPoints;
  return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void ERGadast::Print(Option_t *option) const
{
  if (fCsIPoints->GetEntriesFast() > 0){
    std::cout << "======== CsI Points ==================" << std::endl;
    for (Int_t i_point = 0; i_point < fCsIPoints->GetEntriesFast(); i_point++){
      ERGadastCsIPoint* point = (ERGadastCsIPoint*)fCsIPoints->At(i_point);
      point->Print();
    }
  }

  if (fLaBrPoints->GetEntriesFast() > 0){
    std::cout << "======== LaBr Points =================" << std::endl;
    for (Int_t i_point = 0; i_point < fLaBrPoints->GetEntriesFast(); i_point++){
      ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fLaBrPoints->At(i_point);
      point->Print();
    }
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERGadast::Reset() {
  fCsIPoints->Clear();
  fLaBrPoints->Clear();
  fGadastSteps->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERGadast::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  std::cout << "ERGadast: " << nEntries << " entries to add" << std::endl;
  TClonesArray& clref = *cl2;
  ERGadastCsIPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERGadastCsIPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERGadastCsIPoint(*oldpoint);
  }
  std::cout << "ERGadast: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERGadastCsIPoint* ERGadast::AddCsIPoint(){
  
  TClonesArray& clref = *fCsIPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastCsIPoint(fEventID, fTrackID, fMot0TrackID, fMass,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, fPDG, fCsIWall, fCsIBlock, fCsICell, fMeshElement);
	
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERGadastLaBrPoint* ERGadast::AddLaBrPoint(){
  
  TClonesArray& clref = *fLaBrPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastLaBrPoint(fEventID, fTrackID, fMot0TrackID, fMass,
              TVector3(fPosIn.X(),  fPosIn.Y(), fPosIn.Z()),
              TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
              TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
              TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
              fTime, fLength, fELoss, fPDG, fMeshElement);
              
  
}
// ----------------------------------------------------------------------------

// -----   Private method AddStep   --------------------------------------------
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


  return new(clref[fGadastSteps->GetEntriesFast()]) ERGadastStep(fEventID, fStepNr, fTrackID, fMot0TrackID, 0,
                                      TVector3(curPosIn.X(),   curPosIn.Y(),   curPosIn.Z()),
                                      TVector3(curMomIn.X(),   curMomIn.Y(),   curMomIn.Z()),  
                                      gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(),fMass, 
                                      trackStatus, gMC->Edep(),gMC->TrackCharge(), processesID);
                                            
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
Bool_t ERGadast::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("LaBrcell_cell")) {
    return kTRUE;
  }
  if(volName.Contains("onecell_cell")) { //CsI
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

void ERGadast::WriteHistos(){
  fHCsIElossInEvent->Write();
  fHLaBrElossInEvent->Write();
}

// ----------------------------------------------------------------------------
void ERGadast::ResetParameters() {
};
// ----------------------------------------------------------------------------
ClassImp(ERGadast)
