/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadViewer.h"

#include "TGraph.h"
#include "TArrayF.h"
#include "TDirectory.h"
#include "TString.h"
#include "TFile.h"
#include "TClonesArray.h"

#include "FairRunAna.h"
//#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairEventHeader.h"

#include "ERNeuRadPixelSignal.h"

// ----------------------------------------------------------------------------
ERNeuRadViewer::ERNeuRadViewer()
  : FairTask("ER NeuRad Viewer"),
    fNeuRadPixelSignals(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadViewer::ERNeuRadViewer(Int_t verbose)
  : fNeuRadPixelSignals(NULL),
    FairTask("ER NeuRad Viewer", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadViewer::~ERNeuRadViewer()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//FIXME this is currently useless!
/*
void ERNeuRadViewer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
*/
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
InitStatus ERNeuRadViewer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPixelSignals = (TClonesArray*) ioman->GetObject("NeuRadPixelSignal");
  if ( ! fNeuRadPixelSignals) Fatal("ERNeuRadViewer::Init","No NeuRadPixelSignal Collection in input file");

  fFile = FairRunAna::Instance()->GetOutputFile();
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadViewer::Exec(Option_t* opt)
{
  TString dirTitle;
  dirTitle.Form("Event%d",FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber());
  TDirectory* dir = fFile->mkdir(dirTitle.Data());

  dir->cd();
  TDirectory* front = dir->mkdir("front");
  TDirectory* back = dir->mkdir("back");

  // Loop over pixel signals
  for (Int_t iSignal=0; iSignal < fNeuRadPixelSignals->GetEntriesFast(); iSignal++)
  {
    // Get current pixel signal
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fNeuRadPixelSignals->At(iSignal);
    
    TArrayF* result = signal->ResultSignal();
    Int_t count = (Int_t)((signal->GetFinishTime() - signal->GetStartTime()) / signal->GetdT());
    Float_t* times = new Float_t[count];

    for (Int_t i=0; i<count; i++) {
      times[i] = signal->GetStartTime() + signal->GetdT()*i; 
    }

    TGraph* gr = new TGraph(count, times, result->GetArray());
    TString title;
    title.Form("PMT Signal, side %d,%d fiber, %d module, %d pe, %d",
      signal->GetSide(), signal->GetPixelNb(), signal->GetModuleNb(),
      signal->GetPECount(), FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber());
    gr->SetTitle(title);

    // Select directory to write into
    if (signal->GetSide() == 0) {
      front->cd();
    } else {
      back->cd();
    }

    // Write graph into the output file
    gr->Write();

    // Free memory
    delete gr;
    delete [] times;

  } // End of loop over pixel signals
  fFile->cd(); //TODO are you sure?
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadViewer::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadViewer::Finish()
{
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadViewer)
