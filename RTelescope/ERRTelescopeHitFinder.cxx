#include "ERRTelescopeHitFinder.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>
using namespace std;

#include "ERDetectorList.h"
#include "ERRTelescopePoint.h"


Int_t ERRTelescopeHitFinder::fEvent = 0;
// ----------------------------------------------------------------------------
ERRTelescopeHitFinder::ERRTelescopeHitFinder()
  : FairTask("ER muSi hit producing scheme")
,fDSRDPoints(NULL)
,fDSRDHits(NULL)
,fElossDispersion(0)
,fTimeDispersionPar(0)
,fElossThreshold(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeHitFinder::ERRTelescopeHitFinder(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
,fDSRDPoints(NULL)
,fDSRDHits(NULL)
,fElossDispersion(0)
,fTimeDispersionPar(0)
,fElossThreshold(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeHitFinder::~ERRTelescopeHitFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeHitFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeHitFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fDSRDPoints = (TClonesArray*) ioman->GetObject("DSRDPoint");
  if (!fDSRDPoints)
    Fatal("Init", "Can`t find collection DSRDPoint!"); 

  // Register output array fDSRDHits
  fDSRDHits = new TClonesArray("ERRTelescopeHit",1000);

  ioman->Register("DSRDHit", "DSRD hits", fDSRDHits, kTRUE);

  fDSRDSetup = ERRTelescopeSetup::Instance();
  fDSRDSetup->Print();
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeHitFinder::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERRTelescopeHitFinder: "<< std::endl;
  Reset();
  //Параметры геометрии. todo - убрать отсюда.
  Float_t Rmin = fDSRDSetup->Rmin(); //cm
  Float_t Rmax = fDSRDSetup->Rmax(); //cm
  Float_t dR  = (Rmax-Rmin)/fDSRDSetup->SensorNb();
  Float_t dPhi = 360./fDSRDSetup->SectorNb();
  Float_t z = fDSRDSetup->Z();

  for (Int_t iPoint = 0; iPoint < fDSRDPoints->GetEntriesFast(); iPoint++){
    ERRTelescopePoint* point = (ERRTelescopePoint*)fDSRDPoints->At(iPoint);
    Float_t eloss = gRandom->Gaus(point->GetEnergyLoss(), fElossDispersion);
    if (eloss < fElossThreshold)
      continue;
    TVector3 dpos = TVector3(0.01, 0.01, 0.01); //ошибка пока фиксирована
    Float_t r = (Rmin+point->Sensor()*dR + Rmin+(point->Sensor()+1)*dR)/2.;
    Float_t phi = ((0.+point->Sector()*dPhi) + (0. + (point->Sector()+1)*dPhi))/2.;
    Float_t x = r*TMath::Cos(phi*TMath::DegToRad());
    Float_t y = r*TMath::Sin(phi*TMath::DegToRad());
    TVector3 pos = TVector3(x, y, z);
    Float_t time = gRandom->Gaus(point->GetTime(), TMath::Sqrt(fTimeDispersionPar/point->GetEnergyLoss()));
    AddHit(kDSRD, pos, dpos,iPoint,eloss, time);
  }

  std::cout << "Hits count: " << fDSRDHits->GetEntriesFast() << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeHitFinder::Reset()
{
  if (fDSRDHits) {
    fDSRDHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeHitFinder::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeHit* ERRTelescopeHitFinder::AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time)
{
  ERRTelescopeHit *hit = new((*fDSRDHits)[fDSRDHits->GetEntriesFast()])
              ERRTelescopeHit(fDSRDHits->GetEntriesFast(),detID, pos, dpos, point_index, eloss, time);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeHitFinder)
