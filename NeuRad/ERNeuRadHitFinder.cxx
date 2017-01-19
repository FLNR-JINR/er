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
#include "ERNeuRadDigi.h"
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
void ERNeuRadHitFinder::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNeuRadHitFinder: "<< std::endl;
  Reset();
  Float_t fOnePEInteg = 4.8;
  /*
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  map<Int_t, Float_t> startTimes;
  map<Int_t,Int_t> startDigis;
  for(Int_t iDigi= 0; iDigi < fNeuRadDigis->GetEntriesFast(); iDigi++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fNeuRadDigis->At(iDigi);
    Float_t frontTDC = digi->FrontTDC();
    Int_t key = digi->ModuleIndex()*setup->NofFibers() + digi->FiberIndex();
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
    TVector3 pos(setup->FiberX(digi->ModuleIndex(), digi->FiberIndex()),
                 setup->FiberY(digi->ModuleIndex(), digi->FiberIndex()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,digi->ModuleIndex(),digi->FiberIndex(),digi->FrontTDC());
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
        if (digiBack->Side() == 1 && digiBack->ModuleIndex()==digi->ModuleIndex() && digiBack->FiberIndex()==digi->FiberIndex()){
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
    TVector3 pos(setup->FiberX(targetHitDigi->ModuleIndex(), targetHitDigi->FiberIndex()),
                 setup->FiberY(targetHitDigi->ModuleIndex(), targetHitDigi->FiberIndex()),
                 setup->Z()-setup->FiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD,pos, dpos,targetHitDigi->ModuleIndex(),targetHitDigi->FiberIndex(),targetHitDigi->FrontTDC());
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
