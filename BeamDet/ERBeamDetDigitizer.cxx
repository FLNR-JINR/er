/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERBeamDetDigitizer.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer()
  : ERTask("ER beamdet digitization"), 
  fBeamDetToFPoints(NULL), 
  fBeamDetMWPCPoints(NULL), 
  fBeamDetToFDigi1(NULL), 
  fBeamDetToFDigi2(NULL), 
  fBeamDetMWPCDigiX1(NULL),
  fBeamDetMWPCDigiX2(NULL),
  fBeamDetMWPCDigiY1(NULL),
  fBeamDetMWPCDigiY2(NULL),
  fElossSigmaToF(0),
  fTimeSigmaToF(0),
  fElossSigmaMWPC(0),
  fTimeSigmaMWPC(0),
  fMWPCElossThreshold(0),
  fToFElossThreshold(0)
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetDigitizer::ERBeamDetDigitizer(Int_t verbose)
  : ERTask("ER beamdet digitization ", verbose),
  fBeamDetToFPoints(NULL), 
  fBeamDetMWPCPoints(NULL), 
  fBeamDetToFDigi1(NULL), 
  fBeamDetToFDigi2(NULL), 
  fBeamDetMWPCDigiX1(NULL),
  fBeamDetMWPCDigiX2(NULL),
  fBeamDetMWPCDigiY1(NULL),
  fBeamDetMWPCDigiY2(NULL),
  fElossSigmaToF(0),
  fTimeSigmaToF(0),
  fElossSigmaMWPC(0),
  fTimeSigmaMWPC(0),
  fMWPCElossThreshold(0),
  fToFElossThreshold(0),
  fElossSigmaOverElossToF(0),
  fSigmaEOverEToFIsSet(0)
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetDigitizer::~ERBeamDetDigitizer() {
}
//--------------------------------------------------------------------------------------------------
InitStatus ERBeamDetDigitizer::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetToFPoints = (TClonesArray*) ioman->GetObject("BeamDetToFPoint");
  fBeamDetMWPCPoints = (TClonesArray*) ioman->GetObject("BeamDetMWPCPoint");

  if (!fBeamDetToFPoints)
    Fatal("Init", "Can`t find collection BeamDetToFPoint!"); 

  if (!fBeamDetMWPCPoints)
    Fatal("Init", "Can`t find collection BeamDetMWPCPoint!"); 

  // Register output array fRTelescopeHits
  fBeamDetToFDigi1 = new TClonesArray("ERBeamDetTOFDigi",1000);
  fBeamDetToFDigi2 = new TClonesArray("ERBeamDetTOFDigi",1000);

  fBeamDetMWPCDigiX1 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiX2 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiY1 = new TClonesArray("ERBeamDetMWPCDigi",1000);
  fBeamDetMWPCDigiY2 = new TClonesArray("ERBeamDetMWPCDigi",1000);


  ioman->Register("BeamDetToFDigi1", "BeamDetToF Digi", fBeamDetToFDigi1, kTRUE);
  ioman->Register("BeamDetToFDigi2", "BeamDetToF Digi", fBeamDetToFDigi2, kTRUE);

  ioman->Register("BeamDetMWPCDigiX1", "BeamDetMWPC x1 Digi", fBeamDetMWPCDigiX1, kTRUE);
  ioman->Register("BeamDetMWPCDigiX2", "BeamDetMWPC x2 Digi", fBeamDetMWPCDigiX2, kTRUE);
  ioman->Register("BeamDetMWPCDigiY1", "BeamDetMWPC y1 Digi", fBeamDetMWPCDigiY1, kTRUE);
  ioman->Register("BeamDetMWPCDigiY2", "BeamDetMWPC y2 Digi", fBeamDetMWPCDigiY2, kTRUE);

  /*fRTelescopeSetup = ERRTelescopeSetup::Instance();
  fRTelescopeSetup->Print();*/
   
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetDigitizer::Exec(Option_t* opt)
{

  Reset();

  map<Int_t, vector<Int_t>> pointsToF;
  for (Int_t iPoint = 0; iPoint < fBeamDetToFPoints->GetEntriesFast(); iPoint++){
    ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)fBeamDetToFPoints->At(iPoint);
    pointsToF[point->GetToFNb()].push_back(iPoint);
  }

  map<Int_t, vector<Int_t> >::iterator  itPlate;
  vector<Int_t>::iterator               itToFPoint;

  for(itPlate = pointsToF.begin(); itPlate != pointsToF.end(); ++itPlate){
  
    Float_t edep = 0.; //sum edep in plate
    Float_t time = numeric_limits<float>::max(); // min time in plate
    
    for (itToFPoint = itPlate->second.begin(); itToFPoint != itPlate->second.end(); ++itToFPoint){
      ERBeamDetTOFPoint* point = (ERBeamDetTOFPoint*)(fBeamDetToFPoints->At(*itToFPoint));
      edep += point->GetEnergyLoss();

      if (point->GetTime() < time){
        time = point->GetTime();
      }
    }

    if(fSigmaEOverEToFIsSet) {
      fElossSigmaToF = edep * fElossSigmaOverElossToF / TMath::Sqrt(edep);
    }
    
    edep = gRandom->Gaus(edep, fElossSigmaToF);

    if (edep < fToFElossThreshold)
      continue;

    time = gRandom->Gaus(time, fTimeSigmaToF);

    ERBeamDetTOFDigi *digi = AddToFDigi(edep, time, itPlate->first);

    itToFPoint = itPlate->second.begin();
    for (; itToFPoint != itPlate->second.end(); ++itToFPoint){
      digi->AddLink(FairLink("BeamDetToFPoint", *itToFPoint));
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

        itMWPCPoint = itWire->second.begin();
        for (; itMWPCPoint != itWire->second.end(); ++itMWPCPoint){
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

        ERBeamDetMWPCDigi *digi = AddMWPCDigi(edep, time, 
                                              itMWPC->first, itPlane->first, itWire->first);
        itMWPCPoint = itWire->second.begin();
        for (; itMWPCPoint != itWire->second.end(); ++itMWPCPoint){
          digi->AddLink(FairLink("BeamDetMWPCPoint", *itMWPCPoint));
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
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
  if (fBeamDetToFDigi1) {
    fBeamDetToFDigi1->Delete();
  }
  if (fBeamDetToFDigi2) {
    fBeamDetToFDigi2->Delete();
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetDigitizer::Finish() {   

}
//--------------------------------------------------------------------------------------------------
ERBeamDetMWPCDigi* ERBeamDetDigitizer::AddMWPCDigi(Float_t edep, Double_t time, 
                                                   Int_t mwpcNb, Int_t planeNb, Int_t wireNb) {
  ERBeamDetMWPCDigi *digi;
  if(mwpcNb == 1) {
    if(planeNb == 1) {
      digi = new((*fBeamDetMWPCDigiX1)[fBeamDetMWPCDigiX1->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiX1->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fBeamDetMWPCDigiY1)[fBeamDetMWPCDigiY1->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiY1->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    }
  }
  if(mwpcNb == 2) {
    if(planeNb == 1) {
      digi = new((*fBeamDetMWPCDigiX2)[fBeamDetMWPCDigiX2->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiX2->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    } else {
      digi = new((*fBeamDetMWPCDigiY2)[fBeamDetMWPCDigiY2->GetEntriesFast()])
              ERBeamDetMWPCDigi(fBeamDetMWPCDigiY2->GetEntriesFast(), edep, time, 
                                mwpcNb, planeNb, wireNb);
    }
  }
  return digi;
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTOFDigi* ERBeamDetDigitizer::AddToFDigi(Float_t edep, Double_t time, Int_t tofNb) {
  ERBeamDetTOFDigi *digi; 
  if(tofNb == 1) {
    digi = new((*fBeamDetToFDigi1)[fBeamDetToFDigi1->GetEntriesFast()])
                ERBeamDetTOFDigi(fBeamDetToFDigi1->GetEntriesFast(), edep, time, tofNb);
  }
  if(tofNb == 2) {
    digi = new((*fBeamDetToFDigi2)[fBeamDetToFDigi2->GetEntriesFast()])
                ERBeamDetTOFDigi(fBeamDetToFDigi2->GetEntriesFast(), edep, time, tofNb);
  }
  return digi;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetDigitizer)
