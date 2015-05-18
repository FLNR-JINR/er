// -------------------------------------------------------------------------
// -----                       ERCave source file                      -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "FairMCApplication.h"
#include "ERCave.h"
#include "FairVolume.h"
#include "FairVolumeList.h"
#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoRootBuilder.h"
#include "FairRuntimeDb.h"
#include "TObjArray.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRun.h"

#include "ERGeoCave.h"
#include "ERGeoPassivePar.h"


ClassImp(ERCave)
void ERCave::ConstructGeometry()
{
	FairGeoLoader *loader=FairGeoLoader::Instance();
	FairGeoInterface *GeoInterface =loader->getGeoInterface();
	ERGeoCave *MGeo=new ERGeoCave();
	MGeo->setGeomFile(GetGeometryFileName());
	GeoInterface->addGeoModule(MGeo);
	Bool_t rc = GeoInterface->readSet(MGeo);
	if ( rc ) MGeo->create(loader->getGeoBuilder());

    TList* volList = MGeo->getListOfVolumes();
        // store geo parameter
	/*
	FairRun *fRun = FairRun::Instance();
    FairRuntimeDb *rtdb= FairRun::Instance()->GetRuntimeDb();
	
	R3BGeoPassivePar* par=(R3BGeoPassivePar*)(rtdb->getContainer("R3BGeoPassivePar"));
        TObjArray *fSensNodes = par->GetGeoSensitiveNodes();
        TObjArray *fPassNodes = par->GetGeoPassiveNodes();

        TListIter iter(volList);
        FairGeoNode* node   = NULL;
        FairGeoVolume *aVol=NULL;

        while( (node = (FairGeoNode*)iter.Next()) ) {
            aVol = dynamic_cast<FairGeoVolume*> ( node );
            if ( node->isSensitive()  ) {
                fSensNodes->AddLast( aVol );
            }else{
                fPassNodes->AddLast( aVol );
            }
        }
        par->setChanged();
        par->setInputVersion(fRun->GetRunId(),1);
*/

}

ERCave::ERCave(){
}

ERCave::~ERCave(){
}


ERCave::ERCave(const char * name,  const char *Title)
  : FairModule(name ,Title)
{
    world[0] = 0;
    world[1] = 0;
    world[2] = 0;
}
