#include "ERLiCalibrator.h"

#include<iostream>
using namespace std;


#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERLiCalibrator::ERLiCalibrator()
  : FairTask("Convert Raw event to analyse event"),
  fRawEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLiCalibrator::ERLiCalibrator(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fRawEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLiCalibrator::~ERLiCalibrator()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLiCalibrator::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERLiCalibrator::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fRawEvent = new TLiEvent();
  fRawEvent = (TLiEvent*)ioman->GetObject("LiEvent.");
  if (!fRawEvent)
      Fatal("Init", "Can`t find branch in input file!");
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERLiCalibrator::Exec(Option_t* opt)
{
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << fRawEvent->beamTRIGGER << std::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERLiCalibrator::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLiCalibrator::Finish()
{   
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERLiCalibrator)
