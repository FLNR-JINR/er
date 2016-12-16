#include "ERDSRDSetup.h"

#include <iostream>

#include "TGeoManager.h"
#include "TGeoSphere.h"
#include "TMath.h"

ERDSRDSetup* ERDSRDSetup::fInstance = NULL;
TGeoNode* ERDSRDSetup::fDSRDnode = NULL;
Float_t ERDSRDSetup::fZ = 0;
Float_t ERDSRDSetup::fRmin = 0;
Float_t ERDSRDSetup::fRmax = 0;
Int_t ERDSRDSetup::fSecNb = 0;
Int_t ERDSRDSetup::fSenNb = 0;

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
        if ( name.Contains("DSRD", TString::kIgnoreCase) ) {
            fDSRDnode = cave->GetDaughter(iNode);
        }
    }
    fZ = fDSRDnode->GetMatrix()->GetTranslation()[2];
    fSecNb = fDSRDnode->GetNdaughters();
    
    TGeoNode* sector = fDSRDnode->GetDaughter(0);
    TGeoSphere* sector_shape = (TGeoSphere*)sector->GetVolume()->GetShape();

    fRmin = TMath::Tan(sector_shape->GetTheta1()*TMath::DegToRad())*sector_shape->GetRmin();
    fRmax = TMath::Tan(sector_shape->GetTheta2()*TMath::DegToRad())*sector_shape->GetRmin();
    fSenNb = sector->GetNdaughters();
}

ERDSRDSetup* ERDSRDSetup::Instance(){
        if (fInstance == NULL)
                return new ERDSRDSetup();
        else
                return fInstance;
}

void ERDSRDSetup::Print(){
    std::cout << "DSRD Z position:" << fZ << std::endl;
    std::cout << "DSRD Rmin, Rmax:" << fRmin << "," << fRmax << std::endl;
    std::cout << "DSRD SectorNb, SensorNb:" << fSecNb << "," << fSenNb << std::endl;
}