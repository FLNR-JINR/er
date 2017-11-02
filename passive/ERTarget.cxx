#include "ERTarget.h"

#include <stdlib.h>

#include "TVirtualMC.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoRootBuilder.h"
#include "FairRuntimeDb.h"
#include "TObjArray.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"

#include "ERMCEventHeader.h"
#include "ERGeoPassivePar.h"

// -----   Initialsisation of static variables   --------------------------
Double_t ERTarget::fThickness = 2.; //cm
// ------------------------------------------------------------------------

ERTarget::ERTarget(): ERDetector("ERNeuRad", kTRUE)
{
  fTargetPoints = new TClonesArray("ERmuSiPoint");
}

ERTarget::ERTarget(const char* name, Bool_t active, Int_t verbose):ERDetector(name, active)
{
  fTargetPoints = new TClonesArray("ERmuSiPoint");
}

ERTarget::~ERTarget()
{ 
}

Bool_t ERTarget::ProcessHits(FairVolume* vol)
{
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

void ERTarget::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {  
}

void ERTarget::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing Target geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Target geometry file name is not set" << FairLogger::endl;
    exit(1);
  }
}

void ERTarget::Initialize() {
  FairDetector::Initialize();
}

Bool_t ERTarget::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("target_vol")) { //TODO серъёзно?
    return kTRUE;
  }
  return kFALSE;
}

ClassImp(ERTarget)
