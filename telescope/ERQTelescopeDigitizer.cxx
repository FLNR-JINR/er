#include "ERQTelescopeDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLink.h"

#include <iostream>
#include <map>
#include <vector>
#include <limits>
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

  //Sort the points by stations and strips
  map<Int_t, map<Int_t, vector<Int_t> > > points;
  for (Int_t iPoint = 0; iPoint < fQTelescopeSiPoints->GetEntriesFast(); iPoint++){
    ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fQTelescopeSiPoints->At(iPoint);
    points[point->GetStationNb()][point->GetStripNb()].push_back(iPoint);
  }

  map<Int_t, map<Int_t, vector<Int_t> > >::iterator itStation;
  map<Int_t, vector<Int_t> >::iterator itStrip;
  vector<Int_t>::iterator itPoint;

  for (itStation = points.begin(); itStation != points.end(); ++itStation){
    for (itStrip = itStation->second.begin(); itStrip != itStation->second.end(); ++itStrip){
      
      Float_t edep = 0.; //sum edep in strip
      Float_t time = numeric_limits<float>::max(); // min time in strip
      
      for (itPoint = itStrip->second.begin(); itPoint != itStrip->second.end(); ++itPoint){
        ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fQTelescopeSiPoints->At(*itPoint);
        edep += point->GetEnergyLoss();
        if (point->GetTime() < time){
          time = point->GetTime();
        }
      }

      edep = gRandom->Gaus(edep, fSiElossDispersion);
      if (edep < fSiElossThreshold)
        continue;

      time = gRandom->Gaus(time, fSiTimeDispersion);

      ERQTelescopeSiDigi* digi = AddSiDigi(edep, time, itStation->first, itStrip->first);

      for (itPoint = itStrip->second.begin(); itPoint != itStrip->second.end(); ++itPoint){
        digi->AddLink(FairLink("ERQTelescopeSiPoint",*itPoint));
      }
    }
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
