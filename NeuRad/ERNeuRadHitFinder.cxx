/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadHitFinder.h"

#include "TVector3.h"
//#include "TGeoMatrix.h"

#include "FairLogger.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERDetectorList.h" // required for kNEURAD
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
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "####### EVENT " << fEvent++ << " #####" << FairLogger::endl;
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "ERNeuRadHitFinder: "<< FairLogger::endl;

  Reset();

  Float_t fOnePEInteg = 4.8; //TODO magic number?
  Int_t hitNumber = 0;
  ERNeuRadPixelSignal* targetHitSignal = NULL;

  // Get the pointer to the singleton setup class object
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  for (Int_t iSignal=0; iSignal <  fNeuRadPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(iSignal);
    LOG(DEBUG) << "0" << FairLogger::endl;
    if (signal->GetSide() == 0) {
      //Ищем пару с другой стороны
      LOG(DEBUG) << "1" << FairLogger::endl;
      Bool_t flagFound = kFALSE;
      Int_t jBackSignal = -1;
      for (Int_t jSignal=0; jSignal<fNeuRadPMTSignals->GetEntriesFast(); jSignal++){
        ERNeuRadPixelSignal* signalBack = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(jSignal);
        if (signalBack->GetSide() == 1 &&
            signalBack->GetModuleNb() == signal->GetModuleNb() &&
            signalBack->GetPixelNb() == signal->GetPixelNb())
        {
          flagFound = kTRUE;
          jBackSignal = jSignal;
          break;
        }
      }

      if (flagFound) {
        ERNeuRadPixelSignal* signalBack = (ERNeuRadPixelSignal*)fNeuRadPMTSignals->At(jBackSignal);
        std::cerr << signal->FullInteg() << " " << fPixelThreshold*signal->OnePEIntegral()<< " " <<signalBack->FullInteg()<< FairLogger::endl;
        if (signal->FullInteg() > fPixelThreshold*signal->OnePEIntegral() &&
            signalBack->FullInteg() > fPixelThreshold*signal->OnePEIntegral())
        {
          targetHitSignal = signal;
          hitNumber++;
        }
      }
      if (hitNumber > 1)
        break;
    }
  }
  if (hitNumber == 1) {
    TVector3 pos(setup->GetFiberX(targetHitSignal->GetModuleNb(), targetHitSignal->GetPixelNb()),
                 setup->GetFiberY(targetHitSignal->GetModuleNb(), targetHitSignal->GetPixelNb()),
                 setup->GetZ()-setup->GetFiberLength());
    TVector3 dpos(0,0,0);
    AddHit(kNEURAD, pos, dpos, targetHitSignal->GetModuleNb(), targetHitSignal->GetPixelNb(), targetHitSignal->FullInteg());
  }
  std::cout << "Hits count: " << fNeuRadHits->GetEntriesFast() << FairLogger::endl;
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
