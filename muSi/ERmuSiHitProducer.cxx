#include "ERmuSiHitProducer.h"

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiPoint.h"
// ----------------------------------------------------------------------------
ERmuSiHitProducer::ERmuSiHitProducer()
  : FairTask("ER muSi hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHitProducer::ERmuSiHitProducer(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHitProducer::~ERmuSiHitProducer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiHitProducer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiHitProducer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fmuSiPoints = (TClonesArray*) ioman->GetObject("muSiPoint");
  //todo check

  // Register output array fmuSiHits
  fmuSiHits = new TClonesArray("ERmuSiHit",1000);

  ioman->Register("muSiHit", "muSi hits", fmuSiHits, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiHitProducer::Exec(Option_t* opt)
{
  //Пока что просто перегоняем поинты в хиты
  for (Int_t iPoint = 0; iPoint < fmuSiPoints->GetEntriesFast(); iPoint++){
    ERmuSiPoint* point = (ERmuSiPoint*)fmuSiPoints->At(iPoint);
    TVector3 pos = TVector3((point->GetXIn()+point->GetXOut())/2.,
                            (point->GetYIn()+point->GetYOut())/2.,
                            (point->GetZIn()+point->GetZOut())/2.);
    TVector3 dpos = TVector3(0.01, 0.01, 0.01);
    ERmuSiHit* hit = AddHit(kMUSI, pos, dpos,iPoint, point->Station());
    hit->AddLink(FairLink("muSiPoint",iPoint));
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiHitProducer::Reset()
{
  if (fmuSiHits) {
    fmuSiHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiHitProducer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHit* ERmuSiHitProducer::AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station)
{
  ERmuSiHit *hit = new((*fmuSiHits)[fmuSiHits->GetEntriesFast()])
              ERmuSiHit(detID, pos, dpos, index, station);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERmuSiHitProducer)
