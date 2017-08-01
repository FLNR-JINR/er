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
  fBeamDetTOFDigi1(NULL), 
  fBeamDetTOFDigi2(NULL), 
  fBeamDetMWPCDigiX1(NULL),
  fBeamDetMWPCDigiX2(NULL),
  fBeamDetMWPCDigiY1(NULL),
  fBeamDetMWPCDigiY2(NULL),
  fElossSigmaTOF(0),
  fTimeSigmaTOF(0),
  fElossSigmaMWPC(0),
  fTimeSigmaMWPC(0),
  fMWPCElossThreshold(0),
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
  fBeamDetTOFDigi1(NULL), 
  fBeamDetTOFDigi2(NULL), 
  fBeamDetMWPCDigiX1(NULL),
  fBeamDetMWPCDigiX2(NULL),
  fBeamDetMWPCDigiY1(NULL),
  fBeamDetMWPCDigiY2(NULL),
  fElossSigmaTOF(0),
  fTimeSigmaTOF(0),
  fElossSigmaMWPC(0),
  fTimeSigmaMWPC(0),
  fMWPCElossThreshold(0),
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
  FairRun* run = FairRun::Instance();
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
  fBeamDetTOFDigi1 = new TClonesArray("ERBeamDetTOFDigi",1000);
  fBeamDetTOFDigi2 = new TClonesArray("ERBeamDetTOFDigi",1000);

  fBeamDetMWPCDigiX1 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiX2 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiY1 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiY2 = new TClonesArray("ERBeamDetMWPCDigi",1000);


  ioman->Register("BeamDetTOFDigi1", "BeamDetTOF Digi", fBeamDetTOFDigi1, kTRUE);
  ioman->Register("BeamDetTOFDigi2", "BeamDetTOF Digi", fBeamDetTOFDigi2, kTRUE);

  ioman->Register("BeamDetMWPCDigiX1", "BeamDetMWPC x1 Digi", fBeamDetMWPCDigiX1, kTRUE);
  ioman->Register("BeamDetMWPCDigiX2", "BeamDetMWPC x2 Digi", fBeamDetMWPCDigiX2, kTRUE);
  ioman->Register("BeamDetMWPCDigiY1", "BeamDetMWPC y1 Digi", fBeamDetMWPCDigiY1, kTRUE);
  ioman->Register("BeamDetMWPCDigiY2", "BeamDetMWPC y2 Digi", fBeamDetMWPCDigiY2, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
  fRTelescopeSetup->Print();*/
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetDigitizer::Exec(Option_t* opt)
{

  Reset();

  map<Int_t, vector<Int_t>> pointsTOF;
  for (Int_t iPoint = 0; iPoint < fBeamDetTOFPoints->GetEntriesFast(); iPoint++){
    ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)fBeamDetTOFPoints->At(iPoint);
    pointsTOF[point->GetTOFNb()].push_back(iPoint);
  }

  map<Int_t, vector<Int_t> >::iterator  itPlate;
  vector<Int_t>::iterator               itTOFPoint;

  for(itPlate = pointsTOF.begin(); itPlate != pointsTOF.end(); ++itPlate){
  
    Float_t edep = 0.; //sum edep in plate
    Float_t time = numeric_limits<float>::max(); // min time in plate
    
    for (itTOFPoint = itPlate->second.begin(); itTOFPoint != itPlate->second.end(); ++itTOFPoint){
      ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)(fBeamDetTOFPoints->At(*itTOFPoint));
      edep += point->GetEnergyLoss();

      if (point->GetTime() < time){
        time = point->GetTime();
      }
    }

    fElossSigmaTOF = edep * 0.1 / TMath::Sqrt(edep);
    edep = gRandom->Gaus(edep, fElossSigmaTOF);

    if (edep < fTOFElossThreshold)
      continue;

    time = gRandom->Gaus(time, fTimeSigmaTOF);

    ERBeamDetTOFDigi *digi = AddTOFDigi(edep, time, itPlate->first);

    for (itTOFPoint = itPlate->second.begin(); itTOFPoint != itPlate->second.end(); ++itTOFPoint){
      digi->AddLink(FairLink("BeamDetTOFPoint", *itTOFPoint));
    }
  }
   //Sort the MWPCpoints by MWPC, planes and wires
  map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>> pointsMWPC;
  for (Int_t iPoint = 0; iPoint < fBeamDetMWPCPoints->GetEntriesFast(); iPoint++){
    ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)fBeamDetMWPCPoints->At(iPoint);
    pointsMWPC[point->GetMWPCNb()][point->GetPlaneNb()][point->GetWireNb()].push_back(iPoint);
  }

  map<Int_t, map<Int_t, map<Int_t, vector<Int_t>>>>::iterator itMWPC;
  map<Int_t, map<Int_t, vector<Int_t> > >::iterator           itPlane;
  map<Int_t, vector<Int_t> >::iterator                        itWire;
  vector<Int_t>::iterator                                     itMWPCPoint;

  for (itMWPC = pointsMWPC.begin(); itMWPC != pointsMWPC.end(); ++itMWPC){
    for (itPlane = itMWPC->second.begin(); itPlane != itMWPC->second.end(); ++itPlane){
      for(itWire = itPlane->second.begin(); itWire != itPlane->second.end(); ++itWire){
      
        Float_t edep = 0.; //sum edep in wire
        Float_t time = numeric_limits<float>::max(); // min time in wire
        
        for (itMWPCPoint = itWire->second.begin(); itMWPCPoint != itWire->second.end(); ++itMWPCPoint){
          ERBeamDetMWPCPoint* point = (ERBeamDetMWPCPoint*)(fBeamDetMWPCPoints->At(*itMWPCPoint));
          edep += point->GetEnergyLoss();
          if (point->GetTime() < time){
            time = point->GetTime();
          }
        }

        edep = gRandom->Gaus(edep, fElossSigmaMWPC);
        if (edep < fMWPCElossThreshold)
          continue;

        time = gRandom->Gaus(time, fTimeSigmaMWPC);

        ERBeamDetMWPCDigi *digi = AddMWPCDigi(edep, time, itMWPC->first, itPlane->first, itWire->first);

        for (itMWPCPoint = itWire->second.begin(); itMWPCPoint != itWire->second.end(); ++itMWPCPoint){
          digi->AddLink(FairLink("BeamDetMWPCPoint", *itMWPCPoint));
        }
      }
    }
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetDigitizer::Reset()
{
  if (fBeamDetMWPCDigiX1) {
    fBeamDetMWPCDigiX1->Delete();
  }
  if (fBeamDetMWPCDigiX2) {
    fBeamDetMWPCDigiX2->Delete();
  }
  if (fBeamDetMWPCDigiY1) {
    fBeamDetMWPCDigiY1->Delete();
  }
  if (fBeamDetMWPCDigiY2) {
    fBeamDetMWPCDigiY2->Delete();
  }
  if (fBeamDetTOFDigi1) {
    fBeamDetTOFDigi1->Delete();
  }
  if (fBeamDetTOFDigi2) {
    fBeamDetTOFDigi2->Delete();
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
  ERBeamDetMWPCDigi *digi;
  if(mwpcNb == 1) {
    if(planeNb == 1) {
      digi = new((*fBeamDetMWPCDigiX1)[fBeamDetMWPCDigiX1->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiX1->GetEntriesFast(), edep, time, mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fBeamDetMWPCDigiY1)[fBeamDetMWPCDigiY1->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiY1->GetEntriesFast(), edep, time, mwpcNb, planeNb, wireNb);
    }
  }
  if(mwpcNb == 2) {
    if(planeNb == 1) {
      digi = new((*fBeamDetMWPCDigiX2)[fBeamDetMWPCDigiX2->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiX2->GetEntriesFast(), edep, time, mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fBeamDetMWPCDigiY2)[fBeamDetMWPCDigiY2->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiY2->GetEntriesFast(), edep, time, mwpcNb, planeNb, wireNb);
    }
  }
  return digi;
}

ERBeamDetTOFDigi* ERBeamDetDigitizer::AddTOFDigi(Float_t edep, Double_t time, Int_t tofNb)
{
  ERBeamDetTOFDigi *digi; 
  if(tofNb == 1) {
    digi = new((*fBeamDetTOFDigi1)[fBeamDetTOFDigi1->GetEntriesFast()])
                ERBeamDetTOFDigi(fBeamDetTOFDigi1->GetEntriesFast(), edep, time, tofNb);
  }
  if(tofNb == 2) {
    digi = new((*fBeamDetTOFDigi2)[fBeamDetTOFDigi2->GetEntriesFast()])
                ERBeamDetTOFDigi(fBeamDetTOFDigi2->GetEntriesFast(), edep, time, tofNb);
  }
  return digi;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetDigitizer)
