#include "ERNeuRadViewer.h"

#include <iostream>
using namespace std;

#include "TGraph.h"
#include "TArrayF.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairEventHeader.h"

#include "ERNeuRadPixelSignal.h"

// ----------------------------------------------------------------------------
ERNeuRadViewer::ERNeuRadViewer()
  : fNeuRadPixelSignals(NULL),
    FairTask("ER NeuRad Viewer")
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
void ERNeuRadViewer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
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
  for (Int_t iSignal = 0; iSignal < fNeuRadPixelSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fNeuRadPixelSignals->At(iSignal);
    
    TArrayF* result = signal->ResultSignal();
    Int_t count = (Int_t)(signal->FinishTime()-signal->StartTime())/signal->dT();
    Float_t* times = new Float_t[count];
    for (Int_t i = 0; i < count; i++){
      times[i] = signal->StartTime() + signal->dT()*i; 
    }
    TGraph* gr = new TGraph(count,times,result->GetArray());
    TString title;
    title.Form("PMT Signal, side %d,%d fiber, %d module, %d pe, %d", signal->Side(),
                        signal->PixelNb(), signal->ModuleNb(), signal->PECount(),
                        FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber());
    gr->SetTitle(title);
    if (signal->Side() == 0)
      front->cd();
    else
      back->cd();
    gr->Write();
    delete gr;
    delete [] times;

  }
  fFile->cd();
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
