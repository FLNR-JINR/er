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
#include <algorithm>
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

   //Sort the points by MWPC, planes and wires
  map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>> points;
  for (Int_t iPoint = 0; iPoint < fBeamDetMWPCPoints->GetEntriesFast(); iPoint++){
    ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)fBeamDetMWPCPoints->At(iPoint);
    points[point->GetMWPCNb()][point->GetPlaneNb()][point->GetWireNb()].push_back(iPoint);
  }

  map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>>::iterator itMWPC;
  map<Int_t, map<Int_t, vector<Int_t> > >::iterator           itPlane;
  map<Int_t, vector<Int_t> >::iterator                        itWire;
  vector<Int_t>::iterator                                     itPoint;

  for (itMWPC = points.begin(); itMWPC != points.end(); ++itMWPC){
    for (itPlane = itMWPC->second.begin(); itPlane != itMWPC->second.end(); ++itPlane){
      for(itWire = itPlane->second.begin(); itWire != itPlane->second.end(); ++itWire){
      
        Float_t edep = 0.; //sum edep in wire
        Float_t time = numeric_limits<float>::max(); // min time in wire
        
        for (itPoint = itWire->second.begin(); itPoint != itWire->second.end(); ++itPoint){
          ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)(fBeamDetMWPCPoints->At(*itPoint));
          edep += point->GetEnergyLoss();
          if (point->GetTime() < time){
            time = point->GetTime();
          }
        }

        edep = gRandom->Gaus(edep, fElossDispersionMWPC);
        if (edep < fMWPCElossThreshold)
          continue;

        time = gRandom->Gaus(time, fTimeDispersionMWPC);

        ERBeamDetMWPCDigi *digi = AddMWPCDigi(edep, time, itMWPC->first, itPlane->first, itWire->first);

        for (itPoint = itWire->second.begin(); itPoint != itWire->second.end(); ++itPoint){
          digi->AddLink(FairLink("BeamDetMWPCPoint", *itPoint));
        }
      }
    }
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
