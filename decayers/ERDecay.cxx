/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
 #include "ERDecay.h"

#include <iostream>

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoBBox.h"
#include "TRandom3.h"

#include "FairRunSim.h"
#include "FairLogger.h"

//#include "ERTarget.h"
#include "ERMCEventHeader.h"      //for ERMCEventHeader

//--------------------------------------------------------------------------------------------------
ERDecay::ERDecay(TString name)
: fName(name),
  fInteractionVolumeName(""),
  fNuclearInteractionLength(0.),
  // fInteractProbability(0.),
  fNormalizingProbability(0.),
  fIsInterationPointFound(kFALSE)
{
  fRnd1 = new TRandom3();
  fRnd2 = new TRandom3();
  fRnd = new TRandom3();
}
//--------------------------------------------------------------------------------------------------
ERDecay::~ERDecay(){
}

void ERDecay::SetInputIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  fInputIonName = fName + TString("_InputIon");
  FairIon* ion = new FairIon(fInputIonName,A,Z,Q);
  run->AddNewIon(ion);
}

void ERDecay::SetUniformPos(Double_t a, Double_t b){
  fUniform = kTRUE;
  fUniformA = a;
  fUniformB = b;
}

void ERDecay::SetExponentialPos(Double_t start, Double_t tau){
  fExponential = kTRUE;
  fExponentialStart = start;
  fExponentialTau = tau;
}

void ERDecay::BeginEvent(){
  fDecayFinish = kFALSE;
  if (fUniform){
    fDecayPosZ = fRnd->Uniform(fUniformA, fUniformB);
  }
  if (fExponential){
    fDecayPosZ = fExponentialStart + fRnd->Exp(fExponentialTau);
  }
}

void ERDecay::FinishEvent(){
}

Bool_t ERDecay::Init(){
  if (fInputIonName == ""){
    LOG(FATAL) << "Input ion not defined!" << FairLogger::endl;
    return kFALSE;
  }

  fInputIonPDG = TDatabasePDG::Instance()->GetParticle(fInputIonName);
  if ( ! fInputIonPDG ) {
    LOG(FATAL) << "Input ion not found in pdg database!" << FairLogger::endl;
    return kFALSE;
  }

  return kTRUE;
}

void ERDecay::AddParticleToStack(Int_t pdg, TLorentzVector pos, TLorentzVector state){
  Int_t newTrackNb;
  gMC->GetStack()->PushTrack(1,gMC->GetStack()->GetCurrentTrackNumber(), pdg,
                     state.Px(),state.Py(),state.Pz(),state.E(),
                     pos.X(), pos.Y(), pos.Z(),
                     gMC->TrackTime(), 0., 0., 0.,
                     kPDecay, newTrackNb, fInputIonPDG->Mass(),0);
  LOG(DEBUG) << "ERDecay: Added output particle with ID = " << newTrackNb << " PDG = " << pdg
     << "; pos=(" << pos.X() << "," << pos.Y() << "," << pos.Z() 
     << "); mom=(" << state.Px() << "," << state.Py() << "," << state.Pz() << ")" << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERDecay::SetMaxPathLength(Double_t pathLength) {
  fNormalizingProbability = 1 - TMath::Exp(-pathLength / fNuclearInteractionLength); 
}
//--------------------------------------------------------------------------------------------------
void ERDecay::CalculateTargetParameters() {
  LOG(DEBUG) << "ERDecay: calculated parameters " << FairLogger::endl;
  if (fInteractionVolumeName != "") {
    TGeoVolume* vol = gGeoManager->FindVolumeFast(fInteractionVolumeName);
    TGeoBBox*   shape = (TGeoBBox*)vol->GetShape(); // we use conversion of shape type to TGeoBBox because all shape types in ROOT inherited from TGeoBBox;
    Double_t    boundX = 2 * shape->GetDX();
    Double_t    boundY = 2 * shape->GetDY();
    Double_t    boundZ = 2 * shape->GetDZ();
    LOG(DEBUG) << "ERDecay: bounding box x = " << boundX 
              << "; y = " << boundY 
              << "; z = " << boundZ << FairLogger::endl;
    // fTargetBoundBoxDiagonal = TMath::Sqrt(boundX*boundX + boundY*boundY + boundZ*boundZ);
    // fNormalizingProbability = 1 - TMath::Exp(-fTargetBoundBoxDiagonal / fNuclearInteractionLength); 
  }
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDecay::FindInteractionPoint() {
  if (!fIsInterationPointFound) {
    gGeoManager->FindNextBoundary(); // find a step to a next boudary along current track direction
    Double_t  distToNextBoundary = gGeoManager->GetStep(); // get calculated step
    LOG(DEBUG) << "[ERDecay::FindInteractionPoint] distance to a next boundary " 
              << distToNextBoundary <<  FairLogger::endl;
    Double_t interactionProbNextBound = 1 - TMath::Exp(-distToNextBoundary / 
                                                        fNuclearInteractionLength);
    Double_t interactProbability = interactionProbNextBound / fNormalizingProbability;  // the interaction probability in current direction in the defined volume 
    if (interactProbability > 1) {
      LOG(ERROR) << "[ERDecay::FindInteractionPoint] interaction probability " 
                 << "in current direction more then 1, "
                 << "incorrect normalizing respect to maximum path length in an interaction volume" 
                 << FairLogger::endl;  
    }
    LOG(DEBUG) << "[ERDecay::FindInteractionPoint] interaction probability in current direction " 
              << interactProbability 
              << "; normalizing probability is " << fNormalizingProbability << FairLogger::endl;         
    if (fRnd1->Uniform(0, 1) > interactProbability) {
      LOG(INFO) << "[ERDecay::FindInteractionPoint]: interaction hasn't happened in current event" 
                << FairLogger::endl;
      return kFALSE;
    } else {
      fDistanceToInteractPoint = -TMath::Log(1 - fRnd2->Uniform(0, interactionProbNextBound)) 
                               * fNuclearInteractionLength;
      LOG(DEBUG) << "[ERDecay::FindInteractionPoint] distance to an interaction point " 
                << "in current direction " << fDistanceToInteractPoint << FairLogger::endl;         
      fIsInterationPointFound = kTRUE;
      return kTRUE;
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDecay)
