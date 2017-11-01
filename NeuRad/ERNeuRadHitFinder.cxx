#include "ERNeuRadHitFinder.h"

#include <iostream>
#include <vector>
#include <map>

#include "TVector3.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERDetectorList.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

Int_t ERNeuRadHitFinder::fEvent = 0;

// ----------------------------------------------------------------------------
ERNeuRadHitFinder::ERNeuRadHitFinder()
  : FairTask("ER muSi hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinder::ERNeuRadHitFinder(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinder::~ERNeuRadHitFinder()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  fDigiPar = (ERNeuRadDigiPar*)
             (rtdb->getContainer("ERNeuRadDigiPar"));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
InitStatus ERNeuRadHitFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPMTSignals = (TClonesArray*) ioman->GetObject("NeuRadPMTSignal");
  //todo check

  // Register output array fmuSiHits
  fNeuRadHits = new TClonesArray("ERNeuRadHit",1000);

  ioman->Register("NeuRadHit", "NeuRad hits", fNeuRadHits, kTRUE);

  ERNeuRadSetup* NeuRadSetup = ERNeuRadSetup::Instance();
  NeuRadSetup->Print();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadHitFinder::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNeuRadHitFinder: "<< std::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;
  Int_t hitNumber=0;
  ERNeuRadPixelSignal* targetHitSignal = NULL;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  for (Int_t iSignal=0; iSignal <  fNeuRadPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(iSignal);
    std::cerr << "0" << std::endl;
    if (signal->Side()==0){
      //Ищем пару с другой стороны
      std::cerr << "1" << std::endl;
      Bool_t founded=kFALSE;
      Int_t jBackSignal = -1;
      for (Int_t jSignal=0; jSignal<fNeuRadPMTSignals->GetEntriesFast(); jSignal++){
        ERNeuRadPixelSignal* signalBack = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(jSignal);
        if (signalBack->Side() == 1 && signalBack->ModuleNb()==signal->ModuleNb() && signalBack->PixelNb()==signal->PixelNb()){
          founded=kTRUE;
          jBackSignal = jSignal;
          break;
        }
      }

      if (founded){
        ERNeuRadPixelSignal* signalBack = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(jBackSignal);
        std::cerr << signal->FullInteg() << " " << fPixelThreshold*signal->OnePEIntegral()<< " " <<signalBack->FullInteg()<< std::endl;
        if (signal->FullInteg() > fPixelThreshold*signal->OnePEIntegral() && signalBack->FullInteg() > fPixelThreshold*signal->OnePEIntegral()){
          targetHitSignal = signal;
          hitNumber++;
        }
      }
      if (hitNumber > 1)
        break;
    }
  }
  if (hitNumber == 1){
    TVector3 pos(setup->FiberX(targetHitSignal->ModuleNb(), targetHitSignal->PixelNb()),
                 setup->FiberY(targetHitSignal->ModuleNb(), targetHitSignal->PixelNb()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,targetHitSignal->ModuleNb(),targetHitSignal->PixelNb(),targetHitSignal->FullInteg());
  }
  std::cout << "Hits count: " << fNeuRadHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinder::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitFinder::Finish()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHit* ERNeuRadHitFinder::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  ModuleNb, Int_t PixelNb, Float_t time)
{
  ERNeuRadHit *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHit(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, ModuleNb, PixelNb, time);
  return hit;
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitFinder)
