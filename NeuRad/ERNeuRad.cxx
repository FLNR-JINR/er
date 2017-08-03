/********************************************************************************
 *    Copyright (C) Joint Institute for Nuclear Research                        *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRad.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TGeoMatrix.h"
#include "TString.h"

#include "ERNeuRadGeoPar.h"
#include "ERMCEventHeader.h"

#include <iostream>
using namespace std;


// -----   Default constructor   -------------------------------------------
ERNeuRad::ERNeuRad() : ERDetector("ERNeuRad", kTRUE),
  fStorePrimarySteps(kFALSE),
  fStoreAllSteps(kFALSE),
  fNeuRadPoints(NULL),
  fNeuRadFirstStep(NULL),
  fNeuRadSteps(NULL)
{
  ResetParameters();
  fNeuRadPoints = new TClonesArray("ERNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ERNeuRadStep");
  fNeuRadSteps = new TClonesArray("ERNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
//-------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRad::ERNeuRad(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active),
  fVerbose(verbose),
  fStorePrimarySteps(kFALSE),
  fStoreAllSteps(kFALSE)
  {
  ResetParameters();
  fNeuRadPoints = new TClonesArray("ERNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ERNeuRadStep");
  fNeuRadSteps = new TClonesArray("ERNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
}
//-------------------------------------------------------------------------
ERNeuRad::~ERNeuRad() {
  if (fNeuRadPoints) {
    fNeuRadPoints->Delete();
    delete fNeuRadPoints;
  }
}
//-------------------------------------------------------------------------
void ERNeuRad::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  ERNeuRadGeoPar* par=(ERNeuRadGeoPar*)(rtdb->getContainer("ERNeuRadGeoPar"));
}
//-------------------------------------------------------------------------
Bool_t ERNeuRad::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation
  static const Double_t dP = 1.032 ;
  static const Double_t BirkC1 =  0.013/dP;
  static const Double_t BirkC2 =  9.6e-6/(dP * dP);
  
  /** Track information to be stored until the track leaves the
      active volume. **/
  
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    StartNewPoint();
    if (fNeuRadFirstStep->GetEntriesFast() == 0)
      AddFirstStep();
  }
  
  if (fStorePrimarySteps && fMot0TrackID == -1 && fNeuRadSteps->GetEntriesFast() == 0){
    ERNeuRadStep* step =  AddStep();
      if (fVerbose > 2)
        step->Print();
  }
  
  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step
  fStepNb++;
  
  if (fStoreAllSteps){
    ERNeuRadStep* step =  AddStep();
    if (fVerbose > 2)
      step->Print();
  }
  Double_t curLightYield = 0.;
  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
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
    FinishNewPoint();
	}
  
  if (CurPointLen(fPosIn) > 4.){
    FinishNewPoint();
    StartNewPoint();
  }
  
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------

Double_t ERNeuRad::CurPointLen(TLorentzVector& posIn){
  TLorentzVector posOut;
  gMC->TrackPosition(posOut);
  return TMath::Sqrt((posIn.X() - posOut.X())*(posIn.X() - posOut.X()) + 
                    (posIn.Y() - posOut.Y())*(posIn.X() - posOut.X()) + 
                    (posIn.Z() - posOut.Z())*(posIn.Z() - posOut.Z()));
}

