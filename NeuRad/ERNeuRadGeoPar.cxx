#include "ERNeuRadGeoPar.h"

#include "FairParamList.h"

#include "TObjArray.h"

#include <iostream>

ERNeuRadGeoPar ::ERNeuRadGeoPar(const char* name,
    const char* title,
    const char* context)
  : FairParGenericSet(name,title,context),
    fGeoSensNodes(new TObjArray()),
    fGeoPassNodes(new TObjArray())
{
}

ERNeuRadGeoPar::~ERNeuRadGeoPar(void)
{
}

void ERNeuRadGeoPar::clear(void)
{
  if(fGeoSensNodes) { delete fGeoSensNodes; }
  if(fGeoPassNodes) { delete fGeoPassNodes; }
}

void ERNeuRadGeoPar::putParams(FairParamList* l)
{
  if (!l) { return; }
  l->addObject("FairGeoNodes Sensitive List", fGeoSensNodes);
  l->addObject("FairGeoNodes Passive List", fGeoPassNodes);
}

Bool_t ERNeuRadGeoPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Sensitive List", fGeoSensNodes)) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Passive List", fGeoPassNodes)) { return kFALSE; }
  return kTRUE;
}

ClassImp(ERNeuRadGeoPar)