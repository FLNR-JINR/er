#include "ERLi10Reconstructor.h"

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
#include "ERNDHit.h"
#include "ERDSRDHit.h"
#include "ERLi10EventHeader.h"

// ----------------------------------------------------------------------------
ERLi10Reconstructor::ERLi10Reconstructor()
  : FairTask("ER Li10 reconstruction scheme")
,fDSRDHits(NULL)
,fNDHits(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLi10Reconstructor::ERLi10Reconstructor(Int_t verbose)
  : FairTask("ER Li10 reconstruction scheme", verbose)
,fDSRDHits(NULL)
,fNDHits(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLi10Reconstructor::~ERLi10Reconstructor()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLi10Reconstructor::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERLi10Reconstructor::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNDHits = (TClonesArray*) ioman->GetObject("NDHit");
  if (!fNDHits)
    Fatal("Init", "Can`t find collection NDHit!"); 

  fDSRDHits = (TClonesArray*) ioman->GetObject("DSRDHit");
  if (!fDSRDHits)
    Fatal("Init", "Can`t find collection NDHit!"); 

  // Register output array fNDHits
  //fNDHits = new TClonesArray("ERNDHit",1000);

  //ioman->Register("NDHit", "ND hits", fNDHits, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERLi10Reconstructor::Exec(Option_t* opt)
{
  std::cout << "ERLi10Reconstructor: "<< std::endl;
  std::cout << "DSRD Hits: "<< std::endl;
  for (Int_t iHit = 0; iHit < fDSRDHits->GetEntriesFast(); iHit++){
    ERDSRDHit* hit = (ERDSRDHit*)fDSRDHits->At(iHit);
    std::cout << iHit << " Eloss:" << hit->Eloss() << " time:" << hit->Time() << std::endl; 
  }
  std::cout << "ND Hits: "<< std::endl;
  for (Int_t iHit = 0; iHit < fNDHits->GetEntriesFast(); iHit++){
    ERNDHit* hit = (ERNDHit*)fNDHits->At(iHit);
    std::cout << iHit << " Eloss:" << hit->LightYield() << " time:" << hit->Time() << " NeutronProb:"<< hit->NeutronProb() <<  std::endl; 
  }

  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) 
    Fatal("SetParContainers", "No analysis run");
  ERLi10EventHeader* header = (ERLi10EventHeader*)run->GetEventHeader();
  header->SetNEnergy(0);
  header->SetPEnergy(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERLi10Reconstructor::Reset()
{
  if (fNDHits) {
    fNDHits->Delete();
  }
  if (fDSRDHits) {
    fDSRDHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLi10Reconstructor::Finish()
{   

}
// ----------------------------------------------------------------------------
/*
// ----------------------------------------------------------------------------
ERNDHit* ERLi10Reconstructor::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                       Int_t point_index, Float_t eloss, Float_t time,Float_t neutronProb)
{
  ERNDHit *hit = new((*fNDHits)[fNDHits->GetEntriesFast()])
              ERNDHit(fNDHits->GetEntriesFast(),detID, pos, dpos, point_index, eloss, time, neutronProb);
  return hit;
}
// ----------------------------------------------------------------------------
*/
//-----------------------------------------------------------------------------
ClassImp(ERLi10Reconstructor)
