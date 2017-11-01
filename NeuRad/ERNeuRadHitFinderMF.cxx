#include "ERNeuRadHitFinderMF.h"

#include <iostream>
#include <vector>
#include <map>

#include "TVector3.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERDetectorList.h"
#include "ERNeuRadDigi.h"
#include "ERNeuRadSetup.h"

Int_t ERNeuRadHitFinderMF::fEvent = 0;

// ----------------------------------------------------------------------------
ERNeuRadHitFinderMF::ERNeuRadHitFinderMF()
  : fPixelThreshold(2.),
    fModuleThreshold(4.),
    fNeuRadDigis(NULL),
    fNeuRadHits(NULL),
    fDigiPar(NULL),
    FairTask("ER muSi hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinderMF::ERNeuRadHitFinderMF(Int_t verbose)
  : fPixelThreshold(2.),
    fModuleThreshold(4.),
    fNeuRadDigis(NULL),
    fNeuRadHits(NULL),
    fDigiPar(NULL),
    FairTask("ER muSi hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitFinderMF::~ERNeuRadHitFinderMF()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinderMF::SetParContainers()
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
InitStatus ERNeuRadHitFinderMF::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadDigis = (TClonesArray*) ioman->GetObject("NeuRadDigi");
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
void ERNeuRadHitFinderMF::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNeuRadHitFinderMF: "<< std::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;

  Int_t hitNumber=0;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  //Суммируем сигналы по модулям
  Float_t* SumModuleSignals = new Float_t[setup->NofModules()];
  for (Int_t iModule=0; iModule < setup->NofModules(); iModule++)
    SumModuleSignals[iModule] =0.;
  for (Int_t iDigi=0; iDigi <  fNeuRadDigis->GetEntriesFast(); iDigi++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
    SumModuleSignals[digi->ModuleIndex()]+=digi->QDC();
  }
  
  Int_t nofModules = 0;
  for (Int_t iModule=0; iModule < setup->NofModules(); iModule++){
    if (SumModuleSignals[iModule] > fModuleThreshold*fOnePEInteg)
      nofModules++;
  }

  //if (nofModules == 1){
    for (Int_t iDigi=0; iDigi <  fNeuRadDigis->GetEntriesFast(); iDigi++){
      ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
      if (digi->Side()==0 && digi->QDC() > fPixelThreshold*fOnePEInteg && SumModuleSignals[digi->ModuleIndex()] > fModuleThreshold*fOnePEInteg){
         TVector3 pos(setup->FiberX(digi->ModuleIndex(), digi->FiberIndex()),
                   setup->FiberY(digi->ModuleIndex(), digi->FiberIndex()),
                   setup->Z()-setup->FiberLength());
          TVector3 dpos(0,0,0);
          AddHit(kNEURAD,pos, dpos,digi->ModuleIndex(),digi->FiberIndex(),digi->FrontTDC());
      }
    }
  //}
  delete [] SumModuleSignals;
  std::cout << "Hits count: " << fNeuRadHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitFinderMF::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitFinderMF::Finish()
{   
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHit* ERNeuRadHitFinderMF::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  ModuleIndex, Int_t FiberIndex, Float_t time)
{
  ERNeuRadHit *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHit(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, ModuleIndex, FiberIndex, time);
  return hit;
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitFinderMF)
