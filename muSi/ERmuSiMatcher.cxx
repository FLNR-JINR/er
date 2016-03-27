#include "ERmuSiMatcher.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"

#include "ERmuSiPoint.h"
#include "ERmuSiHit.h"
#include "ERmuSiTrack.h"
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
 Int_t trueTracks = 0;
 Int_t wrongTracks = 0;
 for(Int_t iTrack = 0; iTrack < fmuSiTracks->GetEntriesFast(); iTrack++){
 	ERmuSiTrack* track = (ERmuSiTrack*)fmuSiTracks->At(iTrack);
    ERmuSiHit* hit0 = track->Hit(0);
    ERmuSiHit* hit1 = track->Hit(1);
    ERmuSiHit* hit2 = track->Hit(2);

    if (hit0->GetRefIndex() == -1 || hit1->GetRefIndex() == -1 || hit2->GetRefIndex() == -1){ //fakes
    	wrongTracks++;
    	continue;
    }
 	ERmuSiPoint* point0 = (ERmuSiPoint*)fmuSiPoints->At(hit0->GetRefIndex());
 	ERmuSiPoint* point1 = (ERmuSiPoint*)fmuSiPoints->At(hit1->GetRefIndex());
 	ERmuSiPoint* point2 = (ERmuSiPoint*)fmuSiPoints->At(hit2->GetRefIndex());
 	if ((point0->GetTrackID() == point1->GetTrackID()) && (point1->GetTrackID() == point2->GetTrackID()))
 		trueTracks++;
 	else
 		wrongTracks++;
 }
 LOG(INFO) << "=== True tracks:" << trueTracks << FairLogger::endl;
 LOG(INFO) << "=== Wrong tracks:" << wrongTracks << FairLogger::endl;
 LOG(INFO) << "=================== ERmuSiMatcher finish!========================" << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiMatcher::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiMatcher::Finish()
{   
	
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERmuSiMatcher)
