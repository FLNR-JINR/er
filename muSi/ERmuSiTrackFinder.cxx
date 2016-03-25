#include "ERmuSiTrackFinder.h"

#include <vector>

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiPoint.h"

// ----------------------------------------------------------------------------
ERmuSiTrackFinder::ERmuSiTrackFinder()
  : FairTask("ER muSi track finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiTrackFinder::ERmuSiTrackFinder(Int_t verbose)
  : FairTask("ER muSi track finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiTrackFinder::~ERmuSiTrackFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiTrackFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiTrackFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fmuSiHits = (TClonesArray*) ioman->GetObject("muSiHit");
  //todo check

  // Register output array fmuSiHits
  fmuSiTracks = new TClonesArray("ERmuSiTrack",1000);

  ioman->Register("muSiTrack", "muSi tracks", fmuSiTracks, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiTrackFinder::Exec(Option_t* opt)
{ 
  //Раскидываем хиты по станциям

  std::vector<ERmuSiHit*> HitsByStation[3];

  for (Int_t iHit = 0; iHit < fmuSiHits->GetEntriesFast(); iHit++){

    ERmuSiHit* hit = (ERmuSiHit*) fmuSiHits->At(iHit);
    HitsByStation[hit->Station()].push_back(hit);
  }

  
  //Синглеты
  std::vector<ERmuSiSinglet> singlets[2];
  //Генерируем синглеты с началом на 1-й станции
  for (std::vector<ERmuSiHit*>::iterator it0 = HitsByStation[0].begin(); it0 != HitsByStation[0].end(); ++it0 ){
    for(std::vector<ERmuSiHit*>::iterator it1 = HitsByStation[1].begin(); it1 != HitsByStation[1].end(); ++it1){
      ERmuSiSinglet singlet = {(*it0), (*it1)};
      singlets[0].push_back(singlet);
    }
  }

  //Генерируем синглеты с началом на 2-й станции
  for (std::vector<ERmuSiHit*>::iterator it0 = HitsByStation[1].begin(); it0 != HitsByStation[1].end(); ++it0 ){
    for(std::vector<ERmuSiHit*>::iterator it1 = HitsByStation[2].begin(); it1 != HitsByStation[2].end(); ++it1){
      ERmuSiSinglet singlet = {(*it0), (*it1)};
      singlets[1].push_back(singlet);
    }
  }
  
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiTrackFinder::Reset()
{
  if (fmuSiHits) {
    fmuSiHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiTrackFinder::Finish()
{   

}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERmuSiTrackFinder)
