#include "ERNeuRadSetup.h"

#include<iostream>

#include "TGeoBBox.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

ERNeuRadSetup* ERNeuRadSetup::fInstance = NULL;
ERNeuRadDigiPar* ERNeuRadSetup::fDigiPar;
Float_t ERNeuRadSetup::fZ;
Float_t ERNeuRadSetup::fLength;
Float_t ERNeuRadSetup::fFiberWidth;

std::vector<ERNeuRadModule*> ERNeuRadSetup::fModules;
std::vector<std::vector<ERNeuRadFiber*> > ERNeuRadSetup::fFibers;

ERNeuRadSetup::ERNeuRadSetup(){
    // --- Catch absence of TGeoManager

    if ( ! gGeoManager ) {
            std::cerr << "ERNeuRadSetup: cannot initialise without TGeoManager!"<< std::endl;
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
    std::cout << "NeuRad Z position:" << fZ << std::endl;

    TGeoNode* module = neuRad->GetDaughter(0);
    TGeoBBox* module_box = (TGeoBBox*)module->GetVolume()->GetShape();
    fLength = module_box->GetDZ()*2;
    std::cout << "NeuRad  length:" << fLength << std::endl;

    TGeoNode* fiber = module->GetDaughter(0);
    TGeoBBox* fiber_box = (TGeoBBox*)fiber->GetVolume()->GetShape();
    fFiberWidth = fiber_box->GetDX()*2;
    std::cout << "NeuRad  fiber width:" << fFiberWidth << std::endl;

    // Get run and runtime database
    FairRunAna* run = FairRunAna::Instance();
    if ( ! run ) Fatal("ERNeuRadSetup", "No analysis run");
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if ( ! rtdb ) Fatal("ERNeuRadSetup", "No runtime database");
    fDigiPar = (ERNeuRadDigiPar*)
         (rtdb->getContainer("ERNeuRadDigiPar"));
    if ( ! fDigiPar ) Fatal("ERNeuRadSetup", "No ERNeuRadDigiPar in runtime");
    Int_t nofFiberInRow = Int_t(TMath::Sqrt(fDigiPar->NofFibers()));
    Int_t nofModulesInRow = Int_t(TMath::Sqrt(fDigiPar->NofModules()));
    Float_t moduleWidth = nofFiberInRow*fFiberWidth;
    Float_t neuradWidth = moduleWidth*nofModulesInRow;
    
    for(Int_t iModule = 0; iModule < nofModulesInRow; iModule++ ){
            for(Int_t jModule = 0; jModule < nofModulesInRow; jModule++ ){
                    Float_t moduleX = neuradWidth/2. - iModule*moduleWidth - moduleWidth/2.;
                    Float_t moduleY = neuradWidth/2. - jModule*moduleWidth - moduleWidth/2.;
                    fModules.push_back(new ERNeuRadModule(moduleX,moduleY));
                    std::vector<ERNeuRadFiber*> fibersInModule;

                    for (Int_t iFiber = 0; iFiber < nofFiberInRow; iFiber++){
                            for (Int_t jFiber = 0; jFiber < nofFiberInRow; jFiber++){
                                    Float_t fiberX = moduleX + moduleWidth/2. - iFiber*fFiberWidth - fFiberWidth/2.;
                                    Float_t fiberY = moduleY + moduleWidth/2. - jFiber*fFiberWidth - fFiberWidth/2.;
                                    fibersInModule.push_back(new ERNeuRadFiber(fiberX,fiberY));
                            }
                    }
                    fFibers.push_back(fibersInModule);
            }
    }
    std::cout << "ERNeuRadSetup initialized! "<< std::endl;
}

ERNeuRadSetup* ERNeuRadSetup::Instance(){
        if (fInstance == NULL)
                return new ERNeuRadSetup();
        else
                return fInstance;
}

Int_t  ERNeuRadSetup::NofFibers() {
        return fDigiPar->NofFibers();
}

Int_t   ERNeuRadSetup::NofModules() {
        return fDigiPar->NofModules();
}

Float_t ERNeuRadSetup::FiberLength() {
        return fLength;
}

Float_t ERNeuRadSetup::FiberWidth() {
        return fFiberWidth;
}

Float_t ERNeuRadSetup::ModuleX(Int_t iModule){
        return fModules[iModule]->fX;
}

Float_t ERNeuRadSetup::ModuleY(Int_t iModule){
        return fModules[iModule]->fY;
}

Float_t ERNeuRadSetup::FiberX(Int_t iModule, Int_t iFiber){
        return fFibers[iModule][iFiber]->fX;
}

Float_t ERNeuRadSetup::FiberY(Int_t iModule, Int_t iFiber){
        return fFibers[iModule][iFiber]->fY;
}

Float_t ERNeuRadSetup::PMTQuantumEfficiency(Int_t iModule, Int_t iFiber){
        return fDigiPar->PMTQuantumEfficiency(iFiber);
}

Float_t ERNeuRadSetup::PMTGain(Int_t iModule, Int_t iFiber){
        return fDigiPar->PMTGain(iFiber);
}

Float_t ERNeuRadSetup::PMTSigma(Int_t iModule, Int_t iFiber){
        return fDigiPar->PMTSigma(iFiber);
}

void ERNeuRadSetup::Print(){
        fDigiPar->print();
}

void ERNeuRadSetup::PMTCrosstalks(Int_t iFiber, TArrayF& crosstalks){
    return fDigiPar->PMTCrosstalks(iFiber, crosstalks);
}

Int_t ERNeuRadSetup::RowNofFibers(){
    return fDigiPar->RowNofFibers();
}

Bool_t ERNeuRadSetup::UseCrosstalks(){
    return fDigiPar->UseCrosstalks();
}

ClassImp(ERNeuRadSetup)
