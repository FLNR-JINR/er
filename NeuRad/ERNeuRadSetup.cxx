/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadSetup.h"

#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TMath.h"

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

    // Get the pointer to the cave node as the top node of the geometry manager
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();

    // --------------------------------------------------------------------------------------------------------------

    // Search for NeuRad node
    TGeoNode* neuRad = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("NeuRad", TString::kIgnoreCase) ) {
            neuRad = cave->GetDaughter(iNode);
            LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad node found. name=" << name << FairLogger::endl;
            break;
        }
    }

    // Get Z position of NeuRad
    fZ = neuRad->GetMatrix()->GetTranslation()[2];
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad Z position:" << fZ << FairLogger::endl;

    // --------------------------------------------------------------------------------------------------------------

    // Search for a module node as the first child of NeuRad node
    TGeoNode* module = neuRad->GetDaughter(0);
    TString moduleNodeName = module->GetName();
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "module node name=" << moduleNodeName << FairLogger::endl;

    UInt_t iModulesCounter = 0;

    // If not found - search one level deeper
    if (! moduleNodeName.Contains("module", TString::kIgnoreCase)) {
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong module node name! Trying again." << FairLogger::endl;
      module = neuRad->GetDaughter(0)->GetDaughter(0);
      moduleNodeName = module->GetName();
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "module node name=" << moduleNodeName << FairLogger::endl;
      if (! moduleNodeName.Contains("module", TString::kIgnoreCase)) {
        LOG(FATAL) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong module node name! Aborting." << FairLogger::endl;
      } else {
        // Count the number of modules
      // TODO unchecked code
        for (UInt_t iDaughterNode=0; iDaughterNode<neuRad->GetDaughter(0)->GetNdaughters(); iDaughterNode++) {
          moduleNodeName = neuRad->GetDaughter(0)->GetDaughter(iDaughterNode)->GetName();
          if (moduleNodeName.Contains("module", TString::kIgnoreCase)) iModulesCounter++;
        }
      }
    } else {
      // Count the number of modules
      for (UInt_t iDaughterNode=0; iDaughterNode<neuRad->GetNdaughters(); iDaughterNode++) {
        moduleNodeName = neuRad->GetDaughter(iDaughterNode)->GetName();
        if (moduleNodeName.Contains("module", TString::kIgnoreCase)) iModulesCounter++;
      }
    }

    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "Found " << iModulesCounter << " modules" << FairLogger::endl;

    // Get module length along Z
    TGeoBBox* module_box = (TGeoBBox*)module->GetVolume()->GetShape();
    fLength = module_box->GetDZ()*2;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "module length (Z):" << fLength << FairLogger::endl;

    // --------------------------------------------------------------------------------------------------------------

    // Search for a pixel node as the first child of a module node
    TGeoNode* pixel = module->GetDaughter(0);
    TString pixelNodeName = pixel->GetName();
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "pixel node name=" << pixelNodeName << FairLogger::endl;

    UInt_t iSubmodulesCounter = 0;
    UInt_t iPixelsCounter = 0;

    // If not found - search one level deeper
    if (! pixelNodeName.Contains("pixel", TString::kIgnoreCase)) {
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong pixel node name! Trying again." << FairLogger::endl;
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "probably there are submodules - check!" << FairLogger::endl;
      if (pixelNodeName.Contains("submodul", TString::kIgnoreCase)) {
        LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "indeed, submodule found. Count how many of them are in the module." << FairLogger::endl;
        TString submoduleNodeName;
        // Count the number of submodules
        for (UInt_t iDaughterNode=0; iDaughterNode<module->GetNdaughters(); iDaughterNode++) {
          submoduleNodeName = module->GetDaughter(iDaughterNode)->GetName();
          if (submoduleNodeName.Contains("submodul", TString::kIgnoreCase)) iSubmodulesCounter++;
        }
        LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "Found " << iSubmodulesCounter << " submodules in a module" << FairLogger::endl;
      } else {
        //TODO something is wrong, but not completely wrong...
      }

      pixel = module->GetDaughter(0)->GetDaughter(0);
      pixelNodeName = pixel->GetName();
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "pixel node name=" << pixelNodeName << FairLogger::endl;
      if (! pixelNodeName.Contains("pixel", TString::kIgnoreCase)) {
        LOG(FATAL) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong pixel node name! Aborting." << FairLogger::endl;
      } else {
        // Count the number of pixels in one submodule
        for (UInt_t iDaughterNode=0; iDaughterNode<module->GetDaughter(0)->GetNdaughters(); iDaughterNode++) {
          pixelNodeName = module->GetDaughter(0)->GetDaughter(iDaughterNode)->GetName();
          if (pixelNodeName.Contains("pixel", TString::kIgnoreCase)) iPixelsCounter++;
        }
        // Take into account that there are several submodules with a few pixels
        iPixelsCounter *= iSubmodulesCounter;
      }
    } else {
      // Count the number of modules
      // TODO unchecked code
      for (UInt_t iDaughterNode=0; iDaughterNode<module->GetNdaughters(); iDaughterNode++) {
        pixelNodeName = module->GetDaughter(iDaughterNode)->GetName();
        if (pixelNodeName.Contains("pixel", TString::kIgnoreCase)) iPixelsCounter++;
      }
    }

    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: "
              << "Found " << iPixelsCounter << " pixels in " << iSubmodulesCounter << " submodules." << FairLogger::endl;

    // --------------------------------------------------------------------------------------------------------------

    // Search for a fiber as the first child of a pixel
    TGeoNode* fiber = pixel->GetDaughter(0); // fiber with clading and dead zone - TODO?
    TString fiberNodeName = fiber->GetName();
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "fiber node name=" << fiberNodeName << FairLogger::endl;

    UInt_t iCladdingsCounter = 0;
    UInt_t iFibersCounter = 0;

    // If not found - search one level deeper
    if (! fiberNodeName.Contains("fiber", TString::kIgnoreCase)) {
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong fiber node name! Trying again." << FairLogger::endl;
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "probably there are claddings - check!" << FairLogger::endl;

      if (fiberNodeName.Contains("cladding", TString::kIgnoreCase)) {
        LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "indeed, cladding found. Count how many of them are in the pixel." << FairLogger::endl;
        TString claddingNodeName;
        // Count the number of claddings
        for (UInt_t iDaughterNode=0; iDaughterNode<pixel->GetNdaughters(); iDaughterNode++) {
          claddingNodeName = pixel->GetDaughter(iDaughterNode)->GetName();
          if (claddingNodeName.Contains("cladding", TString::kIgnoreCase)) iCladdingsCounter++;
        }
        LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "Found " << iCladdingsCounter << " claddings in a pixel." << FairLogger::endl;
      } else {
        //TODO something is wrong, but not completely wrong...
      }

      fiber = pixel->GetDaughter(0)->GetDaughter(0);
      fiberNodeName = fiber->GetName();
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "fiber node name=" << fiberNodeName << FairLogger::endl;
      if (! fiberNodeName.Contains("fiber", TString::kIgnoreCase)) {
        LOG(FATAL) << "ERNeuRadSetup::ERNeuRadSetup: " << "wrong fiber node name! Aborting." << FairLogger::endl;
      } else {
        // Count the number of fibers in one cladding
        for (UInt_t iDaughterNode=0; iDaughterNode<pixel->GetDaughter(0)->GetNdaughters(); iDaughterNode++) {
          fiberNodeName = pixel->GetDaughter(0)->GetDaughter(iDaughterNode)->GetName();
          if (fiberNodeName.Contains("fiber", TString::kIgnoreCase)) iFibersCounter++;
        }
        // Take into account that there are several fibers with a few pixels
        iFibersCounter *= iCladdingsCounter;
        // Take into account that there are several claddings (with fibers) in a pixel
        iFibersCounter *= iPixelsCounter;
      }
    } else {
      // Count the number of fibers
      // TODO unchecked code
      for (UInt_t iDaughterNode=0; iDaughterNode<pixel->GetNdaughters(); iDaughterNode++) {
        fiberNodeName = pixel->GetDaughter(iDaughterNode)->GetName();
        if (fiberNodeName.Contains("fiber", TString::kIgnoreCase)) iFibersCounter++;
      }
    }

    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "Found " << iFibersCounter
              << " fibers in " << iPixelsCounter << " pixels." << FairLogger::endl;

    // Get fiber width along X
    TGeoBBox* fiber_box = (TGeoBBox*)fiber->GetVolume()->GetShape();
    fFiberWidth = fiber_box->GetDX()*2;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "fiber width (X):" << fFiberWidth << FairLogger::endl;
    
    // --------------------------------------------------------------------------------------------------------------

    fRowNofModules = Int_t(TMath::Sqrt(iModulesCounter));
    fRowNofPixels = Int_t(TMath::Sqrt(iPixelsCounter));
    fRowNofFibers = Int_t(TMath::Sqrt(iFibersCounter));
