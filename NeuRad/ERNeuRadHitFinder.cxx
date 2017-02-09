#include "ERNeuRadHitFinder.h"

#include <vector>
#include <map>

#include "TVector3.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>

#include "ERDetectorList.h"
#include "ERNeuRadPMTSignal.h"
#include "ERNeuRadSetup.h"

using namespace std;

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
  ERNeuRadPMTSignal* targetHitSignal = NULL;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  for (Int_t iSignal=0; iSignal <  fNeuRadPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)fNeuRadPMTSignals->At(iSignal);
    cerr << "0" << endl;
    if (signal->Side()==0){
      //Ищем пару с другой стороны
      cerr << "1" << endl;
      Bool_t founded=kFALSE;
      Int_t jBackSignal = -1;
      for (Int_t jSignal=0; jSignal<fNeuRadPMTSignals->GetEntriesFast(); jSignal++){
        ERNeuRadPMTSignal* signalBack = (ERNeuRadPMTSignal*)fNeuRadPMTSignals->At(jSignal);
        if (signalBack->Side() == 1 && signalBack->ModuleIndex()==signal->ModuleIndex() && signalBack->FiberIndex()==signal->FiberIndex()){
          founded=kTRUE;
          jBackSignal = jSignal;
          break;
        }
      }

      if (founded){
        ERNeuRadPMTSignal* signalBack = (ERNeuRadPMTSignal*)fNeuRadPMTSignals->At(jBackSignal);
        cerr << signal->FullInteg() << " " << fPixelThreshold*signal->OnePEIntegral()<< " " <<signalBack->FullInteg()<< endl;
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
    TVector3 pos(setup->FiberX(targetHitSignal->ModuleIndex(), targetHitSignal->FiberIndex()),
                 setup->FiberY(targetHitSignal->ModuleIndex(), targetHitSignal->FiberIndex()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,targetHitSignal->ModuleIndex(),targetHitSignal->FiberIndex(),targetHitSignal->FullInteg());
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
                                           Int_t  ModuleIndex, Int_t FiberIndex, Float_t time)
{
  ERNeuRadHit *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHit(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, ModuleIndex, FiberIndex, time);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitFinder)
