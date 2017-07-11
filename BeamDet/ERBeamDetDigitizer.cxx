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

// ----------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer()
  : FairTask("ER beamdet digitization"), 
  fBeamDetTOFPoints(NULL), 
  fBeamDetMWPCPoints(NULL), 
  fBeamDetTOFDigi(NULL), 
  fBeamDetMWPCDigi(NULL),
  fElossDispersionTof(0),
  fTimeDispersionTof(0),
  fTOFElossThreshold(0),
  fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer(Int_t verbose)
  : FairTask("ER beamdet digitization ", verbose),
  fBeamDetTOFPoints(NULL), 
  fBeamDetMWPCPoints(NULL), 
  fBeamDetTOFDigi(NULL), 
  fBeamDetMWPCDigi(NULL),
  fElossDispersionTof(0),
  fTimeDispersionTof(0),
  fTOFElossThreshold(0),
  fDigiEloss(0)
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
 /* Int_t iEvent =
      FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
      std::cout << "Event " << iEvent << std::endl;
  // Reset entries in output arrays*/
  Reset();

  for (Int_t iPoint = 0; iPoint < fBeamDetTOFPoints->GetEntriesFast(); iPoint++){
    ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)fBeamDetTOFPoints->At(iPoint);

    Float_t edep = gRandom->Gaus(point->GetEnergyLoss(), fElossDispersionTof);
    if (edep < fTOFElossThreshold)
      continue;

    Int_t tofNb = point->GetTofNb();
    Float_t time = gRandom->Gaus(point->GetTime(), fTimeDispersionTof);

    AddTOFDigi(edep, time, tofNb);
  }

  for (Int_t iPoint = 0; iPoint < fBeamDetMWPCPoints->GetEntriesFast(); iPoint++){
    ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)fBeamDetMWPCPoints->At(iPoint);
    
    Float_t edep = gRandom->Gaus(point->GetEnergyLoss(), fElossDispersionTof);
    if (edep < fTOFElossThreshold)
      continue;
    Int_t mwpcNb = point->GetMWPCNb();
    Int_t planeNb = point->GetPlaneNb();
    Int_t wireNb  = point->GetWireNb();
    Float_t time = gRandom->Gaus(point->GetTime(), fTimeDispersionTof);
    AddMWPCDigi(edep, time, mwpcNb, planeNb, wireNb);
  }
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
ERBeamDetMWPCDigi* ERBeamDetDigitizer::AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb)
{
  ERBeamDetMWPCDigi *digi = new((*fBeamDetMWPCDigi)[fBeamDetMWPCDigi->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigi->GetEntriesFast(), edep, time, mwpcNb, planeNb, wireNb);
  return digi;
}

ERBeamDetTOFDigi* ERBeamDetDigitizer::AddTOFDigi(Float_t edep, Double_t time, Int_t tofNb)
{
  ERBeamDetTOFDigi *digi = new((*fBeamDetTOFDigi)[fBeamDetTOFDigi->GetEntriesFast()])
              ERBeamDetTOFDigi(fBeamDetTOFDigi->GetEntriesFast(), edep, time, tofNb);
  return digi;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetDigitizer)
