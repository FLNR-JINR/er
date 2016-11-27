#include "ERNDSetup.h"

#include <iostream>

#include "TGeoManager.h"

ERNDSetup* ERNDSetup::fInstance = NULL;
TGeoNode* ERNDSetup::fNDnode = NULL;

ERNDSetup::ERNDSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERNDSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    fNDnode  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("ND", TString::kIgnoreCase) ) {
            fNDnode = cave->GetDaughter(iNode);
        }
    }
}

ERNDSetup* ERNDSetup::Instance(){
        if (fInstance == NULL)
                return new ERNDSetup();
        else
                return fInstance;
}

void ERNDSetup::PMTPos(TVector3 pos, TVector3& pmtPos){
	//@TODO пока что высчитывается координата центра стилбена.
	TGeoNode* stilben = gGeoManager->FindNode(pos.X(), pos.Y(), pos.Z());

	TGeoMatrix* stilbenMatrix = stilben->GetMatrix();
	pmtPos.SetXYZ(stilbenMatrix->GetTranslation()[0],
				stilbenMatrix->GetTranslation()[1],
				stilbenMatrix->GetTranslation()[2]);
}