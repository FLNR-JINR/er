#include "ERBeamDetSetup.h"

#include <iostream>
using namespace std;

#include "TGeoTube.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

ERBeamDetSetup* ERBeamDetSetup::fInstance = NULL;
Double_t        ERBeamDetSetup::fTargetR = 0;
map<Int_t, map<Int_t, map<Int_t, ERBeamDetWire*>>>ERBeamDetSetup::fWires;

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
            break;
        }
    }

    TGeoNode* mwpc = NULL;
    for (Int_t iNode = 0; iNode < beamDet->GetNdaughters(); iNode++) {
        TString name = beamDet->GetDaughter(iNode)->GetName();
        if ( name.Contains("MWPC", TString::kIgnoreCase) ) {
            mwpc = beamDet->GetDaughter(iNode);
            break;
        }
    }

    TGeoNode*   mwpcStation = NULL;
    Double_t    mwpcStationZ;
    TGeoMatrix* mwpcLocalPos;
    Double_t    mwpcMasterPos[3];
    TGeoNode*   plane = NULL;
    TGeoNode*   wire = NULL;

    for (Int_t mwpcNb = 0; mwpcNb < mwpc->GetNdaughters(); mwpcNb++) {
      mwpcStation = mwpc->GetDaughter(mwpcNb);
      //---- Convertion mwpc station local position to global coordinates---
      mwpcLocalPos = mwpcStation->GetMatrix();
      mwpcLocalPos->LocalToMaster(mwpc->GetMatrix()->GetTranslation(), mwpcMasterPos);
      mwpcStationZ = mwpcMasterPos[2];
      //--------------------------------------------------------------------
      for (Int_t planeNb = 0; planeNb < mwpcStation->GetNdaughters(); planeNb++) {
        plane = mwpcStation->GetDaughter(planeNb);
        for (Int_t wireNb = 0; wireNb < plane->GetNdaughters(); wireNb++) {
          wire = plane->GetDaughter(wireNb);
          Double_t x = wire->GetMatrix()->GetTranslation()[0];
          Double_t y = wire->GetMatrix()->GetTranslation()[1];
          (planeNb == 0) ? fWires[mwpcNb][planeNb].insert(std::make_pair(wireNb, new ERBeamDetWire(x, y, mwpcStationZ)))
                         : fWires[mwpcNb][planeNb].insert(std::make_pair(wireNb, new ERBeamDetWire(y, x, mwpcStationZ)));
          std::cout << "Wire " << wireNb << " position (" << fWires[mwpcNb][planeNb][wireNb]->fX << ", " 
                                                          << fWires[mwpcNb][planeNb][wireNb]->fY << ", " 
                                                          << mwpcStationZ << ") cm" << endl;
        }
      } 
    }

    TGeoNode* target = NULL;
    for (Int_t iNode = 0; iNode < beamDet->GetNdaughters(); iNode++) {
        TString name = beamDet->GetDaughter(iNode)->GetName();
        if ( name.Contains("target", TString::kIgnoreCase) ) {
            target = beamDet->GetDaughter(iNode);
            TGeoNode* shell = target->GetDaughter(0);
            cout << "Shell " << shell->GetName() << endl;
            TGeoNode* h2 = shell->GetDaughter(0);
            cout << "h2 " << h2->GetName() << endl;
            TGeoTube* h2Tube = (TGeoTube*)h2->GetVolume()->GetShape();
            fTargetR = h2Tube->GetRmax();
            std::cout<< "Target radius " << fTargetR << std::endl;
            break;
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
    return fWires[mwpcNb][planeNb][wireNb]->fX;
}

Double_t ERBeamDetSetup::WireY(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
    return fWires[mwpcNb][planeNb][wireNb]->fY;
}

Double_t ERBeamDetSetup::WireZ(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
    return fWires[mwpcNb][planeNb][wireNb]->fZ;
}

Double_t ERBeamDetSetup::TargetR() {
    return fTargetR;
}

ClassImp(ERBeamDetSetup)