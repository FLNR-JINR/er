/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSimtoRaw.h"

#include "FairRun.h"

#include<iostream>

using namespace std;

// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw(Int_t NEvents)
  : FairTask("Convert Sim event to Raw event", NEvents),
fNChanels(2),
fEvent(0),
fNPoints(1000),
fPixelSignals(NULL),
fRawEvents(NULL)
{
}
// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw()
  : FairTask("Convert Sim event to Raw event"),
fNChanels(2),
fEvent(0),
fNPoints(1000),
fPixelSignals(NULL),
fRawEvents(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::~ERSimtoRaw()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERSimtoRaw::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  // fPixelSignals = (ERNeuRadPixelSignal*) ioman->GetObject("NeuRadPixelSignal");
  fPixelSignals = (TClonesArray*) ioman->GetObject("NeuRadPixelSignal");
    if (!fPixelSignals)
      Fatal("Init", "Can`t find branch in input file!");


  //Register output objects
  fRawEvents = new ERNeuRadRawEvent*[fNChanels];
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fRawEvents[iChanel] = new ERNeuRadRawEvent(fNPoints);
    TString bName;
    bName.Form("ch%d.",iChanel+1);
    ioman->Register(bName,"Raw", fRawEvents[iChanel], kTRUE);
  }

      return kSUCCESS;
}  
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERSimtoRaw::Exec(Option_t* opt)
{
  fEvent++;
  if ( !(fEvent%500) ) { std::cout << "####### EVENT " << fEvent << " #####" << std::endl; }
  Int_t simSize,side,counter,PEnum;
  Double_t maxAmp, StartTime, FinishTime;


  
  FairRun* run = FairRun::Instance();
  run->MarkFill(kFALSE);
  for (Int_t iSignal = 0; iSignal < fPixelSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fPixelSignals->At(iSignal);
    if ( signal->PixelNb() != 136 ) continue; // for obtaining data from single chosen fiber
    TArrayF* result = signal->ResultSignal();
    maxAmp = 0;
    simSize = 0;
    simSize = result->GetSize();

    if ( signal->Side() == 0 ){
      for(Int_t i = 0; i<simSize; i++){
        if( maxAmp < result->GetAt(i) ) {maxAmp = result->GetAt(i);}
      }  //searching maxAmp
      if(maxAmp > 0.) { // trigger in [mV]
        run->MarkFill(kTRUE);
      }
    }
    StartTime = signal->GetStartTime();
    FinishTime = signal->GetFinishTime();

    for(Int_t nPoint = 0; nPoint< fNPoints; nPoint++) {

      if( nPoint<=StartTime*10 || nPoint>=(FinishTime*10-1) ) {
        if(signal->Side() == 0) {fRawEvents[0]->SetAmp(0, nPoint);}
        if(signal->Side() == 1) {fRawEvents[1]->SetAmp(0, nPoint);}
      }
      else {
        if(signal->Side() == 0) {fRawEvents[0]->SetAmp(-result->GetAt(nPoint - StartTime*10), nPoint);} //fill the RawEvent
        if(signal->Side() == 1) {fRawEvents[1]->SetAmp(-result->GetAt(nPoint - StartTime*10), nPoint);}
      }
      
      if(signal->Side() == 0) {fRawEvents[0]->SetTime(nPoint*0.1, nPoint);}
      if(signal->Side() == 1) {fRawEvents[1]->SetTime(nPoint*0.1, nPoint);}
    }

    if(signal->Side() == 0) fRawEvents[0]->SetStartTime(StartTime);
    if(signal->Side() == 1) fRawEvents[1]->SetStartTime(StartTime);
    if(signal->Side() == 0) fRawEvents[0]->SetFinishTime(FinishTime);
    if(signal->Side() == 1) fRawEvents[1]->SetFinishTime(FinishTime);
	
    if(signal->Side() == 0) {
      fRawEvents[0]->SetPETimes(signal->GetPETimes());
      fRawEvents[0]->SetPEAmps(signal->GetPEAmps());
      fRawEvents[0]->SetEvent(fEvent);
    }
    if(signal->Side() == 1) {
      fRawEvents[1]->SetPETimes(signal->GetPETimes());
      fRawEvents[1]->SetPEAmps(signal->GetPEAmps());
      fRawEvents[1]->SetEvent(fEvent);
    }
  }
}// pixelsignal entries
//----------------------------------------------------------------------------

