#include "ERNeuRadHitProducerMF.h"

#include <vector>
#include <map>

#include "TVector3.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERNeuRadDigi.h"
#include "ERNeuRadSetup.h"

using namespace std;

Int_t ERNeuRadHitProducerMF::fEvent = 0;
// ----------------------------------------------------------------------------
ERNeuRadHitProducerMF::ERNeuRadHitProducerMF()
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
ERNeuRadHitProducerMF::ERNeuRadHitProducerMF(Int_t verbose)
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
ERNeuRadHitProducerMF::~ERNeuRadHitProducerMF()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducerMF::SetParContainers()
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
InitStatus ERNeuRadHitProducerMF::Init()
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
void ERNeuRadHitProducerMF::Exec(Option_t* opt)
{
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "####### EVENT " << fEvent++ << " #####" << FairLogger::endl;
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "ERNeuRadHitProducerMF: "<< FairLogger::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;

  Int_t hitNumber=0;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  //Суммируем сигналы по модулям
  Float_t* SumBundleSignals = new Float_t[setup->NofBundles()];
  for (Int_t iBundle=0; iBundle < setup->NofBundles(); iBundle++)
    SumBundleSignals[iBundle] =0.;
  for (Int_t iDigi=0; iDigi <  fNeuRadDigis->GetEntriesFast(); iDigi++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
    SumBundleSignals[digi->BundleIndex()]+=digi->QDC();
  }
  
  Int_t nofBundles = 0;
  for (Int_t iBundle=0; iBundle < setup->NofBundles(); iBundle++){
    if (SumBundleSignals[iBundle] > fModuleThreshold*fOnePEInteg)
      nofBundles++;
  }

  if (nofBundles == 1){
    for (Int_t iDigi=0; iDigi <  fNeuRadDigis->GetEntriesFast(); iDigi++){
      ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
      if (digi->Side()==0 && digi->QDC() > fPixelThreshold*fOnePEInteg && SumBundleSignals[digi->BundleIndex()] > fModuleThreshold*fOnePEInteg){
         TVector3 pos(setup->FiberX(digi->BundleIndex(), digi->FiberIndex()),
                   setup->FiberY(digi->BundleIndex(), digi->FiberIndex()),
                   setup->Z()-setup->FiberLength());
          TVector3 dpos(0,0,0);
          AddHit(kNEURAD,pos, dpos,digi->BundleIndex(),digi->FiberIndex(),digi->FrontTDC());
      }
    }
  }
  delete [] SumBundleSignals;
  LOG(INFO) << "Hits count: " << fNeuRadHits->GetEntriesFast() << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducerMF::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitProducerMF::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHit* ERNeuRadHitProducerMF::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  BundleIndex, Int_t FiberIndex, Float_t time)
{
  ERNeuRadHit *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHit(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, BundleIndex, FiberIndex, time);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitProducerMF)
