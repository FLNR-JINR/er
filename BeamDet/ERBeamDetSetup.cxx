/*
#include "ERBeamDetSetup.h"

#include<iostream>
using namespace std;

#include "TGeoBBox.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

ERBeamDetSetup* ERBeamDetSetup::fInstance = NULL;
ERBeamDetDigiPar* ERBeamDetSetup::fDigiPar;
Float_t ERBeamDetSetup::fZ;
Float_t ERBeamDetSetup::fLength;
Float_t ERBeamDetSetup::fFiberWidth;
Int_t ERBeamDetSetup::fRowNofFibers;
Int_t ERBeamDetSetup::fRowNofModules;
Int_t ERBeamDetSetup::fRowNofPixels;

std::vector<ERBeamDetModule*> ERBeamDetSetup::fModules;
std::vector<std::vector<ERBeamDetFiber*> > ERBeamDetSetup::fFibers;

ERBeamDetSetup::ERBeamDetSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERBeamDetSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    TGeoNode* BeamDet  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("BeamDet", TString::kIgnoreCase) ) {
            BeamDet = cave->GetDaughter(iNode);
        }
    }
    fZ = BeamDet->GetMatrix()->GetTranslation()[2];
    std::cout << "BeamDet Z position:" << fZ << std::endl;

    TGeoNode* module = BeamDet->GetDaughter(0);
    TGeoBBox* module_box = (TGeoBBox*)module->GetVolume()->GetShape();
    fLength = module_box->GetDZ()*2;
    std::cout << "BeamDet  length:" << fLength << std::endl;

    TGeoNode* pixel = module->GetDaughter(0);
    TGeoNode* fiber = pixel->GetDaughter(0); // fiber with clading and dead zone
    TGeoBBox* fiber_box = (TGeoBBox*)fiber->GetVolume()->GetShape();
    fFiberWidth = fiber_box->GetDX()*2;
    std::cout << "BeamDet  fiber width:" << fFiberWidth << std::endl;
    
    fRowNofModules = Int_t(TMath::Sqrt(BeamDet->GetNdaughters()));
    fRowNofPixels = Int_t(TMath::Sqrt(module->GetNdaughters()));
    fRowNofFibers = Int_t(TMath::Sqrt(pixel->GetNdaughters()));

    std::cout << "BeamDet  modules in row count:" << fRowNofModules << std::endl;
    std::cout << "BeamDet  pixels in row count:" << fRowNofPixels << std::endl;
    std::cout << "BeamDet  fibers in row count:" << fRowNofFibers << std::endl;

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
  fDigiPar = (ERBeamDetDigiPar*)
           (rtdb->getContainer("ERBeamDetDigiPar"));
  if (fDigiPar) {
      std::cout << "ERBeamDetSetup::SetParContainers() "<< std::endl;
      std::cout << "ERBeamDetDigiPar initialized! "<< std::endl;
      return 0;
  }
  return 1;
}

Int_t   ERBeamDetSetup::NofFibers() {
        return fRowNofFibers*fRowNofFibers;
}

Int_t   ERBeamDetSetup::NofPixels(){
        return fRowNofPixels*fRowNofPixels;
}

Int_t   ERBeamDetSetup::NofModules() {
        return fRowNofModules*fRowNofModules;
}

Float_t ERBeamDetSetup::FiberLength() {
        return fLength;
}

Float_t ERBeamDetSetup::FiberWidth() {
        return fFiberWidth;
}

Float_t ERBeamDetSetup::ModuleX(Int_t iModule){
        return fModules[iModule]->fX;
}

Float_t ERBeamDetSetup::ModuleY(Int_t iModule){
        return fModules[iModule]->fY;
}

Float_t ERBeamDetSetup::FiberX(Int_t iModule, Int_t iFiber){
        return fFibers[iModule][iFiber]->fX;
}

Float_t ERBeamDetSetup::FiberY(Int_t iModule, Int_t iFiber){
        return fFibers[iModule][iFiber]->fY;
}

Float_t ERBeamDetSetup::PixelQuantumEfficiency(Int_t iModule, Int_t iFiber){
        return fDigiPar->PixelQuantumEfficiency(iFiber);
}

Float_t ERBeamDetSetup::PixelGain(Int_t iModule, Int_t iFiber){
        return fDigiPar->PixelGain(iFiber);
}

Float_t ERBeamDetSetup::PixelSigma(Int_t iModule, Int_t iFiber){
        return fDigiPar->PixelSigma(iFiber);
}

void ERBeamDetSetup::Print(){
        fDigiPar->print();
}

void ERBeamDetSetup::Crosstalks(Int_t iFiber, TArrayF& crosstalks){
    return fDigiPar->Crosstalks(iFiber, crosstalks);
}

Int_t ERBeamDetSetup::RowNofFibers(){
    return fRowNofFibers;
}

Int_t ERBeamDetSetup::RowNofPixels(){
  return fRowNofPixels;
}

Int_t ERBeamDetSetup::RowNofModules(){
    return fRowNofModules;
}

Bool_t ERBeamDetSetup::UseCrosstalks(){
    return fDigiPar->UseCrosstalks();
}

ClassImp(ERBeamDetSetup)
*/