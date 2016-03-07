// -------------------------------------------------------------------------
// -----                        ERGadast source file                   -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------
#include "ERGadast.h"

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"


// -----   Default constructor   -------------------------------------------
ERGadast::ERGadast() : ERDetector("ERGadast", kTRUE)
{
  ResetParameters();
  fCsIPoints = new TClonesArray("ERGadastCsIPoint");
  fLaBrPoints = new TClonesArray("ERGadastLaBrPoint");
  fGadastSteps = new TClonesArray("ERGadastStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
  
}
//-------------------------------------------------------------------------

//-----   Standard constructor   ------------------------------------------
ERGadast::ERGadast(const char* name, Bool_t active) 
  : ERDetector(name, active)
{  
  ResetParameters();
  fCsIPoints = new TClonesArray("ERGadastCsIPoint");
  fLaBrPoints = new TClonesArray("ERGadastLaBrPoint");
  fGadastSteps = new TClonesArray("ERGadastStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
  
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
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Bool_t ERGadast::ProcessHits(FairVolume* vol) {
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn, posOut;    //!  position
  static TLorentzVector momIn, momOut;    //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss
  static Int_t pdg;
  static ExpertTrackingStatus trackStatus;
  static Int_t          stepNr;            //!  current step numb in this active volumes
  static Int_t          detectorType;      //! 0 - CsI, 1 - LaBr


  //Start point
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

    if (TString(gMC->CurrentVolName()).Contains("LaBrcell_cell"))
      detectorType = 1;
    else
      detectorType = 0;

    pdg = gMC->TrackPid();
    stepNr = 0;
  }


  //Fill step info
  trackStatus = ERGadastStep::GetTrackStatus();
  TArrayI processesID;
  gMC->StepProcesses(processesID);
  TLorentzVector curPosIn;
  TLorentzVector curMomIn;
  gMC->TrackPosition(curPosIn);
  gMC->TrackMomentum(curMomIn);

  //if (stepNr == 0){
    ERGadastStep* step =  AddStep( eventID, stepNr, trackID, mot0TrackID, 0,
                                      TVector3(curPosIn.X(),   curPosIn.Y(),   curPosIn.Z()),
                                      TVector3(curMomIn.X(),   curMomIn.Y(),   curMomIn.Z()),  
                                      gMC->TrackTime() * 1.0e09, gMC->TrackStep(), gMC->TrackPid(),mass, 
                                      trackStatus, gMC->Edep(),gMC->TrackCharge(), processesID);
  //}
  

  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step
  stepNr++;
  
  //finish point
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    
	  if (eLoss > 0.){
      if(detectorType == 0) 
        AddCsIPoint( eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, pdg);
      if(detectorType == 1)
        AddLaBrPoint( eventID, trackID, mot0TrackID, mass,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, pdg);
    }
  }
  
  return kTRUE;
}

// -----   Public method BeginOfEvent   -----------------------------------------
void ERGadast::BeginEvent() {
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERGadast::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
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
  LOG(INFO) << "ERGadast: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERGadastCsIPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERGadastCsIPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERGadastCsIPoint(*oldpoint);
  }
  LOG(INFO) << "ERGadast: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
  
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERGadastCsIPoint* ERGadast::AddCsIPoint(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss, Int_t pdg) {
  TClonesArray& clref = *fCsIPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastCsIPoint(eventID, trackID, mot0trackID, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss, pdg);
	
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
ERGadastLaBrPoint* ERGadast::AddLaBrPoint(Int_t eventID, Int_t trackID,
            Int_t mot0trackID,
            Double_t mass,
            TVector3 posIn,
            TVector3 posOut, TVector3 momIn,
            TVector3 momOut, Double_t time,
            Double_t length, Double_t eLoss, Int_t pdg) {
  TClonesArray& clref = *fLaBrPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERGadastLaBrPoint(eventID, trackID, mot0trackID, mass,
            posIn, posOut, momIn, momOut, time, length, eLoss, pdg);
  
}
// ----------------------------------------------------------------------------

// -----   Private method AddStep   --------------------------------------------
ERGadastStep* ERGadast::AddStep(Int_t eventID, Int_t stepNr,Int_t trackID,
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
  TClonesArray& clref = *fGadastSteps;
  return new(clref[fGadastSteps->GetEntriesFast()]) ERGadastStep(eventID,  stepNr, trackID,
              mot0trackID, fiberInBundleNb, pos, mom, tof, length, pid, mass, trackStatus,
              eLoss, charge, processID);        
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

// ----------------------------------------------------------------------------
void ERGadast::ResetParameters() {
};
// ----------------------------------------------------------------------------
ClassImp(ERGadast)
