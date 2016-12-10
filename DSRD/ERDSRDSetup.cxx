#include "ERDSRDSetup.h"

#include <iostream>

#include "TGeoManager.h"

ERDSRDSetup* ERDSRDSetup::fInstance = NULL;
TGeoNode* ERDSRDSetup::fDSRDnode = NULL;

ERDSRDSetup::ERDSRDSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERDSRDSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    fDSRDnode  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("ND", TString::kIgnoreCase) ) {
            fNDnode = cave->GetDaughter(iNode);
        }
    }
}

ERDSRDSetup* ERDSRDSetup::Instance(){
        if (fInstance == NULL)
                return new ERDSRDSetup();
        else
                return fInstance;
}

Float_t ERDSRDSetup::SensorNb(){
    
}

Float_t ERDSRDSetup::SectorNb(){
    
}