#include <iostream>
#include <stdlib.h>

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoRootBuilder.h"
#include "FairRuntimeDb.h"
#include "TObjArray.h"
#include "FairRun.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"

#include "ERTarget.h"
#include "ERGeoPassivePar.h"

using namespace std;


ERTarget::~ERTarget()
{
  if(fTargetName) {
    delete fTargetName;
  }
}


ERTarget::ERTarget()
{
  fTargetName = NULL;
}


ERTarget::ERTarget(const char * name,  const char * title)
  : FairModule(name ,title)
{
  fTargetName = new TString(name);
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


Bool_t ERTarget::CheckIfSensitive(std::string name)
{
  return kFALSE;
}


ClassImp(ERTarget)

