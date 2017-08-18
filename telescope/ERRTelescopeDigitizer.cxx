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
  fRTelescopeSiDigi(NULL),
  fCsIPoints(NULL),
  fRTelescopeCsIDigi(NULL),
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
  fCsIPoints(NULL),
  fRTelescopeSiDigi(NULL),
  fRTelescopeCsIDigi(NULL),
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

  fCsIPoints = (TClonesArray*) ioman->GetObject("RTelescopeCsIPoint");

  if (!fCsIPoints)
    Fatal("Init", "Can`t find collection RTelescopeCsiPoint!");

  // Register output array fRTelescopeHits
  fRTelescopeSiDigi = new TClonesArray("ERRTelescopeSiDigi",2 * 1000);

  ioman->Register("RTelescopeSiDigi", "RTelescope Si Digi", fRTelescopeSiDigi, kTRUE);

  fRTelescopeCsIDigi = new TClonesArray("ERRTelescopeCsIDigi",1000);

  ioman->Register("RTelescopeCsIDigi", "RTelescope CsI Digi", fRTelescopeCsIDigi, kTRUE);


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
  map<Int_t, vector<Int_t> >::iterator               itRing;
  vector<Int_t>::iterator                            itPoint;
// !!!!


  for(itSector = points.begin(); itSector != points.end(); ++itSector){
    for(itRing = itSector->second.begin(); itRing != itSector->second.end(); ++itRing) {
      Float_t edep = 0.; //sum edep in ring
      Float_t time = numeric_limits<float>::max(); // min time in plate

      for (itPoint = itRing->second.begin(); itPoint != itRing->second.end(); ++itPoint){
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

      ERRTelescopeSiDigi *si_digi = AddSiDigi(edep, time, itSector->first, itRing->first);


      for (itPoint = itRing->second.begin(); itPoint != itRing->second.end(); ++itPoint){
        si_digi->AddLink(FairLink("", *itPoint));
      }
    }
  }
// CSI CSI CSI
map<Int_t, vector<Int_t>> CsIpoints;
for (Int_t iPoint = 0; iPoint < fCsIPoints->GetEntriesFast(); iPoint++){
  ERRTelescopeCsIPoint* CsIpoint = (ERRTelescopeCsIPoint*)fCsIPoints->At(iPoint);
  CsIpoints[CsIpoint->GetCrystallNb()].push_back(iPoint);
}

map<Int_t, vector<Int_t> >::iterator               itCrystall;


for(itCrystall = CsIpoints.begin(); itCrystall != CsIpoints.end(); ++itCrystall) {
  Float_t edep = 0.; //sum edep in crystall
  Float_t time = numeric_limits<float>::max(); // min time in plate

  for (itPoint = itCrystall->second.begin(); itPoint != itCrystall->second.end(); ++itPoint)
  {
    ERRTelescopeCsIPoint* CsIpoint = (ERRTelescopeCsIPoint*)(fCsIPoints->At(*itPoint));
    edep += CsIpoint->GetEnergyLoss();
  }


  Float_t LC = 1.;
  Float_t a = 0.07;
  Float_t b = 0.02;

  Float_t disp = a*a*edep + b*b*edep*edep;
  edep = gRandom->Gaus(edep*LC, disp);


  ERRTelescopeCsIDigi *csi_digi = AddCsIDigi(edep, 2000 , itCrystall->first);   // !!
  // fLaBrElossInEvent += edep;  // !!


  // edep = gRandom->Gaus(edep, fElossSigma);

  if (edep < fElossThreshold)
    continue;


    for (itPoint = itCrystall->second.begin(); itPoint != itCrystall->second.end(); ++itPoint){
      csi_digi->AddLink(FairLink("RTelescopeCsIPoint", *itPoint));
    }

  // time = gRandom->Gaus(time, fTimeSigma);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Reset()
{
  if (fRTelescopeSiDigi) {
    fRTelescopeSiDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRTelescopeDigitizer::Finish()
{

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRTelescopeSiDigi* ERRTelescopeDigitizer::AddSiDigi(Float_t edep, Double_t time, Int_t sectorNb, Int_t sensorNb)
{
  ERRTelescopeSiDigi *si_digi = new((*fRTelescopeSiDigi)[fRTelescopeSiDigi->GetEntriesFast()])
        ERRTelescopeSiDigi(fRTelescopeSiDigi->GetEntriesFast(), edep, time, 0, sensorNb, 0);  // Side = 0 => ring



  si_digi = new((*fRTelescopeSiDigi)[fRTelescopeSiDigi->GetEntriesFast()])
              ERRTelescopeSiDigi(fRTelescopeSiDigi->GetEntriesFast(), edep, time, sectorNb, 0, 1);   // Side = 1 => sector


  return si_digi;
}
// ----------------------------------------------------------------------------
ERRTelescopeCsIDigi* ERRTelescopeDigitizer::AddCsIDigi(Float_t edep, Double_t time, Int_t crystall)
{
  ERRTelescopeCsIDigi *csi_digi = new((*fRTelescopeCsIDigi)[fRTelescopeCsIDigi->GetEntriesFast()])
        ERRTelescopeCsIDigi(fRTelescopeCsIDigi->GetEntriesFast(), edep,time, crystall);
      return csi_digi;
}
//-----------------------------------------------------------------------------
ClassImp(ERRTelescopeDigitizer)
