// -------------------------------------------------------------------------
// -----                      ERGadastStep source file                 -----
// -------------------------------------------------------------------------


#include "ERGadastStep.h"
#include<iostream>
#include "TVirtualMC.h"


// -----   Default constructor   -------------------------------------------
ERGadastStep::ERGadastStep()
  : fX(0.), fY(0.), fZ(0.),
    fPx(0.), fPy(0.), fPz(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERGadastStep::ERGadastStep(Int_t eventID, Int_t stepNr,Int_t trackID,
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
      TArrayI  processID)
  : fEventID(eventID), fStepNr(stepNr), fTrackID(trackID), fMot0TrackID(mot0trackID),
    fFiberInBundleNb(fiberInBundleNb),
    fX(pos.X()), fY(pos.Y()), fZ(pos.Z()),
    fPx(mom.X()), fPy(mom.Y()), fPz(mom.Z()),
    fTOF(tof), fLength(length), fPID(pid), fMass(mass),
    fTrackStatus(trackStatus),
    fEloss(eLoss),
    fCharge(charge),
    fProcessID(processID)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERGadastStep::ERGadastStep(const ERGadastStep& right)
  : fEventID(right.fEventID),  fStepNr(right.fStepNr), fTrackID(right.fTrackID), 
    fMot0TrackID(right.fMot0TrackID), fFiberInBundleNb(right.fFiberInBundleNb),
    fX(right.fX), fY(right.fY), fZ(right.fZ),
    fPx(right.fPx), fPy(right.fPy), fPz(right.fPz),
    fTOF(right.fTOF), fLength(right.fLength), fPID(right.fPID),fMass(right.fMass),
    fTrackStatus(right.fTrackStatus),
    fEloss(right.fEloss), fCharge(right.fCharge), fProcessID(right.fProcessID)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERGadastStep::~ERGadastStep()
{
}
// -------------------------------------------------------------------------

// -----   Public method Print   ---------------------------------------
void ERGadastStep::Print()
{
  // Particle being tracked
  const char *sParticle;
  
  switch(fPID){
    case 2212:          sParticle="proton"    ;break;
    case 2112:          sParticle="neutron"   ;break;
    case 22:            sParticle="gamma"     ;break;
    case 50000050:      sParticle="ckov"      ;break;
    case 111:           sParticle="pi0"       ;break;  
    case 211:           sParticle="pi+"       ;break;  
    case -211:          sParticle="Pi-"       ;break;  
    case 1000010020:    sParticle="deuteron"  ;break;
    case 1000010030:    sParticle="triton"    ;break;
    case 1000020030:    sParticle="he3"       ;break;
    case 1000020040:    sParticle="alpha"     ;break;
    default:            sParticle="not known" ;break;
  }
  
  std::cout << "-I- STEPINFO:" << std::endl;
  TString flag;
  switch (fTrackStatus) {
    case Entering :      flag="enters to";               break;
    case Exiting :  flag="exits from";              break;
    case Inside :   flag="inside";                  break;
    case Stop :     flag="stopped in";              break;
    default:                                        flag="unknown tracking status"; break;  
  }
  
  std::cout << "STEP = " << fStepNr << " particle="<< sParticle << "(" << fPID << ") Edep = " 
            << fEloss << "[KeV]" << std::endl;
  std::cout << "track_status = "  << flag.Data() << " track_charge = " << fCharge 
            << "track_nb = " << fTrackID << std::endl;
  std::cout << "track_pos = "  << fX << " " << fY << " " << fZ << std::endl;
  std::cout << "track_mom = "  << fPx << " " << fPy << " " << fPz << std::endl;
  
  for ( int i = 0 ; i < fProcessID.GetSize(); i++){
    //if(proc.At(i)!=22 && proc.At(i)!=23 && proc.At(i)!=31 && proc.At(i)!=43 &&  proc.At(i)!=13){
    std::cout << "process: " << fProcessID.At(i) <<"  "<< TMCProcessName[fProcessID.At(i)] 
              << std::endl;
	}
  std::cout << std::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertTrackingStatus ERGadastStep::GetTrackStatus(){
  if(gMC->IsTrackAlive()) {
    if(gMC->IsTrackEntering())      
      return Entering;
    else if(gMC->IsTrackExiting())  
      return Exiting;
    else if(gMC->IsTrackInside())   
      return Inside;
  } else {
    return Stop;
  }
  
}
// ----------------------------------------------------------------------------
ClassImp(ERGadastStep)
