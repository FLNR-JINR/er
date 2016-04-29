// -------------------------------------------------------------------------
// -----                        ERNeuRad source file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------
#include "ERNeuRad.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "ERNeuRadGeoPar.h"

#include <iostream>


// -----   Default constructor   -------------------------------------------
ERNeuRad::ERNeuRad() : ERDetector("ERNeuRad", kTRUE),
  fStorePrimarySteps(kFALSE),
  fStoreAllSteps(kFALSE)
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
  fHElossInEvent = new TH1F("fHElossInEvent", "Full Eloss in event",1000, 0., 0.5);
  fHLYInEvent = new TH1F("fHLYInEvent", "Full LY in event",1000, 0., 0.5);
}
//-------------------------------------------------------------------------
Bool_t ERNeuRad::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation
  static const Double_t dP = 1.032 ;
  static const Double_t BirkC1 =  0.013/dP;
  static const Double_t BirkC2 =  9.6e-6/(dP * dP);
  
  /** Track information to be stored until the track leaves the
      active volume. **/
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn;    //!  position
  static TLorentzVector momIn;    //!  momentum
  static Double32_t     timeIn;           //!  time
  static Double32_t     trackLength;       //!  track length from his origin
  static Double32_t     eLoss;             //!  energy loss
  static Double_t       lightYield;        //!  light yield
  static Int_t          fiberInBundleNb;  //!  number of fiber in bundle
  static Int_t          bundle;
  static Int_t          stepNr;            //!  current step numb in this active volumes
  static ExpertTrackingStatus trackStatus;
  
  trackStatus = ERNeuRadStep::GetTrackStatus();
  TArrayI processesID;  
  gMC->StepProcesses(processesID);
  TLorentzVector curPosIn;
  TLorentzVector curMomIn;
  gMC->TrackPosition(curPosIn);
  gMC->TrackMomentum(curMomIn);
  
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    StartNewPoint(eventID, eLoss, lightYield, stepNr, posIn, momIn, trackID, mot0TrackID,
                  trackLength, fiberInBundleNb, bundle, mass, timeIn);
                  
    if (fNeuRadFirstStep->GetEntriesFast() == 0){
      AddFirstStep( eventID, stepNr+1, trackID, mot0TrackID, fiberInBundleNb-1,
                    TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                    TVector3(momIn.X(),   momIn.Y(),   momIn.Z()),  
                    timeIn, gMC->TrackStep(), gMC->TrackPid(),mass, 
                    trackStatus, gMC->Edep(),gMC->TrackCharge(), processesID);
    }
    

  }
  
      if (fStorePrimarySteps && mot0TrackID == -1 && fNeuRadSteps->GetEntriesFast() == 0){
      ERNeuRadStep* step =  AddStep( eventID, stepNr, trackID, mot0TrackID, fiberInBundleNb-1,
                                        TVector3(curPosIn.X(),   curPosIn.Y(),   curPosIn.Z()),
                                        TVector3(curMomIn.X(),   curMomIn.Y(),   curMomIn.Z()),  
                                        gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(),mass, 
                                        trackStatus, gMC->Edep(),gMC->TrackCharge(), processesID);
      if (fVerbose > 2)
        step->Print();
    }
  
  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step
  stepNr++;
  
  if (fStoreAllSteps){
    ERNeuRadStep* step =  AddStep( eventID, stepNr, trackID, mot0TrackID, fiberInBundleNb-1,
                                      TVector3(curPosIn.X(),   curPosIn.Y(),   curPosIn.Z()),
                                      TVector3(curMomIn.X(),   curMomIn.Y(),   curMomIn.Z()),  
                                      gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(),mass, 
                                      trackStatus, gMC->Edep(),gMC->TrackCharge(), processesID);
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
      lightYield+=curLightYield;
    }
  }
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    FinishNewPoint(eventID,eLoss,lightYield,stepNr, posIn, momIn, trackID,mot0TrackID,
                    trackLength,fiberInBundleNb, bundle,mass, timeIn);
	}
  
  if (CurPointLen(posIn) > 4.){
    FinishNewPoint(eventID,eLoss,lightYield,stepNr, posIn, momIn, trackID,mot0TrackID,
                    trackLength,fiberInBundleNb, bundle,mass, timeIn);
    StartNewPoint(eventID, eLoss, lightYield, stepNr, posIn, momIn, trackID, mot0TrackID,
                  trackLength, fiberInBundleNb, bundle, mass, timeIn);
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
void ERNeuRad::StartNewPoint(Int_t& eventID,Double_t& eLoss,Double_t& lightYield,Int_t& stepNr,
                            TLorentzVector& posIn, TLorentzVector& momIn, Int_t& trackID,Int_t& mot0TrackID,
                            Double_t& trackLength,Int_t& fiberInBundleNb, Int_t& bundleNb, Double_t& mass, Double_t& timeIn){
  eLoss  = 0.;
  lightYield = 0.;
  stepNr = 0;
  
  eventID = gMC->CurrentEvent();
  gMC->TrackPosition(posIn);
  gMC->TrackMomentum(momIn);
  trackID  = gMC->GetStack()->GetCurrentTrackNumber();
  timeIn   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
  trackLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
  mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  Int_t curVolId =  gMC->CurrentVolID(fiberInBundleNb);
  Int_t corOffVolId = gMC->CurrentVolOffID(1, bundleNb);              
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void ERNeuRad::FinishNewPoint(Int_t& eventID,Double_t& eLoss,Double_t& lightYield,Int_t& stepNr,
                   TLorentzVector& posIn, TLorentzVector& momIn, Int_t& trackID,Int_t& mot0TrackID,
                   Double_t& trackLength,Int_t& fiberInBundleNb, Int_t& bundle ,Double_t& mass, Double_t& timeIn){

  TLorentzVector posOut, momOut;
    
  gMC->TrackPosition(posOut);
  gMC->TrackMomentum(momOut);
  Double_t timeOut = gMC->TrackTime() * 1.0e09; 
  
  if (eLoss > 0.){
    AddPoint( eventID, trackID, mot0TrackID, fiberInBundleNb-1,bundle-1, mass,
              TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
              TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
              TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
              TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
              timeIn, timeOut, trackLength, eLoss, lightYield, gMC->TrackPid(), gMC->TrackCharge());
    fFullEnergy+=eLoss;
    fFullLY+=lightYield;
  }                
}
                   
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// -----   Public method EndOfEvent   -----------------------------------------
void ERNeuRad::BeginEvent() {
  fFullEnergy = 0.;
  fFullLY = 0.;
}


void ERNeuRad::EndOfEvent() {
  if (fVerbose > 1) {
    Print();
  }
  fHElossInEvent->Fill(fFullEnergy);
  fHLYInEvent->Fill(fFullLY);
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
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERNeuRad::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "NeuRad: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERNeuRadPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERNeuRadPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERNeuRadPoint(*oldpoint);
  }
  LOG(INFO) << "NeuRad: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERNeuRadPoint* ERNeuRad::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
            Int_t fiberInBundleNb, Int_t bundle, 
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time, Double_t timeOut,
				    Double_t length, Double_t eLoss, Double_t lightYield, Int_t pid, Double_t charge) {
  TClonesArray& clref = *fNeuRadPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERNeuRadPoint(eventID, trackID, mot0trackID, fiberInBundleNb, bundle, mass,
					  posIn, posOut, momIn, momOut, time,timeOut, length, eLoss, lightYield, pid, charge);
	
}
// ----------------------------------------------------------------------------

// -----   Private method AddFirstStep   --------------------------------------------
ERNeuRadStep* ERNeuRad::AddFirstStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberInBundleNb,
		  TVector3 pos, 
      TVector3 mom, 
		  Double_t tof, 
      Double_t length, 
      Int_t pid,
      Double_t mass,
      ExpertTrackingStatus trackStatus,
      Double_t eLoss,
      Double_t charge,
      TArrayI  processID){
  TClonesArray& clref = *fNeuRadFirstStep;
  return new(clref[0]) ERNeuRadStep(eventID,  stepNr, trackID, mot0trackID, fiberInBundleNb,
					  pos, mom, tof, length, pid, mass, trackStatus, eLoss, charge, processID);        
}

// -----   Private method AddStep   --------------------------------------------
ERNeuRadStep* ERNeuRad::AddStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberInBundleNb,
		  TVector3 pos, 
      TVector3 mom, 
		  Double_t tof, 
      Double_t length, 
      Int_t pid,
      Double_t mass,
      ExpertTrackingStatus trackStatus,
      Double_t eLoss,
      Double_t charge,
      TArrayI  processID){
  TClonesArray& clref = *fNeuRadSteps;
  return new(clref[fNeuRadSteps->GetEntriesFast()]) ERNeuRadStep(eventID,  stepNr, trackID,
              mot0trackID, fiberInBundleNb, pos, mom, tof, length, pid, mass, trackStatus,
              eLoss, charge, processID);        
}

// ----------------------------------------------------------------------------
Bool_t ERNeuRad::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("fiber")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRad::ResetParameters() {
  
};
//----------------------------------------------------------------------------
void ERNeuRad::WriteHistos(){
  fHElossInEvent->Write();
  fHLYInEvent->Write();
}
//----------------------------------------------------------------------------
ClassImp(ERNeuRad)
