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
  // fRnd1->SetSeed();
  fRnd2 = new TRandom3();
  // fRnd2->SetSeed();
}
//--------------------------------------------------------------------------------------------------
ERDecay::~ERDecay(){

}
//--------------------------------------------------------------------------------------------------
void ERDecay::SetMaxPathLength(Double_t pathLength) {
  fNormalizingProbability = 1 - TMath::Exp(-pathLength / fNuclearInteractionLength); 
}
//--------------------------------------------------------------------------------------------------
// void ERDecay::CalculateTargetParameters() {
//   if (fInteractionVolumeName != "") {
//     TGeoVolume* vol = gGeoManager->FindVolumeFast(fInteractionVolumeName);
//     TGeoBBox*   shape = (TGeoBBox*)vol->GetShape(); // we use conversion of shape type to TGeoBBox because all shape types in ROOT inherited from TGeoBBox;
//     Double_t    boundX = 2 * shape->GetDX();
//     Double_t    boundY = 2 * shape->GetDY();
//     Double_t    boundZ = 2 * shape->GetDZ();
//     LOG(DEBUG) << "ERDecay: bounding box x = " << boundX 
//               << "; y = " << boundY 
//               << "; z = " << boundZ << FairLogger::endl;
//     fTargetBoundBoxDiagonal = TMath::Sqrt(boundX*boundX + boundY*boundY + boundZ*boundZ);
//     fNormalizingProbability = 1 - TMath::Exp(-fTargetBoundBoxDiagonal / fNuclearInteractionLength); 
//   }
// }
//--------------------------------------------------------------------------------------------------
Bool_t ERDecay::FindInteractionPoint() {
  if (!fIsInterationPointFound) {
    gGeoManager->FindNextBoundary(); // find a step to a next boudary along current track direction
    Double_t  distToNextBoundary = gGeoManager->GetStep(); // get calculated step
    LOG(DEBUG) << "[ERDecay::FindInteractionPoint] distance to a next boundary " 
              << distToNextBoundary <<  FairLogger::endl;
    Double_t interactProbability = (1 - TMath::Exp(-distToNextBoundary / fNuclearInteractionLength))
                                    / fNormalizingProbability;  // the interaction probability in current direction in the defined volume 
    if (interactProbability > 1) {
      LOG(ERROR) << "[ERDecay::FindInteractionPoint] interaction probability " 
                 << "in current direction more then 1, "
                 << "incorrect normalizing respect to maximum path length in an interaction volume" 
                 << FairLogger::endl;  
    }
    LOG(DEBUG) << "[ERDecay::FindInteractionPoint] interaction probability in current direction " 
              << interactProbability 
              << "; normalizing probability is " << fNormalizingProbability << FairLogger::endl;         
    if (gRandom->Uniform(0, 1) > interactProbability) {
      LOG(INFO) << "[ERDecay::FindInteractionPoint]: interaction hasn't happened in current event" 
                << FairLogger::endl;
      return kFALSE;
    } else {
      Double_t interactionProbNextBound = 1 - TMath::Exp(-distToNextBoundary / 
                                                          fNuclearInteractionLength);
      fDistanceToInteractPoint = -TMath::Log(1- gRandom->Uniform(0, interactionProbNextBound)) 
                               / fNuclearInteractionLength;
      LOG(DEBUG) << "[ERDecay::FindInteractionPoint] distance to an interaction point " 
                << "in current direction " << fDistanceToInteractPoint << FairLogger::endl;         
      fIsInterationPointFound = kTRUE;
      return kTRUE;
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDecay)
