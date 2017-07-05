#include "ERRTelescopeSetup.h"

#include <iostream>

#include "TGeoManager.h"
#include "TGeoSphere.h"
#include "TMath.h"

ERRTelescopeSetup* ERRTelescopeSetup::fInstance = NULL;
TGeoNode* ERRTelescopeSetup::fRTelescopenode = NULL;
Float_t ERRTelescopeSetup::fZ = 0;
Float_t ERRTelescopeSetup::fRmin = 0;
Float_t ERRTelescopeSetup::fRmax = 0;
Int_t ERRTelescopeSetup::fSecNb = 0;
Int_t ERRTelescopeSetup::fSenNb = 0;

ERRTelescopeSetup::ERRTelescopeSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERRTelescopeSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    fRTelescopenode  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("RTelescope", TString::kIgnoreCase) ) {
            fRTelescopenode = cave->GetDaughter(iNode);
        }
    }
    fZ = fRTelescopenode->GetMatrix()->GetTranslation()[2];
    fSecNb = fRTelescopenode->GetNdaughters();
    
    TGeoNode* sector = fRTelescopenode->GetDaughter(0);
    TGeoSphere* sector_shape = (TGeoSphere*)sector->GetVolume()->GetShape();

    fRmin = TMath::Tan(sector_shape->GetTheta1()*TMath::DegToRad())*sector_shape->GetRmin();
    fRmax = TMath::Tan(sector_shape->GetTheta2()*TMath::DegToRad())*sector_shape->GetRmin();
    fSenNb = sector->GetNdaughters();
}

ERRTelescopeSetup* ERRTelescopeSetup::Instance(){
        if (fInstance == NULL)
                return new ERRTelescopeSetup();
        else
                return fInstance;
}

void ERRTelescopeSetup::Print(){
    std::cout << "RTelescope Z position:" << fZ << std::endl;
    std::cout << "RTelescope Rmin, Rmax:" << fRmin << "," << fRmax << std::endl;
    std::cout << "RTelescope SectorNb, SensorNb:" << fSecNb << "," << fSenNb << std::endl;
}
