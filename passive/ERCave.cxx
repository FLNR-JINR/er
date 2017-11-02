// -------------------------------------------------------------------------
// -----           ERCave source file                                  -----
// -----           Created 11/12/15 by V.Schetinin                     -----
// -------------------------------------------------------------------------

#include "ERCave.h"

#include "ERGeoCave.h"
#include "ERGeoPassivePar.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairGeoNode.h"

#include "TList.h"
#include "TObjArray.h"

void ERCave::ConstructGeometry()
{
	FairGeoLoader *loader = FairGeoLoader::Instance();
	FairGeoInterface *GeoInterface = loader->getGeoInterface();

	ERGeoCave *MGeo = new ERGeoCave();
	MGeo->setGeomFile(GetGeometryFileName());
	GeoInterface->addGeoModule(MGeo);
	Bool_t rc = GeoInterface->readSet(MGeo);
	if (rc) MGeo->create(loader->getGeoBuilder());

	TList* volList = MGeo->getListOfVolumes();
	// store geo parameter

//TODO this piece of code is used in CBM. Here it does not work...
/*
	FairRun *fRun = FairRun::Instance();
	FairRuntimeDb *rtdb = FairRun::Instance()->GetRuntimeDb();
	ERGeoPassivePar* par = (ERGeoPassivePar*)(rtdb->getContainer("ERGeoPassivePar"));
	TObjArray *fSensNodes = par->GetGeoSensitiveNodes();
	TObjArray *fPassNodes = par->GetGeoPassiveNodes();

	TListIter iter(volList);
	FairGeoNode* node = NULL;
	FairGeoVolume *aVol = NULL;

	while( (node = (FairGeoNode*)iter.Next()) ) {
		aVol = dynamic_cast<FairGeoVolume*> (node);
		if (node->isSensitive()) {
			fSensNodes->AddLast(aVol);
		} else {
			fPassNodes->AddLast(aVol);
		}
	}
	par->setChanged();
	par->setInputVersion(fRun->GetRunId(),1);
*/
}

ERCave::ERCave()
	: FairModule(),
	world()
{
}

ERCave::ERCave(const char* name, const char* title)
	: FairModule(name, title),
	world()
{
	world[0] = 0;
	world[1] = 0;
	world[2] = 0;
}

ERCave::~ERCave()
{
}

ClassImp(ERCave)
