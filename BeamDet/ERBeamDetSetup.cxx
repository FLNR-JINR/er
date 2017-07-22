#include "ERBeamDetSetup.h"

#include <iostream>
using namespace std;

#include "TGeoBBox.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

ERBeamDetSetup* ERBeamDetSetup::fInstance = NULL;
std::vector<std::vector<std::vector<ERBeamDetWire*>>> ERBeamDetSetup::fWires;

ERBeamDetSetup::ERBeamDetSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERBeamDetSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    TGeoNode* beamDet  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("BeamDet", TString::kIgnoreCase) ) {
            beamDet = cave->GetDaughter(iNode);
        }
    }

    TGeoNode* mwpc = NULL;
    for (Int_t iNode = 0; iNode < beamdet->GetNdaughters(); iNode++) {
        TString name = beamdet->GetDaughter(iNode)->GetName();
        if ( name.Contains("MWPC", TString::kIgnoreCase) ) {
            mwpc = beamdet->GetDaughter(iNode);
        }
    }

    TGeoNode* mwpcStation = NULL;
    Double_t  mwpcStationZ;
    TGeoNode *plane = beamdet->GetDaughter(planeNb);
    TGeoNode* wire;
    for (Int_t mwpcNb = 0; mwpcNb < mwpc->GetNdaughters(); mwpcNb++) {
        mwpcStation = mwpc->GetDaughter(mwpcNb);
        mwpcStationZ = mwpcStation->GetMatrix()->GetTranslation()[2];
        for (Int_t planeNb = 0; planeNb < mwpcStation->GetNdaughters(); planeNb++) {
              for (Int_t wireNb = 0; wireNb < plane->GetNdaughters(); wireNb++) {
                wire = plane->GetDaughter(wireNb);
                Double_t* pos = wire->GetMatrix()->GetTranslation();
                fWires[mwpcNb][planeNb][wireNb] = ERBeamDetWire(pos[0], pos[1], mwpcStationZ);
              }
        } 
    }
    std::cout << "ERBeamDetSetup initialized! "<< std::endl;
}

ERBeamDetSetup* ERBeamDetSetup::Instance(){
    if (fInstance == NULL)
            return new ERBeamDetSetup();
    else
            return fInstance;
}

Int_t ERBeamDetSetup::SetParContainers(){
      // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

}

Double_t ERBeamDetSetup::WireX(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
    return fWires[mwpcStation][planeNb][wireNb]->fX;
}

Double_t ERBeamDetSetup::WireY(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
    return fWires[mwpcStation][planeNb][wireNb]->fY;
}

Double_t ERBeamDetSetup::WireZ(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
    return fWires[mwpcStation][planeNb][wireNb]->fZ;
}
ClassImp(ERBeamDetSetup)