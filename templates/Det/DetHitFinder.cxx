
#include "DetHitFinder.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>
using namespace std;
#include "DetPoint.h"

// ----------------------------------------------------------------------------
DetHitFinder::DetHitFinder()
  : FairTask("Det hit producing scheme")
{
  fHits = new TClonesArray("DetHit",1000);
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
DetHitFinder::DetHitFinder(Int_t verbose)
  : FairTask("Det hit producing scheme ", verbose)
{
  fHits = new TClonesArray("DetHit",1000);
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
DetHitFinder::~DetHitFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void DetHitFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus DetHitFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fPoints = (TClonesArray*) ioman->GetObject("DetPoints");
  if (!fPoints)
    Fatal("Init", "Can`t find collection DetPoints!"); 

  // Register output array fHits
  ioman->Register("DetHits", "Det", fHits, kTRUE);

   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void DetHitFinder::Exec(Option_t* opt)
{
  Reset();

  for (Int_t iPoint = 0; iPoint < fPoints->GetEntriesFast(); iPoint++){
    DetPoint* point = (DetPoint*)fPoints->At(iPoint);
    Float_t time = point->GetTime();
    Float_t eloss = point->GetEnergyLoss();
    TVector3 pos = TVector3(point->GetXIn(), point->GetYIn(), point->GetZIn());
    TVector3 dpos = TVector3(0.01, 0.01, 0.01); //ошибка пока фиксирована
    AddHit(0, pos, dpos,iPoint,eloss, time);
  }

  std::cout << "Hits count: " << fHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void DetHitFinder::Reset()
{
  if (fHits) {
    fHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void DetHitFinder::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
DetHit* DetHitFinder::AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time)
{
  DetHit *hit = new((*fHits)[fHits->GetEntriesFast()])
              DetHit(fHits->GetEntriesFast(),detID, pos, dpos, point_index, eloss, time);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(DetHitFinder)
