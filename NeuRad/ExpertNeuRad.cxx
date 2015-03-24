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

Int_t ExpertNeuRad::fNEeventsWithoutPoints = 0;
const Double_t ExpertNeuRad::fHistoELossThreshold = 100.;

// -----   Default constructor   -------------------------------------------
ExpertNeuRad::ExpertNeuRad() : FairDetector("ExpertNeuRad", kTRUE){
  ResetParameters();
  fNeuRadCollection = new TClonesArray("ExpertNeuRadPoint");
  fPosIndex = 0;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
  fhModulesPerSumEnergyLoss =  new TH1F("ModulesPerSumEnergyLoss", "Modules per sum of energy loss", 10000, 0., 10000.);
  fhSumEnergyLossOfAllModules =  new TH1F("SumEnergyLossOfAllModules", "Sum Of ELoss in all modules", 10000, 0., 10000.);
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ExpertNeuRad::ExpertNeuRad(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active) {
  ResetParameters();
  fNeuRadCollection = new TClonesArray("ExpertNeuRadPoint");
  fPosIndex = 0;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = verbose;
  fVersion = 1;
  fhModulesPerSumEnergyLoss =  new TH1F("ModulesPerSumEnergyLoss", "Modules per sum of energy loss", 10000, 0., 10000.);
  fhSumEnergyLossOfAllModules =  new TH1F("SumEnergyLossOfAllModules", "Sum Of ELoss in all modules", 10000, 0., 10000.);
}

ExpertNeuRad::~ExpertNeuRad() {
  if (fNeuRadCollection) {
    fNeuRadCollection->Delete();
    delete fNeuRadCollection;
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
  
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    fELoss  = 0.;
    fLightYield = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    fLength = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fModuleInBundleNb = -1.;
    Int_t curVolId =  gMC->CurrentVolID(fModuleInBundleNb);
  }
    
  fELoss += gMC->Edep() * 1E+6; // keV //Return the energy lost in the current step
  
  if (fVerboseLevel > 2)
    StepHistory();
  
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
      
      fLightYield+=curLightYield;
    }
  }
  
	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
	  fEventID = gMC->CurrentEvent();
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
      
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
	  
	  if (fELoss > 0.){
      AddHit(fEventID, fTrackID, fMot0TrackID, fModuleInBundleNb-1, fMass,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fTime, fLength, fELoss, fLightYield);
    }
	}
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ExpertNeuRad::BeginEvent() {

}


void ExpertNeuRad::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  
  FillHisto();
  
  fhModulesPerSumEnergyLoss->Write();
  fhSumEnergyLossOfAllModules->Write();
  
  Reset();
}


