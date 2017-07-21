/*#include "ERBeamDetTrackFinder.h"

#include <vector>
#include <iostream>
using namespace std;

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>

#include "ERDetectorList.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetPoint.h"

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder()
  : FairTask("ER BeamDet track finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder(Int_t verbose)
  : FairTask("ER BeamDet track finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::~ERBeamDetTrackFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetTrackFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetTrackFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetHits = (TClonesArray*) ioman->GetObject("BeamDetHit");
  //todo check

  // Register output array fBeamDetHits
  fBeamDetTracks = new TClonesArray("ERBeamDetTrack",1000);

  ioman->Register("BeamDetTrack.", "BeamDet track", fBeamDetTracks, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt)
{ 
  Reset();
  std::cout << std::endl;
  std::cout << "ERBeamDetTrackFinder:" << std::endl;
  //Раскидываем хиты по станциям

  std::vector<ERBeamDetHit*> HitsByStation[3];

  for (Int_t iHit = 0; iHit < fBeamDetHits->GetEntriesFast(); iHit++){

    ERBeamDetHit* hit = (ERBeamDetHit*) fBeamDetHits->At(iHit);
    HitsByStation[hit->Station()].push_back(hit);
  }

  for (std::vector<ERBeamDetHit*>::iterator it0 = HitsByStation[0].begin(); it0 != HitsByStation[0].end(); ++it0 ){
    for(std::vector<ERBeamDetHit*>::iterator it1 = HitsByStation[1].begin(); it1 != HitsByStation[1].end(); ++it1){

      TVector3 singlet1((*it1)->GetX()-(*it0)->GetX(),(*it1)->GetY()-(*it0)->GetY(),(*it1)->GetZ()-(*it0)->GetZ());

      for(std::vector<ERBeamDetHit*>::iterator it2 = HitsByStation[2].begin(); it2 != HitsByStation[2].end(); ++it2){

        TVector3 singlet2((*it2)->GetX()-(*it1)->GetX(),(*it2)->GetY()-(*it1)->GetY(),(*it2)->GetZ()-(*it1)->GetZ());

        Double_t angle = singlet2.Angle(singlet1);
        if (angle < fAngleCut){
          ERBeamDetTrack* track = new((*fBeamDetTracks)[fBeamDetTracks->GetEntriesFast()])ERBeamDetTrack();
          track->AddHit(0,*(*it0));
          track->AddHit(1,*(*it1));
          track->AddHit(2,*(*it2));
        }
      }
    }
  }
  std::cout << "Tracks count:" << fBeamDetTracks->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Reset()
{
  if (fBeamDetTracks) {
    fBeamDetTracks->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Finish()
{   
  
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERBeamDetTrackFinder)
*/