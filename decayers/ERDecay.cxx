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
  fNuclearInteractionLenght(0.),
  // fInteractionProbability(0.),
  fIsInteraction(kFALSE),
  fTargetBoundBoxDiagonal(0.),
  fNormalizingProbability(0.),
  fIsInterationPointFound(kFALSE),
  fMaxPathLenght(0.)
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
void ERDecay::SetMaxPathLenght(Double_t pathLenght) {
  fNormalizingProbability = 1 - TMath::Exp(-fMaxPathLenght / fNuclearInteractionLenght); 
}
//--------------------------------------------------------------------------------------------------
void ERDecay::CalculateTargetParameters() {
  if (fInteractionVolumeName != "") {
    TGeoVolume* vol = gGeoManager->FindVolumeFast(fInteractionVolumeName);
    TGeoBBox*   shape = (TGeoBBox*)vol->GetShape(); // we use conversion of shape type to TGeoBBox because all shape types in ROOT inherited from TGeoBBox;
    Double_t    boundX = 2 * shape->GetDX();
    Double_t    boundY = 2 * shape->GetDY();
    Double_t    boundZ = 2 * shape->GetDZ();
    std::cout << "ERDecay: bounding box x = " << boundX << "; y = " << boundY << "; z = " << boundZ << std::endl;
    fTargetBoundBoxDiagonal = TMath::Sqrt(boundX*boundX + boundY*boundY + boundZ*boundZ);
    fNormalizingProbability = 1 - TMath::Exp(-fTargetBoundBoxDiagonal / fNuclearInteractionLenght); 
  }
}
//--------------------------------------------------------------------------------------------------
Bool_t ERDecay::FindInteractionPoint() {
  if (!fIsInterationPointFound) {
    gGeoManager->FindNextBoundary(); // find a step to a next boudary along current track direction
    Double_t  distToNextBoundary = gGeoManager->GetStep(); // get calculated step

    Double_t interactionProbability = (1 - TMath::Exp(-distToNextBoundary / fNuclearInteractionLenght))
                                    / fNormalizingProbability;  // the interaction probability in current direction in defined volume 
    std::cout << "ERDecay::FindInteractionPoint interaction prob " << interactionProbability << std::endl;         
    if (gRandom->Uniform(0, 1) > interactionProbability) {
      std::cout << "[ERDecay::FindInteractionPoint]: interaction hasn't happened in current event" << FairLogger::endl;
      return kFALSE;
    } else {
      Double_t const *unitDirectVec = gGeoManager->GetCurrentDirection();
      fStepToInteractPoint = fRnd2->Exp(1 / fNuclearInteractionLenght);
      TVector3 stepVector(unitDirectVec[0], unitDirectVec[1], unitDirectVec[2]);
      std::cout << "ERDecay::FindInteractionPoint step " << fStepToInteractPoint << std::endl;         
      std::cout << "ERDecay::FindInteractionPoint start current point unit direction " 
                << unitDirectVec[0] << " " << unitDirectVec[1] << " " <<  unitDirectVec[2]  << " "
                << unitDirectVec[0]*unitDirectVec[0] + unitDirectVec[1]*unitDirectVec[1] + unitDirectVec[2]*unitDirectVec[2] 
                << std::endl; 
      std::cout << "ERDecay::FindInteractionPoint distToNextBoundary " << distToNextBoundary << std::endl; 
      stepVector *= fStepToInteractPoint;
      TLorentzVector curPosLorentz;
      gMC->TrackPosition(curPosLorentz);
      fInputPoint = curPosLorentz.Vect();
      fInteractionPoint = fInputPoint + stepVector;
      fIsInterationPointFound = kTRUE;
      return kTRUE;
    }
  }
  // std::cout << "ERDecay::FindInteractionPoint curren point " 
  //           << gGeoManager->GetCurrentPoint()[2] << std::endl; 
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERDecay)
