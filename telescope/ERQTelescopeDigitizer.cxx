#include "ERQTelescopeDigitizer.h"

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
ERQTelescopeDigitizer::ERQTelescopeDigitizer()
  : FairTask("ER beamdet digitization"), 
  fQTelescopeSiPoints(NULL), 
  fQTelescopeSiDigi(NULL), 
  fSiElossDispersion(0),
  fSiTimeDispersion(0),
  fSiElossThreshold(0),
  fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERQTelescopeDigitizer::ERQTelescopeDigitizer(Int_t verbose)
  : FairTask("ER beamdet digitization ", verbose),
  fQTelescopeSiPoints(NULL), 
  fQTelescopeSiDigi(NULL), 
  fSiElossDispersion(0),
  fSiTimeDispersion(0),
  fSiElossThreshold(0),
  fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERQTelescopeDigitizer::~ERQTelescopeDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERQTelescopeDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERQTelescopeDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fQTelescopeSiPoints = (TClonesArray*) ioman->GetObject("QTelescopeSiPoint");

  if (!fQTelescopeSiPoints)
    Fatal("Init", "Can`t find collection QTelescopeSiPoint!"); 

  // Register output array fRTelescopeHits
  fQTelescopeSiDigi = new TClonesArray("ERQTelescopeSiDigi",1000);

  ioman->Register("QTelescopeSiDigi", "QTelescope Si Digi", fQTelescopeSiDigi, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERQTelescopeDigitizer::Exec(Option_t* opt)
{
  Reset();

  for (Int_t iPoint = 0; iPoint < fQTelescopeSiPoints->GetEntriesFast(); iPoint++){
    ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fQTelescopeSiPoints->At(iPoint);

    Float_t edep = gRandom->Gaus(point->GetEnergyLoss(), fSiElossDispersion);
    if (edep < fSiElossThreshold)
      continue;

    Int_t stationNb = point->GetStationNb();
    Int_t stripNb = point->GetStripNb();
    Float_t time = gRandom->Gaus(point->GetTime(), fSiTimeDispersion);

    AddSiDigi(edep, time, stationNb, stripNb);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERQTelescopeDigitizer::Reset()
{
  if (fQTelescopeSiDigi) {
    fQTelescopeSiDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERQTelescopeDigitizer::Finish()
{   

}
// ----------------------------------------------------------------------------
ERQTelescopeSiDigi* ERQTelescopeDigitizer::AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, Int_t stripNb)
{
  ERQTelescopeSiDigi *digi = new((*fQTelescopeSiDigi)[fQTelescopeSiDigi->GetEntriesFast()])
              ERQTelescopeSiDigi(fQTelescopeSiDigi->GetEntriesFast(), edep, time, stationNb, stripNb);
  return digi;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERQTelescopeDigitizer)
