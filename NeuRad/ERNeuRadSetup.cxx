#include "ERNeuRadSetup.h"

#include "TError.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

ERNeuRadSetup* ERNeuRadSetup::fInstance = NULL;
ERNeuRadDigiPar* ERNeuRadSetup::fDigiPar = NULL;
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
	fDigiPar->print();
	Int_t nofFiberInRow = Int_t(TMath::Sqrt(fDigiPar->NofFibers()));
	Int_t nofBundlesInRow = Int_t(TMath::Sqrt(fDigiPar->NofBundles()));
	Float_t bundleWidth = nofFiberInRow*fDigiPar->FiberWidth();
	Float_t neuradWidth = bundleWidth*nofBundlesInRow;
	
	//Инициализируем бандлы
	for(Int_t iBundle = 0; iBundle < nofBundlesInRow; iBundle++ ){
		for(Int_t jBundle = 0; jBundle < nofBundlesInRow; jBundle++ ){
			Float_t bundleX = neuradWidth/2. - iBundle*bundleWidth - bundleWidth/2.;
			Float_t bundleY = neuradWidth/2. - jBundle*bundleWidth - bundleWidth/2.;
			fBundles.push_back(new ERNeuRadBundle(bundleX,bundleY));
			std::vector<ERNeuRadFiber*> fibersInBundle;
			//Инициализируем файберы в бандле
			for (Int_t iFiber = 0; iFiber < nofFiberInRow; iFiber++){
				for (Int_t jFiber = 0; jFiber < nofFiberInRow; jFiber++){
					Float_t fiberX = bundleX + bundleWidth/2. - iFiber*bundleWidth - bundleWidth/2.;
					Float_t fiberY = bundleY + bundleWidth/2. - jFiber*bundleWidth - bundleWidth/2.;
					fibersInBundle.push_back(new ERNeuRadFiber(fiberX,fiberY));
				}
			}
			fFibers.push_back(fibersInBundle);
		}
	}
	LOG(INFO) << "ERNeuRadSetup::ERNeuRadSetup() "<< FairLogger::endl;
    LOG(INFO) << "ERNeuRadSetup initialized! "<< FairLogger::endl;
}

ERNeuRadSetup* ERNeuRadSetup::Instance(){
	if (fInstance == NULL)
		return new ERNeuRadSetup();
	else
		return fInstance;
}

Int_t  ERNeuRadSetup::NofFibers() {
	fDigiPar->NofFibers();
}

Int_t   ERNeuRadSetup::NofBundles() {
	fDigiPar->NofBundles();
}

Float_t ERNeuRadSetup::FiberLength() {
	fDigiPar->FiberLength();
}

Float_t ERNeuRadSetup::FiberWidth() {
	fDigiPar->FiberWidth();
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

ClassImp(ERNeuRadSetup)