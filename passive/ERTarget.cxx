/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTarget.h"

#include "TClonesArray.h"
#include "TGeoMatrix.h"

// -----   Initialisation of static variables   --------------------------
Double_t ERTarget::fThickness = 2.; // cm
// ------------------------------------------------------------------------

ERTarget::ERTarget()
  : ERDetector("ERNeuRad", kTRUE) //TODO rename?
{
  fTargetPoints = new TClonesArray("ERmuSiPoint"); //TODO rename?
}

ERTarget::ERTarget(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active)
{
  fTargetPoints = new TClonesArray("ERmuSiPoint"); //TODO rename?
}

ERTarget::~ERTarget() {
}

Bool_t ERTarget::ProcessHits(FairVolume* vol) {
/*
  FairRunSim* run = FairRunSim::Instance();

  ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();

  Double_t targetReactionPos = header->TargetReactionPos();

  TLorentzVector curPos;
  gMC->TrackPosition(curPos);
  if (gMC->ParticleName(gMC->TrackPid()).Contains("F27") ||
       gMC->ParticleName(gMC->TrackPid()).Contains("ExpertPrimaryIon"))
  {
    if (curPos.Z() >= targetReactionPos){
      cerr << "TargetReactionPos = " << targetReactionPos << " " << curPos.Z() << endl;
      gMC->StopTrack();
    }
  }
*/
  return kTRUE;
}

void ERTarget::BeginEvent() {
}

void ERTarget::EndOfEvent() {
}

void ERTarget::Register() {
}

TClonesArray* ERTarget::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fTargetPoints;
  else 
    return NULL;
}

void ERTarget::Print(Option_t *option) const {
}

void ERTarget::Reset() {
}

void ERTarget::CopyClones(TClonesArray* cl1,
                          TClonesArray* cl2,
                          Int_t offset) {
}

// ------------------------------------------------------------------------------
void ERTarget::SetModulePosition(Double_t x, Double_t y, Double_t z) {
  fPositionRotation->SetDx(x);
  fPositionRotation->SetDy(y);
  fPositionRotation->SetDz(z);
}
// ------------------------------------------------------------------------------

void ERTarget::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if (fileName == "") {
    LOG(FATAL) << "Target geometry file name is not set." << FairLogger::endl;
  } else if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing target geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    //TODO Uncomment when the corresponding method is implemented in FairRoot
    ConstructRootGeometry(/*(TGeoMatrix*)(fPositionRotation)*/);
  } else if(fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing target geometry from GDML file " << fileName.Data() << FairLogger::endl;
    ConstructGDMLGeometry(fPositionRotation);
  } else {
    LOG(FATAL) << "Target geometry file name is not correct." << FairLogger::endl;
  }
}

void ERTarget::Initialize() {
  FairDetector::Initialize();
}

Bool_t ERTarget::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.Contains("target_vol")) {
    return kTRUE;
  }
  return kFALSE;
}

ClassImp(ERTarget)
