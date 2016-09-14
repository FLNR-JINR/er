#include "ERNeuRadSetup.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>

ERNeuRadSetup* ERNeuRadSetup::fInstance = NULL;
ERNeuRadDigiPar* ERNeuRadSetup::fDigiPar;
Float_t ERNeuRadSetup::fZ;

std::vector<ERNeuRadBundle*> ERNeuRadSetup::fBundles;
std::vector<std::vector<ERNeuRadFiber*> > ERNeuRadSetup::fFibers;

ERNeuRadSetup::ERNeuRadSetup(){
        // Get run and runtime database
        FairRunAna* run = FairRunAna::Instance();
        if ( ! run ) Fatal("ERNeuRadSetup", "No analysis run");
        FairRuntimeDb* rtdb = run->GetRuntimeDb();
        if ( ! rtdb ) Fatal("ERNeuRadSetup", "No runtime database");
        fDigiPar = (ERNeuRadDigiPar*)
             (rtdb->getContainer("ERNeuRadDigiPar"));
        if ( ! fDigiPar ) Fatal("ERNeuRadSetup", "No ERNeuRadDigiPar in runtime");
        Int_t nofFiberInRow = Int_t(TMath::Sqrt(fDigiPar->NofFibers()));
        Int_t nofBundlesInRow = Int_t(TMath::Sqrt(fDigiPar->NofBundles()));
        Float_t fiberWidth = fDigiPar->FiberWidth();
        Float_t bundleWidth = nofFiberInRow*fiberWidth;
        Float_t neuradWidth = bundleWidth*nofBundlesInRow;
        //?????????????? ??????
        for(Int_t iBundle = 0; iBundle < nofBundlesInRow; iBundle++ ){
                for(Int_t jBundle = 0; jBundle < nofBundlesInRow; jBundle++ ){
                        Float_t bundleX = neuradWidth/2. - iBundle*bundleWidth - bundleWidth/2.;
                        Float_t bundleY = neuradWidth/2. - jBundle*bundleWidth - bundleWidth/2.;
                        fBundles.push_back(new ERNeuRadBundle(bundleX,bundleY));
                        std::vector<ERNeuRadFiber*> fibersInBundle;
                        //?????????????? ??????? ? ??????
                        for (Int_t iFiber = 0; iFiber < nofFiberInRow; iFiber++){
                                for (Int_t jFiber = 0; jFiber < nofFiberInRow; jFiber++){
                                        Float_t fiberX = bundleX + bundleWidth/2. - iFiber*fiberWidth - fiberWidth/2.;
                                        Float_t fiberY = bundleY + bundleWidth/2. - jFiber*fiberWidth - fiberWidth/2.;
                                        fibersInBundle.push_back(new ERNeuRadFiber(fiberX,fiberY));
                                }
                        }
                        fFibers.push_back(fibersInBundle);
                }
        }
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
    //std::cout << "ERNeuRadSetup::ERNeuRadSetup() "<< std::endl;
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

Int_t   ERNeuRadSetup::NofBundles() {
        return fDigiPar->NofBundles();
}

Float_t ERNeuRadSetup::FiberLength() {
        return fDigiPar->FiberLength();
}

Float_t ERNeuRadSetup::FiberWidth() {
        return fDigiPar->FiberWidth();
}

Float_t ERNeuRadSetup::BundleX(Int_t iBundle){
        return fBundles[iBundle]->fX;
}

Float_t ERNeuRadSetup::BundleY(Int_t iBundle){
        return fBundles[iBundle]->fY;
}

Float_t ERNeuRadSetup::FiberX(Int_t iBundle, Int_t iFiber){
        return fFibers[iBundle][iFiber]->fX;
}

Float_t ERNeuRadSetup::FiberY(Int_t iBundle, Int_t iFiber){
        return fFibers[iBundle][iFiber]->fY;
}

Float_t ERNeuRadSetup::PMTQuantumEfficiency(Int_t iBundle, Int_t iFiber){
        return fDigiPar->PMTQuantumEfficiency(iFiber);
}

Float_t ERNeuRadSetup::PMTGain(Int_t iBundle, Int_t iFiber){
        return fDigiPar->PMTGain(iFiber);
}

void ERNeuRadSetup::Print(){
        fDigiPar->print();
}

ClassImp(ERNeuRadSetup)
