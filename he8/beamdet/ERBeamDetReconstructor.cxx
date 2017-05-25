#include "ERBeamDetReconstructor.h"

#include<iostream>
using namespace std;

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERBeamDetReconstructor::ERBeamDetReconstructor()
  : FairTask("Convert cal event to reco event"),
  fInEvent(NULL),
  fInCalEvent(NULL),
  fOutEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetReconstructor::ERBeamDetReconstructor(Int_t verbose)
  : FairTask("Convert cal event to reco event", verbose),
  fInEvent(NULL),
  fInCalEvent(NULL),
  fOutEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetReconstructor::~ERBeamDetReconstructor()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetReconstructor::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetReconstructor::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fInEvent = (ERBeamDetEvent*)ioman->GetObject("BeamDetEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fInCalEvent = (ERBeamDetCalEvent*)ioman->GetObject("BeamDetCalEvent.");
  if (!fInEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERBeamDetRecoEvent();
  ioman->Register("BeamDetRecoEvent.","Reco",fOutEvent, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetReconstructor::Exec(Option_t* opt)
{
  //std::cout << std::endl;
  //std::cout << "####### ERBeamDetReconstructor EVENT " << fEvent++ << " #####" << std::endl;
  Reset();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetReconstructor::Reset()
{
  fOutEvent->Reset();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetReconstructor::Finish()
{   
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetReconstructor)