//--------------------------------------------------------------------------------------------------
void ERNeuRad::StartNewPoint(){
  fELoss  = 0.;
  fLightYield = 0.;
  fStepNb = 0;
  fEventID = gMC->CurrentEvent();
  gMC->TrackPosition(fPosIn);
  gMC->TrackMomentum(fMomIn);
  fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
  fTimeIn   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
  fTrackLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
  fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  Int_t curVolId, corOffVolId;
  if(!(TString(gMC->CurrentVolOffName(1)).Contains("dead") && TString(gMC->CurrentVolOffName(2)).Contains("pixel"))){
    Fatal("StartNewPoint", "Old version of geometry structure is used"); 
  }
  curVolId = gMC->CurrentVolOffID(1,fFiberNb); 
  corOffVolId = gMC->CurrentVolOffID(2, fPixelNb);
  corOffVolId = gMC->CurrentVolOffID(3, fModuleNb);
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  fPosInLocal.SetXYZ(localPos[0],localPos[1],localPos[2]);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void ERNeuRad::FinishNewPoint(){
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09; 
  
  if (fELoss > 0.){
    AddPoint();
    fFullEnergy+=fELoss;
    fFullLY+=fLightYield;
  }                
}
//--------------------------------------------------------------------------------------------------
// -----   Public method EndOfEvent   -----------------------------------------
void ERNeuRad::BeginEvent() {
}
//--------------------------------------------------------------------------------------------------
void ERNeuRad::EndOfEvent() {
  if (fVerbose > 1) {
    Print();
  }

  FairRunSim* run = FairRunSim::Instance();
  ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
  header->SetNeuRadEloss(fFullEnergy);
  header->SetNeuRadLY(fFullLY);

  Reset();
}


// -----   Public method Register   -------------------------------------------
void ERNeuRad::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("NeuRadPoint","NeuRad", fNeuRadPoints, kTRUE);
  ioman->Register("NeuRadFirstStep","NeuRad", fNeuRadFirstStep, kTRUE);
  ioman->Register("NeuRadStep","NeuRad", fNeuRadSteps, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERNeuRad::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNeuRadPoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ERNeuRad::Print(Option_t *option) const
{
  for (Int_t i_point = 0; i_point < fNeuRadPoints->GetEntriesFast(); i_point++){
    ERNeuRadPoint* point = (ERNeuRadPoint*)fNeuRadPoints->At(i_point);
    point->Print();
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERNeuRad::Reset() {
  fNeuRadPoints->Clear();
  fNeuRadSteps->Clear();
  fNeuRadFirstStep->Clear();
  fFullEnergy = 0.;
  fFullLY = 0.;
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERNeuRad::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  std::cout << "NeuRad: " << nEntries << " entries to add" << std::endl;
  TClonesArray& clref = *cl2;
  ERNeuRadPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERNeuRadPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERNeuRadPoint(*oldpoint);
  }
  std::cout << "NeuRad: " << cl2->GetEntriesFast() << " merged entries" << std::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERNeuRadPoint* ERNeuRad::AddPoint() {
  TClonesArray& clref = *fNeuRadPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERNeuRadPoint(fEventID, fTrackID, fMot0TrackID, fFiberNb,fPixelNb,fModuleNb,fMass,
					  fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,fTimeOut,
            fTrackLength, fELoss, fLightYield,gMC->TrackPid(), gMC->TrackCharge());
}
// ----------------------------------------------------------------------------

// -----   Private method AddFirstStep   --------------------------------------------
ERNeuRadStep* ERNeuRad::AddFirstStep(){
  TClonesArray& clref = *fNeuRadFirstStep;
  fTrackStatus = ERNeuRadStep::GetTrackStatus();
  gMC->StepProcesses(fProcessesID);
  gMC->TrackPosition(fCurPosIn);
  gMC->TrackMomentum(fCurMomIn);
  return new(clref[0]) ERNeuRadStep(fEventID,fStepNb, fTrackID, fMot0TrackID, fFiberNb, fPixelNb, fModuleNb,
             fCurPosIn.Vect(), fCurMomIn.Vect(), gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(), gMC->TrackMass(),
             fTrackStatus, gMC->Edep(), gMC->TrackCharge(), fProcessesID);        
}

// -----   Private method AddStep   --------------------------------------------
ERNeuRadStep* ERNeuRad::AddStep(){
  TClonesArray& clref = *fNeuRadSteps;
  fTrackStatus = ERNeuRadStep::GetTrackStatus();
  gMC->StepProcesses(fProcessesID);
  gMC->TrackPosition(fCurPosIn);
  gMC->TrackMomentum(fCurMomIn);
  return new(clref[fNeuRadSteps->GetEntriesFast()])ERNeuRadStep(fEventID,fStepNb, fTrackID, fMot0TrackID, fFiberNb,
          fPixelNb, fModuleNb, fCurPosIn.Vect(), fCurMomIn.Vect(), gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(), 
          gMC->TrackMass(),fTrackStatus, gMC->Edep(), gMC->TrackCharge(), fProcessesID); 
}

// ----------------------------------------------------------------------------
Bool_t ERNeuRad::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("fiber") && !volName.Contains("dead") ) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRad::ResetParameters() {
}
//----------------------------------------------------------------------------
ClassImp(ERNeuRad)