// -----   Public method Register   -------------------------------------------
void ExpertNeuRad::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");	
  ioman->Register("NeuRadPoint","NeuRad", fNeuRadCollection, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* ExpertNeuRad::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNeuRadCollection;
  else 
    return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void ExpertNeuRad::Print(Option_t *option) const
{
  for (Int_t i_point = 0; i_point < fNeuRadCollection->GetEntriesFast(); i_point++){
    ExpertNeuRadPoint* point = (ExpertNeuRadPoint*)fNeuRadCollection->At(i_point);
    point->Print();
  }
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ExpertNeuRad::Reset() {
  fNeuRadCollection->Clear();
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
   new (clref[fPosIndex]) ExpertNeuRadPoint(*oldpoint);
   fPosIndex++;
  }
  LOG(INFO) << "NeuRad: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddHit   --------------------------------------------
ExpertNeuRadPoint* ExpertNeuRad::AddHit(Int_t eventID, Int_t trackID,
				    Int_t mot0trackID,
            Int_t moduleInBundleNb,
				    Double_t mass,
				    TVector3 posIn,
				    TVector3 posOut, TVector3 momIn,
				    TVector3 momOut, Double_t time,
				    Double_t length, Double_t eLoss, Double_t lightYield) {
  TClonesArray& clref = *fNeuRadCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ExpertNeuRadPoint(eventID, trackID, mot0trackID, moduleInBundleNb, mass,
					  posIn, posOut, momIn, momOut, time, length, eLoss, lightYield);
	
}
// ----------------------------------------------------------------------------

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
  if(volName.Contains("module")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRad::ResetParameters() {
  fEventID = fTrackID = -1;
  fMot0TrackID = -1;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = fMass = 0;
  fPosIndex = 0;
};
// ----------------------------------------------------------------------------

// -----   Private method FillHisto   ---------------------------------------
void ExpertNeuRad::FillHisto(){
  //Fill ModulesPerSumEnergyLoss histo
  Int_t NPoints = fNeuRadCollection->GetEntriesFast();
  if (NPoints == 0)
    fNEeventsWithoutPoints++;
  Int_t NModules = 64.;
  Double_t* SumEloss = new Double_t[NModules];
  // init 
  for (Int_t i_module = 0; i_module < NModules; i_module++)
    SumEloss[i_module] = 0.;
  
  // Sum energy per modules
  Double_t sumOfEnergy = 0.;
  for (Int_t i_point = 0; i_point < NPoints; i_point++){
    ExpertNeuRadPoint* point = (ExpertNeuRadPoint*)fNeuRadCollection->At(i_point);
    Int_t point_module_nb = point->GetModuleInBundleNb();
    SumEloss[point_module_nb] += point->GetEnergyLoss();
    sumOfEnergy+= point->GetEnergyLoss();
  }
  if (sumOfEnergy > fHistoELossThreshold)
    fhSumEnergyLossOfAllModules->Fill(sumOfEnergy);
  for (Int_t i_module = 0; i_module < NModules; i_module++){
    if (SumEloss[i_module] > fHistoELossThreshold)
      fhModulesPerSumEnergyLoss->Fill(SumEloss[i_module]);
  }
}
// ----------------------------------------------------------------------------

// -----   Private method StepHistory   ---------------------------------------
void ExpertNeuRad::StepHistory()
{
  static Int_t iStepN;

  // Particle being tracked
  const char *sParticle;
  switch(gMC->TrackPid()){
    case 2212:          sParticle="proton"    ;break;
    case 2112:          sParticle="neutron"   ;break;
    case 22:            sParticle="gamma"     ;break;
    case 50000050:      sParticle="ckov"      ;break;
    case 111:           sParticle="pi0"       ;break;  
    case 211:           sParticle="pi+"       ;break;  
    case -211:          sParticle="Pi-"       ;break;  
    case 1000010020:            sParticle="deuteron"        ;break;
    case 1000010030:            sParticle="triton"        ;break;
    case 1000020030:            sParticle="he3"        ;break;
    case 1000020040:            sParticle="alpha"        ;break;
    default:            sParticle="not known" ;break;
  }


  TString flag="-I- STEPINFO: tracking status: ";
  if(gMC->IsTrackAlive()) {
    if(gMC->IsTrackEntering())      flag="enters to";
    else if(gMC->IsTrackExiting())  flag="exits from";
    else if(gMC->IsTrackInside())   flag="inside";
  } else {
    if(gMC->IsTrackStop())          flag="stopped in";
  }
  
  LOG(INFO) << "STEP = " << iStepN << " particle="<< sParticle << "(" << gMC->TrackPid() << ") Edep = " << gMC->Edep()*1e6 << "[KeV]" << FairLogger::endl;
  LOG(INFO) << "track_status = "  << flag.Data() << " track_charge = " << gMC->TrackCharge() << "track_nb = " << gMC->GetStack()->GetCurrentTrackNumber() << FairLogger::endl;
  
  TArrayI proc;  
  gMC->StepProcesses(proc);
  for ( int i = 0 ; i < proc.GetSize(); i++){
    //if(proc.At(i)!=22 && proc.At(i)!=23 && proc.At(i)!=31 && proc.At(i)!=43 &&  proc.At(i)!=13){
    LOG(INFO) << "process: " << proc.At(i) <<"  "<< TMCProcessName[proc.At(i)] << FairLogger::endl;
	}
  LOG(INFO) << FairLogger::endl;
  iStepN++;
}
// ----------------------------------------------------------------------------
ClassImp(ExpertNeuRad)
