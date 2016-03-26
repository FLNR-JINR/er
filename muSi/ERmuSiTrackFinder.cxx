#include "ERmuSiTrackFinder.h"

#include <vector>

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiTrack.h"
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
  LOG(INFO) << "===================ERmuSiTrackFinder started!========================" << FairLogger::endl;
  //Раскидываем хиты по станциям

  std::vector<ERmuSiHit*> HitsByStation[3];

  for (Int_t iHit = 0; iHit < fmuSiHits->GetEntriesFast(); iHit++){

    ERmuSiHit* hit = (ERmuSiHit*) fmuSiHits->At(iHit);
    HitsByStation[hit->Station()].push_back(hit);
  }

  for (std::vector<ERmuSiHit*>::iterator it0 = HitsByStation[0].begin(); it0 != HitsByStation[0].end(); ++it0 ){
    for(std::vector<ERmuSiHit*>::iterator it1 = HitsByStation[1].begin(); it1 != HitsByStation[1].end(); ++it1){
      TVector3 singlet1((*it1)->GetX()-(*it0)->GetX(),(*it1)->GetY()-(*it0)->GetY(),(*it1)->GetZ()-(*it0)->GetZ());
      for(std::vector<ERmuSiHit*>::iterator it2 = HitsByStation[2].begin(); it2 != HitsByStation[2].end(); ++it2){
        TVector3 singlet2((*it2)->GetX()-(*it1)->GetX(),(*it2)->GetY()-(*it1)->GetY(),(*it2)->GetZ()-(*it1)->GetZ());
        Double_t angle = singlet2.Angle(singlet1);
        //LOG(ERROR) << angle << FairLogger::endl;
        if (angle < fAngleCut){
          ERmuSiTrack* track = new((*fmuSiTracks)[fmuSiTracks->GetEntriesFast()])ERmuSiTrack();
          track->AddHit(0,*(*it0));
          track->AddHit(1,*(*it1));
          track->AddHit(2,*(*it2));
        }
      }
    }
  }
  LOG(INFO) << "==== " << fmuSiTracks->GetEntriesFast() << " tracks founded" << FairLogger::endl;
  LOG(INFO) << "=================== ERmuSiTrackFinder finish!========================" << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiTrackFinder::Reset()
{
  if (fmuSiHits) {
    fmuSiTracks->Delete();
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