/*
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad modules in row count:" << fRowNofModules << FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad pixels in row count:" << fRowNofPixels << FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad fibers in row count:" << fRowNofFibers << FairLogger::endl;

    // Обработка субмодулей в новой геометрии
    Int_t iSubm = -1; // Любой subm
    Int_t nSubm = 0;
    for (Int_t iNode = 0; iNode < module->GetNdaughters(); iNode++) {
      if (TString(module->GetDaughter(iNode)->GetName()).Contains("submodul")) {
        iSubm = iNode;
        nSubm++;
      }
    }
    Int_t nPixel_in_subm = 0;
    if (iSubm > -1) {
      LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "Submodules in geometry!" << FairLogger::endl;
      TGeoNode* subm = module->GetDaughter(iSubm);
      for (Int_t iNode = 0; iNode < subm->GetNdaughters(); iNode++) {
        if (TString(subm->GetDaughter(iNode)->GetName()).Contains("pixel")) {
          pixel = subm->GetDaughter(iNode);
          nPixel_in_subm++;
        }
      }
      fRowNofPixels = Int_t(TMath::Sqrt(nSubm))*Int_t(TMath::Sqrt(nPixel_in_subm));
      fRowNofFibers = Int_t(TMath::Sqrt(pixel->GetNdaughters()));
    }
*/
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad modules in row count:" << fRowNofModules << FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad pixels in row count:" << fRowNofPixels << FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "NeuRad fibers in row count:" << fRowNofFibers << FairLogger::endl;

    LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup: " << "initialized! "<< FairLogger::endl;
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
    LOG(INFO) << "ERNeuRadSetup::SetParContainers: " << "ERNeuRadDigiPar initialized! "<< FairLogger::endl;
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
