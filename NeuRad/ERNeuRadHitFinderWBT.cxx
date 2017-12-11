#include "ERNeuRadHitFinderWBT.h"

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

Int_t ERNeuRadHitFinderWBT::fEvent = 0;

// ----------------------------------------------------------------------------
ERNeuRadHitFinderWBT::ERNeuRadHitFinderWBT()
  : FairTask("ER NeuRad hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinderWBT::ERNeuRadHitFinderWBT(Int_t verbose)
  : FairTask("ER NeuRad hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinderWBT::~ERNeuRadHitFinderWBT()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinderWBT::SetParContainers()
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
InitStatus ERNeuRadHitFinderWBT::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPMTSignals = (TClonesArray*) ioman->GetObject("NeuRadPMTSignal");
  //todo check

  // Register output array fmuSiHits
  fNeuRadHits = new TClonesArray("ERNeuRadHitWBT",1000);

  ioman->Register("NeuRadHit", "NeuRad hits", fNeuRadHits, kTRUE);

  // Get singleton ERNeuRadSetup object
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  setup->Print();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadHitFinderWBT::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNeuRadHitFinderWBT: "<< std::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;
  Int_t hitNumber=0;

  // Get singleton ERNeuRadSetup object
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  for (Int_t iSignal=0; iSignal <  fNeuRadPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(iSignal);
    if (signal->Side() == 0){
      //Float_t qInteg = signal->GetInteg(signal->GetStartTime(), signal->GetFinishTime());
      Float_t qInteg = signal->AmplitudesSum();
      TVector3 pos(setup->GetFiberX(signal->ModuleNb(), signal->PixelNb()),
                   setup->GetFiberY(signal->ModuleNb(), signal->PixelNb()),
                   setup->GetZ()-setup->GetFiberLength());
      TVector3 dpos(0,0,0);
      AddHit(kNEURAD,pos, dpos,signal->ModuleNb(),signal->PixelNb(), -1, qInteg);
    }
  }
  std::cout << "Hits count: " << fNeuRadHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinderWBT::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitFinderWBT::Finish()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitWBT* ERNeuRadHitFinderWBT::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  ModuleNb, Int_t PixelNb, Float_t time,
                                           Float_t qInteg)
{
  ERNeuRadHitWBT *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHitWBT(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, ModuleNb, PixelNb, time, 
                          qInteg);
  return hit;
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitFinderWBT)
