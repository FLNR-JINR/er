#include "ERBeamDetDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>
using namespace std;

#include "ERDetectorList.h"


Int_t ERBeamDetDigitizer::fEvent = 0;
// ----------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer()
  : FairTask("ER muSi hit producing scheme")
,fRTelescopePoints(NULL)
,fRTelescopeHits(NULL)
,fElossDispersion(0)
,fTimeDispersionPar(0)
,fElossThreshold(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
,fRTelescopePoints(NULL)
,fRTelescopeHits(NULL)
,fElossDispersion(0)
,fTimeDispersionPar(0)
,fElossThreshold(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetDigitizer::~ERBeamDetDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetTOFPoints = (TClonesArray*) ioman->GetObject("BeamDetTOFPoint");
  fBeamDetMWPCPoints = (TClonesArray*) ioman->GetObject("BeamDetMWPCPoint");

  if (!fBeamDetTOFPoints)
    Fatal("Init", "Can`t find collection BeamDetTOFPoint!"); 

  if (!fBeamDetMWPCPoints)
    Fatal("Init", "Can`t find collection BeamDetMWPCPoint!"); 

  // Register output array fRTelescopeHits
  fBeamDetTOFDigi = new TClonesArray("ERBeamDetTOFDigi",1000);
  fBeamDetMWPCDigi = new TClonesArray("ERBeamDetMWPCDigi",1000);

  ioman->Register("BeamDetTOFDigi", "BeamDetTOF Digi", fBeamDetTOFDigi, kTRUE);
  ioman->Register("BeamDetMWPCDigi", "BeamDetMWPC Digi", fBeamDetMWPCDigi, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
  fRTelescopeSetup->Print();*/
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetDigitizer::Exec(Option_t* opt)
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetDigitizer::Reset()
{
  if (fBeamDetMWPCDigi) {
    fBeamDetMWPCDigi->Delete();
  }
  if (fBeamDetTOFDigi) {
    fBeamDetTOFDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetDigitizer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetMWPCDigi* ERBeamDetDigitizer::AddMWPCDigi()
{
}

ERBeamDetTOFDigi* ERBeamDetDigitizer::AddTOFDigi()
{
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetDigitizer)
