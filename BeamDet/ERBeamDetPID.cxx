#include "ERBeamDetPID.h"

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

// ----------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID()
  : FairTask("ER BeamDet particle finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetPID::ERBeamDetPID(Int_t verbose)
  : FairTask("ER BeamDet particle finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetPID::~ERBeamDetPID()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetPID::SetParContainers()
{
  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetPID::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetTOFDigi = (TClonesArray*) ioman->GetObject("BeamDetTOFDigi");

  // Register output array fBeamDetHits
  fBeamDetParticle = (ERBeamDetParticle*)new ERBeamDetParticle();

  ioman->Register("BeamDetParticle.", "BeamDet Particle", fBeamDetParticle, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetPID::Exec(Option_t* opt)
{ 
  Reset();
 
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetPID::Reset()
{
  if (fBeamDetParticle) {
    fBeamDetParticle->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetPID::Finish()
{   
  
}
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetPID)