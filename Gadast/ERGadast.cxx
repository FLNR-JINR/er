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
#include "TVector3.h"
#include "TGeoMatrix.h"

#include "ERGadastGeoPar.h"


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
  fStoreSteps = kFALSE;
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
  fStoreSteps = kFALSE; 
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
  fMesh = new ERGadastMesh();
  fRnd = new TRandom3();
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

  if (fDetectorType == CsI){
    TLorentzVector curPosIn;
    gMC->TrackPosition(curPosIn);
    TGeoHMatrix m;
    TString path(gMC->CurrentVolPath());
    gMC->GetTransformation(path, m);
    int curMeshElement = fMesh->GetMeshElement(&curPosIn,&m, fDetectorType);
    if (fStepNr == 0){
      fMeshElement = curMeshElement;
    }
    else {
      if (fMeshElement != curMeshElement){
        FinishPoint();
        fMeshElement = curMeshElement;
        StartPoint();
      }
    }
  }
  fStepNr++;
}

// -----   Private method StartPoint   -----------------------------------------
void ERGadast::StartPoint(){
    fELoss  = 0.;
    fEventID = gMC->CurrentEvent();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
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
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  
  if (fELoss > 0.){
    //energy resolution
    const double E0=0.662;
    const double FWHM= 0.08;
    const double sigma0=0.5*FWHM*E0/sqrt(log(2.));
    const double sigma = sigma0*sqrt(fELoss/E0);
    fELoss = fRnd->Gaus(fELoss, sigma);
    if(fDetectorType == 0) 
      AddCsIPoint();
    if(fDetectorType == 1)
      AddLaBrPoint();
    }
}
//------------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------
void ERGadast::ResetParameters() {
};
// ----------------------------------------------------------------------------
ClassImp(ERGadast)
