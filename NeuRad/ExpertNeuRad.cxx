// -------------------------------------------------------------------------
// -----                        ExpertNeuRad source file               -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------
#include "ExpertNeuRad.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"


// -----   Default constructor   -------------------------------------------
ExpertNeuRad::ExpertNeuRad() : FairDetector("ExpertNeuRad", kTRUE),
  fStorePrimarySteps(kFALSE),
  fStoreAllSteps(kFALSE)
{
  ResetParameters();
  fNeuRadPoints = new TClonesArray("ExpertNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ExpertNeuRadStep");
  fNeuRadSteps = new TClonesArray("ExpertNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ExpertNeuRad::ExpertNeuRad(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active),
  fVerbose(verbose),
  fStorePrimarySteps(kFALSE),
  fStoreAllSteps(kFALSE)
  {
  ResetParameters();
  fNeuRadPoints = new TClonesArray("ExpertNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ExpertNeuRadStep");
  fNeuRadSteps = new TClonesArray("ExpertNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
}

ExpertNeuRad::~ExpertNeuRad() {
  if (fNeuRadPoints) {
    fNeuRadPoints->Delete();
    delete fNeuRadPoints;
  }
}

void ExpertNeuRad::Initialize()
{
  FairDetector::Initialize();
}


Bool_t ExpertNeuRad::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation
  static Double_t dP = 1.032 ;
  static Double_t BirkC0 =  1.;
  static Double_t BirkC1 =  0.013/dP;
  static Double_t BirkC2 =  9.6e-6/(dP * dP);
  
  /** Track information to be stored until the track leaves the
      active volume. **/
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn, posOut;    //!  position
  static TLorentzVector momIn, momOut;    //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss
  static Double_t       lightYield;        //!  light yield
  static Int_t          fiberInBundleNb;  //!  number of fiber in bundle
  static Int_t          stepNr;            //!  current step numb in this active volumes
  static ExpertTrackingStatus trackStatus;
  static Double_t charge;
  
  trackStatus = ExpertNeuRadStep::GetTrackStatus();
  TArrayI processesID;  
  gMC->StepProcesses(processesID);
  charge = gMC->TrackCharge();
  
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    eLoss  = 0.;
    lightYield = 0.;
    stepNr = 0;
    
    eventID = gMC->CurrentEvent();
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID  = gMC->GetStack()->GetCurrentTrackNumber();
    time   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    Int_t curVolId =  gMC->CurrentVolID(fiberInBundleNb);
    
    if (fNeuRadFirstStep->GetEntriesFast() == 0){
      AddFirstStep( eventID, stepNr+1, trackID, mot0TrackID, fiberInBundleNb-1,
                    TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                    TVector3(momIn.X(),   momIn.Y(),   momIn.Z()),  
                    time, length, gMC->TrackPid(),mass, 
                    trackStatus, gMC->Edep() * 1E+6,charge, processesID);
    }
  }
  
  eLoss += gMC->Edep() * 1E+6; // keV //Return the energy lost in the current step
  stepNr++;
  
  
  if (fStorePrimarySteps && mot0TrackID == -1){
    ExpertNeuRadStep* step =  AddStep( eventID, stepNr, trackID, mot0TrackID, fiberInBundleNb-1,
                                      TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                                      TVector3(momIn.X(),   momIn.Y(),   momIn.Z()),  
                                      time, length, gMC->TrackPid(),mass, 
                                      trackStatus, gMC->Edep() * 1E+6,charge, processesID);
    if (fVerbose > 2)
      step->Print();
  }
  if (fStoreAllSteps){
    ExpertNeuRadStep* step =  AddStep( eventID, stepNr, trackID, mot0TrackID, fiberInBundleNb-1,
                                      TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                                      TVector3(momIn.X(),   momIn.Y(),   momIn.Z()),  
                                      time, length, gMC->TrackPid(),mass, 
                                      trackStatus, gMC->Edep() * 1E+6,charge, processesID);
    if (fVerbose > 2)
      step->Print();
  }

  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  // Correction for all charge states
  if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0;
    // Apply correction for higher charge states
    if (BirkC0==1){
      if (TMath::Abs(gMC->TrackCharge())>=2)
        BirkC1Mod=BirkC1*7.2/12.6;
      else
        BirkC1Mod=BirkC1;
    }

    if (gMC->TrackStep()>0)
    {
      Double_t dedxcm=1000.*gMC->Edep()/gMC->TrackStep(); //[MeV/cm]
      Double_t curLightYield=1000.*gMC->Edep()/(1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
      
      lightYield+=curLightYield;
    }
  }
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    
	  if (eLoss > 0.){
      AddPoint( eventID, trackID, mot0TrackID, fiberInBundleNb-1, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, lightYield);
    }
	}
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ExpertNeuRad::BeginEvent() {

}


void ExpertNeuRad::EndOfEvent() {
  if (fVerbose > 1) {
    Print();
  }
  
  Reset();
}


// -----   Public method Register   -------------------------------------------
void ExpertNeuRad::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("NeuRadPoint","NeuRad", fNeuRadPoints, kTRUE);
  ioman->Register("NeuRadFirstStep","NeuRad", fNeuRadFirstStep, kTRUE);
  ioman->Register("NeuRadStep","NeuRad", fNeuRadSteps, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ExpertNeuRad::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNeuRadPoints;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ExpertNeuRad::Print(Option_t *option) const
{
  for (Int_t i_point = 0; i_point < fNeuRadPoints->GetEntriesFast(); i_point++){
    ExpertNeuRadPoint* point = (ExpertNeuRadPoint*)fNeuRadPoints->At(i_point);
    point->Print();
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ExpertNeuRad::Reset() {
  fNeuRadPoints->Clear();
  fNeuRadFirstStep->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ExpertNeuRad::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "NeuRad: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ExpertNeuRadPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ExpertNeuRadPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ExpertNeuRadPoint(*oldpoint);
  }
  LOG(INFO) << "NeuRad: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ExpertNeuRadPoint* ExpertNeuRad::AddPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
            Int_t fiberInBundleNb,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss, Double_t lightYield) {
  TClonesArray& clref = *fNeuRadPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ExpertNeuRadPoint(eventID, trackID, mot0trackID, fiberInBundleNb, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss, lightYield);
	
}
// ----------------------------------------------------------------------------

// -----   Private method AddFirstStep   --------------------------------------------
ExpertNeuRadStep* ExpertNeuRad::AddFirstStep(Int_t eventID, Int_t stepNr,Int_t trackID,
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
  return new(clref[0]) ExpertNeuRadStep(eventID,  stepNr, trackID, mot0trackID, fiberInBundleNb,
					  pos, mom, tof, length, pid, mass, trackStatus, eLoss, charge, processID);        
}

// -----   Private method AddStep   --------------------------------------------
ExpertNeuRadStep* ExpertNeuRad::AddStep(Int_t eventID, Int_t stepNr,Int_t trackID,
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
  return new(clref[fNeuRadSteps->GetEntriesFast()]) ExpertNeuRadStep(eventID,  stepNr, trackID, 
              mot0trackID, fiberInBundleNb, pos, mom, tof, length, pid, mass, trackStatus,
              eLoss, charge, processID);        
}

// -----   Public method ConstructGeometry   ----------------------------------
void ExpertNeuRad::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing ExpertNeuRad geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ExpertNeuRad::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("fiber")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRad::ResetParameters() {
  
};
// ----------------------------------------------------------------------------
ClassImp(ExpertNeuRad)
