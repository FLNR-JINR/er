#include "ERNeuRadHitProducer.h"

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

Int_t ERNeuRadHitProducer::fEvent = 0;
// ----------------------------------------------------------------------------
ERNeuRadHitProducer::ERNeuRadHitProducer()
  : FairTask("ER muSi hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitProducer::ERNeuRadHitProducer(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHitProducer::~ERNeuRadHitProducer()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducer::SetParContainers()
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
InitStatus ERNeuRadHitProducer::Init()
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
void ERNeuRadHitProducer::Exec(Option_t* opt)
{
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "####### EVENT " << fEvent++ << " #####" << FairLogger::endl;
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "ERNeuRadHitProducer: "<< FairLogger::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;
  /*
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  map<Int_t, Float_t> startTimes;
  map<Int_t,Int_t> startDigis;
  for(Int_t iDigi= 0; iDigi < fNeuRadDigis->GetEntriesFast(); iDigi++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
    Float_t frontTDC = digi->FrontTDC();
    Int_t key = digi->BundleIndex()*setup->NofFibers() + digi->FiberIndex();
    if (startTimes.find(key) == startTimes.end()){
      startTimes[key] = frontTDC;
      startDigis[key] = iDigi;
    }
    else if(frontTDC < startTimes[key]){
      startTimes[key] = frontTDC;
      startDigis[key] = iDigi;
    }
  }

  for(map<Int_t, Int_t>::iterator it = startDigis.begin(); it != startDigis.end(); ++it){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(it->second);
    TVector3 pos(setup->FiberX(digi->BundleIndex(), digi->FiberIndex()),
                 setup->FiberY(digi->BundleIndex(), digi->FiberIndex()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,digi->BundleIndex(),digi->FiberIndex(),digi->FrontTDC());
  }
  */
  Int_t hitNumber=0;
  ERNeuRadDigi* targetHitDigi = NULL;
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  for (Int_t iDigi=0; iDigi <  fNeuRadDigis->GetEntriesFast(); iDigi++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
    if (digi->Side()==0){
      //Ищем пару с другой стороны
      Bool_t founded=kFALSE;
      Int_t jBackDigi = -1;
      for (Int_t jDigi=0; jDigi<fNeuRadDigis->GetEntriesFast(); jDigi++){
        ERNeuRadDigi* digiBack = (ERNeuRadDigi*)fNeuRadDigis->At(jDigi);
        if (digiBack->Side() == 1 && digiBack->BundleIndex()==digi->BundleIndex() && digiBack->FiberIndex()==digi->FiberIndex()){
          founded=kTRUE;
          jBackDigi = jDigi;
          break;
        }
      }
      if (founded){
        ERNeuRadDigi* digiBack = (ERNeuRadDigi*)fNeuRadDigis->At(jBackDigi);
        if (digi->QDC() > fPixelThreshold*fOnePEInteg && digiBack->QDC() > fPixelThreshold*fOnePEInteg){
          targetHitDigi = digi;
          hitNumber++;
        }
      }
      if (hitNumber > 1)
        break;
    }
  }
  if (hitNumber == 1){
    TVector3 pos(setup->FiberX(targetHitDigi->BundleIndex(), targetHitDigi->FiberIndex()),
                 setup->FiberY(targetHitDigi->BundleIndex(), targetHitDigi->FiberIndex()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,targetHitDigi->BundleIndex(),targetHitDigi->FiberIndex(),targetHitDigi->FrontTDC());
  }
  LOG(INFO) << "Hits count: " << fNeuRadHits->GetEntriesFast() << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadHitProducer::Reset()
{
  if (fNeuRadHits) {
    fNeuRadHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadHitProducer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadHit* ERNeuRadHitProducer::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                                           Int_t  BundleIndex, Int_t FiberIndex, Float_t time)
{
  ERNeuRadHit *hit = new((*fNeuRadHits)[fNeuRadHits->GetEntriesFast()])
              ERNeuRadHit(fNeuRadHits->GetEntriesFast(),detID, pos, dpos,-1, BundleIndex, FiberIndex, time);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadHitProducer)
