#include "ERmuSiMatcher.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiPoint.h"
// ----------------------------------------------------------------------------
ERmuSiMatcher::ERmuSiMatcher()
  : FairTask("ER muSi matching scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiMatcher::ERmuSiMatcher(Int_t verbose)
  : FairTask("ER muSi matching scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiMatcher::~ERmuSiMatcher()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiMatcher::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiMatcher::Init()
{

  
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fmuSiPoints = (TClonesArray*) ioman->GetObject("muSiPoint");
  fmuSiHits = (TClonesArray*) ioman->GetObject("muSiHit");
  fmuSiTracks = (TClonesArray*) ioman->GetObject("muSiTrack");
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiMatcher::Exec(Option_t* opt)
{
 LOG(INFO) << "===================ERmuSiMatcher started!========================" << FairLogger::endl;
 LOG(INFO) << "=================== ERmuSiMatcher finish!========================" << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiMatcher::Reset()
{
  if (fmuSiHits) {
    fmuSiHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiMatcher::Finish()
{   
	
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERmuSiMatcher)
