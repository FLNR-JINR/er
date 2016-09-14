#include "ERNeuRadHitProducerWBT.h"

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

Int_t ERNeuRadHitProducerWBT::fEvent = 0;
// ----------------------------------------------------------------------------
ERNeuRadHitProducerWBT::ERNeuRadHitProducerWBT()
  : FairTask("ER NeuRad hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitProducerWBT::ERNeuRadHitProducerWBT(Int_t verbose)
  : FairTask("ER NeuRad hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitProducerWBT::~ERNeuRadHitProducerWBT()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducerWBT::SetParContainers()
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
InitStatus ERNeuRadHitProducerWBT::Init()
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
void ERNeuRadHitProducerWBT::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNeuRadHitProducerWBT: "<< std::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;
  Int_t hitNumber=0;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  for (Int_t iSignal=0; iSignal <  fNeuRadPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)fNeuRadPMTSignals->At(iSignal);
    if (signal->Side() == 0){
      Float_t qInteg = signal->GetInteg(signal->GetStartTime(), signal->GetFinishTime());
      TVector3 pos(setup->FiberX(signal->ModuleIndex(), signal->FiberIndex()),
                   setup->FiberY(signal->ModuleIndex(), signal->FiberIndex()),
                   setup->Z()-setup->FiberLength());
      TVector3 dpos(0,0,0);
      AddHit(kNEURAD,pos, dpos,signal->ModuleIndex(),signal->FiberIndex(), -1, qInteg);
    }
  }
  std::cout << "Hits count: " << fNeuRadHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducerWBT::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitProducerWBT::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitWBT* ERNeuRadHitProducerWBT::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  BundleIndex, Int_t FiberIndex, Float_t time,
                                           Float_t qInteg)
{
  ERNeuRadHitWBT *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHitWBT(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, BundleIndex, FiberIndex, time, 
                          qInteg);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitProducerWBT)
