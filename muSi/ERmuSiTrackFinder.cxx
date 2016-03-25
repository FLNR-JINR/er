#include "ERmuSiTrackFinder.h"

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
  
  fmuSiHits = (TClonesArray*) ioman->GetObject("ERmuSiHit");
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
  //Генерируем синглеты
  //for (Int_t iHit = 0; iHit < f)
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
