#include "ERRawToAnalyzeConverter.h"

#include<iostream>
using namespace std;


#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERRawToAnalyzeConverter::ERRawToAnalyzeConverter()
  : FairTask("Convert Raw event to analyse event"),
  fNChanels(2),
  fNPoints(1000),
  fRawEvents(NULL),
  fAEvents(NULL),
  fRatio(0.3),
  fTD(1.5), //in ns
  fNoiseMin(5),
  fNoiseMax(25),
  fEvent(0),
  fSmoothPar(kFALSE),
  fNumSmoothP(10)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRawToAnalyzeConverter::ERRawToAnalyzeConverter(Int_t verbose)
  : FairTask("Convert Raw event to analyse event", verbose),
  fNChanels(2),
  fNPoints(1000),
  fRawEvents(NULL),
  fAEvents(NULL),
  fRatio(0.3),
  fTD(1.5), //in ns
  fNoiseMin(5),
  fNoiseMax(25),
  fEvent(0),
  fSmoothPar(kFALSE),
  fNumSmoothP(10)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERRawToAnalyzeConverter::~ERRawToAnalyzeConverter()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRawToAnalyzeConverter::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERRawToAnalyzeConverter::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fRawEvents = new RawEvent*[fNChanels];
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    TString bName;
    bName.Form("ch%d.", iChanel+1);
    fRawEvents[iChanel] = (RawEvent*) ioman->GetObject(bName);
    if (!fRawEvents[iChanel])
      Fatal("Init", "Can`t find branch in input file!");
  }

  //Register output objects
  fAEvents = new AEvent*[fNChanels];
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fAEvents[iChanel] = new AEvent(fNPoints);
    TString bName;
    bName.Form("Ach%d.",iChanel+1);
    ioman->Register(bName,"Analyze", fAEvents[iChanel], kTRUE);
  }
  if (fSmoothPar == kTRUE ) { std::cout << std::endl << "SIGNALS WILL BE SMOOTHED!" << std::endl; }
  else std::cout << std::endl << "SIGNALS WILL NOT BE SMOOTHED!" << std::endl;
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERRawToAnalyzeConverter::Exec(Option_t* opt)
{
  fEvent++;
  if ( !(fEvent%500) ) { std::cout << "####### EVENT " << fEvent << " #####" << std::endl; }

/*std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;*/

  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fAEvents[iChanel]->SetInputEvent(&fRawEvents[iChanel]); //takes raw event from RawEvent
    fAEvents[iChanel]->SetCFratio(fRatio);
    fAEvents[iChanel]->SetCFtimeDelay(fTD);
    fAEvents[iChanel]->SetNoiseRange(fNoiseMin, fNoiseMax);
    fAEvents[iChanel]->SetSmoothPoints(fNumSmoothP);
    fAEvents[iChanel]->ProcessEvent(fSmoothPar);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERRawToAnalyzeConverter::Reset()
{
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fAEvents[iChanel]->Reset();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERRawToAnalyzeConverter::Finish()
{   

}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERRawToAnalyzeConverter)
