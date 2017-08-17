#include "ERRTelescopeDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>
#include <algorithm>
using namespace std;

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::ERRTelescopeDigitizer()
  : FairTask("ER rtelescope digitization"),
  fSiPoints(NULL),
  fRTelescopeDigi(NULL),
  fElossSigma(0),
  fTimeSigma(0),
  fElossThreshold(0),
  fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::ERRTelescopeDigitizer(Int_t verbose)
  : FairTask("ER rtelescope digitization ", verbose),
  fSiPoints(NULL),
  fRTelescopeDigi(NULL),
  fElossSigma(0),
  fTimeSigma(0),
  fElossThreshold(0),
  fDigiEloss(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeDigitizer::~ERRTelescopeDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRTelescopeDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  fSiPoints = (TClonesArray*) ioman->GetObject("RTelescopeSiPoint");

  if (!fSiPoints)
    Fatal("Init", "Can`t find collection RTelescopeSiPoint!");


  // Register output array fRTelescopeHits
  fRTelescopeDigi = new TClonesArray("ERRTelescopeSiDigi",1000);

  ioman->Register("RTelescopeDigi", "RTelescope Digi", fRTelescopeDigi, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
  fRTelescopeSetup->Print();*/

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRTelescopeDigitizer::Exec(Option_t* opt)
{

  Reset();
  //Sort the points by sensors and sectors
  map<Int_t, map<Int_t, vector<Int_t>>> points;
  for (Int_t iPoint = 0; iPoint < fSiPoints->GetEntriesFast(); iPoint++){
    ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)fSiPoints->At(iPoint);
    points[point->GetSectorNb()][point->GetSensorNb()].push_back(iPoint);
  }

  map<Int_t, map<Int_t, vector<Int_t> > >::iterator  itSector;
  map<Int_t, vector<Int_t> >::iterator               itSensor;
  vector<Int_t>::iterator                            itPoint;

  for(itSector = points.begin(); itSector != points.end(); ++itSector){
    for(itSensor = itSector->second.begin(); itSensor != itSector->second.end(); ++itSensor) {
      Float_t edep = 0.; //sum edep in sensor
      Float_t time = numeric_limits<float>::max(); // min time in plate

      for (itPoint = itSensor->second.begin(); itPoint != itSensor->second.end(); ++itPoint){
        ERRTelescopeSiPoint* point = (ERRTelescopeSiPoint*)(fSiPoints->At(*itPoint));
        edep += point->GetEnergyLoss();

        if (point->GetTime() < time){
          time = point->GetTime();
        }
      }
      edep = gRandom->Gaus(edep, fElossSigma);

      if (edep < fElossThreshold)
        continue;

      time = gRandom->Gaus(time, fTimeSigma);

      ERRTelescopeSiDigi *digi = AddDigi(edep, time, itSector->first, itSensor->first);

      for (itPoint = itSensor->second.begin(); itPoint != itSensor->second.end(); ++itPoint){
        digi->AddLink(FairLink("RTelescopeSiPoint", *itPoint));
      }
    }
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Reset()
{
  if (fRTelescopeDigi) {
    fRTelescopeDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Finish()
{

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeSiDigi* ERRTelescopeDigitizer::AddDigi(Float_t edep, Double_t time, Int_t sectorNb, Int_t sensorNb)
{
  ERRTelescopeSiDigi *digi = new((*fRTelescopeDigi)[fRTelescopeDigi->GetEntriesFast()])
              ERRTelescopeSiDigi(fRTelescopeDigi->GetEntriesFast(), edep, time, sectorNb, sensorNb);
  return digi;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeDigitizer)
