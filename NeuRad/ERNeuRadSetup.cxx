/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadSetup.h"

#include "TGeoBBox.h"
#include "TMath.h"
#include "TGeoManager.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERNeuRadDigiPar.h"

ERNeuRadSetup* ERNeuRadSetup::fInstance = NULL;
ERNeuRadDigiPar* ERNeuRadSetup::fDigiPar; //TODO initialize!!!
Float_t ERNeuRadSetup::fZ; //TODO initialize!!!
Float_t ERNeuRadSetup::fLength; //TODO initialize!!!
Float_t ERNeuRadSetup::fFiberWidth; //TODO initialize!!!
Int_t ERNeuRadSetup::fRowNofFibers; //TODO initialize!!!
Int_t ERNeuRadSetup::fRowNofModules; //TODO initialize!!!
Int_t ERNeuRadSetup::fRowNofPixels; //TODO initialize!!!

std::vector<ERNeuRadModule*> ERNeuRadSetup::fModules;
std::vector<std::vector<ERNeuRadFiber*> > ERNeuRadSetup::fFibers;

ERNeuRadSetup::ERNeuRadSetup() {
    // --- Catch absence of TGeoManager
    if (!gGeoManager) {
      LOG(FATAL) << "ERNeuRadSetup: cannot initialise without TGeoManager!" << FairLogger::endl;
    }

    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    TGeoNode* neuRad  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("NeuRad", TString::kIgnoreCase) ) {
            neuRad = cave->GetDaughter(iNode);
        }
    }
    fZ = neuRad->GetMatrix()->GetTranslation()[2];
    LOG(INFO) << "NeuRad Z position:" << fZ << FairLogger::endl;

    TGeoNode* module = neuRad->GetDaughter(0);
    TGeoBBox* module_box = (TGeoBBox*)module->GetVolume()->GetShape();
    fLength = module_box->GetDZ()*2;
    LOG(INFO) << "NeuRad length:" << fLength << FairLogger::endl;

    TGeoNode* pixel = module->GetDaughter(0);
    TGeoNode* fiber = pixel->GetDaughter(0); // fiber with clading and dead zone
    TGeoBBox* fiber_box = (TGeoBBox*)fiber->GetVolume()->GetShape();
    fFiberWidth = fiber_box->GetDX()*2;
    LOG(INFO) << "NeuRad fiber width:" << fFiberWidth << FairLogger::endl;
    
    fRowNofModules = Int_t(TMath::Sqrt(neuRad->GetNdaughters()));
    fRowNofPixels = Int_t(TMath::Sqrt(module->GetNdaughters()));
    fRowNofFibers = Int_t(TMath::Sqrt(pixel->GetNdaughters()));

    //Обработка субмодулей в новой геометрии
    Int_t iSubm = -1; //Любой subm
    Int_t nSubm = 0;
    for (Int_t iNode = 0; iNode < module->GetNdaughters(); iNode++){
      if (TString(module->GetDaughter(iNode)->GetName()).Contains("submodul")){
        iSubm = iNode;
        nSubm++;
      }
    }
    Int_t nPixel_in_subm = 0;
    if (iSubm > -1){
      LOG(INFO) << "Submodules in geometry!" << FairLogger::endl;
      TGeoNode* subm = module->GetDaughter(iSubm);
      for (Int_t iNode = 0; iNode < subm->GetNdaughters(); iNode++){
        if (TString(subm->GetDaughter(iNode)->GetName()).Contains("pixel")){
          pixel = subm->GetDaughter(iNode);
          nPixel_in_subm++;
        }
      }
      fRowNofPixels = Int_t(TMath::Sqrt(nSubm))*Int_t(TMath::Sqrt(nPixel_in_subm));
      fRowNofFibers = Int_t(TMath::Sqrt(pixel->GetNdaughters()));
    }

    LOG(INFO) << "NeuRad modules in row count:" << fRowNofModules << FairLogger::endl;
    LOG(INFO) << "NeuRad pixels in row count:" << fRowNofPixels << FairLogger::endl;
    LOG(INFO) << "NeuRad fibers in row count:" << fRowNofFibers << FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup initialized! "<< FairLogger::endl;
}

ERNeuRadSetup* ERNeuRadSetup::Instance() {
  if (fInstance == NULL) {
    return new ERNeuRadSetup();
  } else {
    return fInstance;
  }
}

//TODO what does the return value mean?
Int_t ERNeuRadSetup::SetParContainers() {
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  fDigiPar = (ERNeuRadDigiPar*) (rtdb->getContainer("ERNeuRadDigiPar"));
  if (fDigiPar) {
    LOG(INFO) << "ERNeuRadSetup::SetParContainers() "<< FairLogger::endl;
    LOG(INFO) << "ERNeuRadDigiPar initialized! "<< FairLogger::endl;
    return 0;
  }
  return 1;
}

Int_t ERNeuRadSetup::NofFibers() {
  return fRowNofFibers*fRowNofFibers;
}

Int_t ERNeuRadSetup::NofPixels(){
  return fRowNofPixels*fRowNofPixels;
}

Int_t ERNeuRadSetup::NofModules() {
  return fRowNofModules*fRowNofModules;
}

Float_t ERNeuRadSetup::FiberLength() {
  return fLength;
}

Float_t ERNeuRadSetup::FiberWidth() {
  return fFiberWidth;
}

Float_t ERNeuRadSetup::ModuleX(Int_t iModule) {
  return fModules[iModule]->fX;
}

Float_t ERNeuRadSetup::ModuleY(Int_t iModule) {
  return fModules[iModule]->fY;
}

Float_t ERNeuRadSetup::FiberX(Int_t iModule, Int_t iFiber) {
  return fFibers[iModule][iFiber]->fX;
}

Float_t ERNeuRadSetup::FiberY(Int_t iModule, Int_t iFiber) {
  return fFibers[iModule][iFiber]->fY;
}

Float_t ERNeuRadSetup::PixelQuantumEfficiency(Int_t iModule, Int_t iFiber) {
  return fDigiPar->PixelQuantumEfficiency(iFiber);
}

Float_t ERNeuRadSetup::PixelGain(Int_t iModule, Int_t iFiber) {
  return fDigiPar->PixelGain(iFiber);
}

Float_t ERNeuRadSetup::PixelSigma(Int_t iModule, Int_t iFiber) {
  return fDigiPar->PixelSigma(iFiber);
}

void ERNeuRadSetup::Print() {
  fDigiPar->print();
}

void ERNeuRadSetup::Crosstalks(Int_t iFiber, TArrayF& crosstalks) {
  return fDigiPar->Crosstalks(iFiber, crosstalks);
}

Int_t ERNeuRadSetup::RowNofFibers() {
  return fRowNofFibers;
}

Int_t ERNeuRadSetup::RowNofPixels() {
  return fRowNofPixels;
}

Int_t ERNeuRadSetup::RowNofModules() {
  return fRowNofModules;
}

Bool_t ERNeuRadSetup::UseCrosstalks() {
  return fDigiPar->UseCrosstalks();
}

ClassImp(ERNeuRadSetup)
