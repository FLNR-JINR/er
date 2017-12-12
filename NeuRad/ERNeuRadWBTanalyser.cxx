/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadWBTanalyser.h"

#include "TClonesArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include "FairLogger.h"
#include "FairRunAna.h"

#include "EREventHeader.h"
#include "ERNeuRadPhotoElectron.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

// ----------------------------------------------------------------------------
ERNeuRadWBTanalyser::ERNeuRadWBTanalyser()
  : FairTask("ER NeuRad WBT analyser")
{
}

// ----------------------------------------------------------------------------
ERNeuRadWBTanalyser::~ERNeuRadWBTanalyser()
{
  if (fHistoAmpsF)     { delete fHistoAmpsF;     fHistoAmpsF = NULL; }
  if (fHistoChannelsF) { delete fHistoChannelsF; fHistoChannelsF = NULL; }
  if (fHistoAmpsB)     { delete fHistoAmpsB;     fHistoAmpsB = NULL; }
  if (fHistoChannelsB) { delete fHistoChannelsB; fHistoChannelsB = NULL; }
}

// ----------------------------------------------------------------------------
InitStatus ERNeuRadWBTanalyser::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) Fatal("Init", "No FairRootManager");

  fNeuRadPhotoElectron = (TClonesArray*) ioman->GetObject("NeuRadPhotoElectron");
  if (!fNeuRadPhotoElectron) Fatal("ERNeuRadWBTanalyser::Init", "No NeuRadPhotoElectron Collection");

  fNeuRadPixelSignal = (TClonesArray*) ioman->GetObject("NeuRadPixelSignal");
  if (!fNeuRadPixelSignal) Fatal("ERNeuRadWBTanalyser::Init", "No NeuRadPixelSignal Collection");

  this->InitHistograms();

  return kSUCCESS;
}

void ERNeuRadWBTanalyser::InitHistograms(void)
{
  fHistoChannelsF = new TH1D("fHistoChannelsF", "fHistoChannelsF", 64, 0., 64.);
  fHistoAmpsF = new TH1D("fHistoAmpsF", "fHistoAmpsF", 200, 0., 20000.);
  fHistoChannelsB = new TH1D("fHistoChannelsB", "fHistoChannelsB", 64, 0., 64.);
  fHistoAmpsB = new TH1D("fHistoAmpsB", "fHistoAmpsB", 200, 0., 20000.);
}

void ERNeuRadWBTanalyser::ExportROOT(TString filename)
{
  TFile* outputFile = new TFile(filename, "RECREATE");

  fHistoChannelsF->Write();
  fHistoAmpsF->Write();
  fHistoChannelsB->Write();
  fHistoAmpsB->Write();

  outputFile->Close();
}

void ERNeuRadWBTanalyser::ExportPictures(void)
{
  //FIXME hardcode!!!!!!
  TCanvas* canv = new TCanvas("canv", "canv", 1600, 800);

  canv->SetTitle("fHistoChannelsF");
  fHistoChannelsF->Draw();
  canv->SaveAs("results/fHistoChannelsF.png");

  canv->SetTitle("fHistoAmpsF");
  fHistoAmpsF->Draw();
  canv->SaveAs("results/fHistoAmpsF.png");

  canv->SetTitle("fHistoChannelsB");
  fHistoChannelsB->Draw();
  canv->SaveAs("results/fHistoChannelsB.png");

  canv->SetTitle("fHistoAmpsB");
  fHistoAmpsB->Draw();
  canv->SaveAs("results/fHistoAmpsB.png");

  delete canv;
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Exec(Option_t* opt)
{
  //TODO is this the correct way to get the current event number?
  Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();

	LOG(INFO) << "Event " << iEvent << "\t"
            << "NeuRadPhotoElectron.Count=" << fNeuRadPhotoElectron->GetEntries() << "\t"
            << "NeuRadPixelSignal.Count=" << fNeuRadPixelSignal->GetEntries() << FairLogger::endl;

  // If there is nothing in the input TClonesArrays - the event is empty, skip it
  if (fNeuRadPhotoElectron->GetEntries() == 0 && fNeuRadPixelSignal->GetEntries() == 0) {
    LOG(DEBUG2) << "Empty event" << FairLogger::endl;
    return;
  }

  // iPE - i photoelectron
  //for (Int_t iPE=0; iPE<fNeuRadPhotoElectron->GetEntries(); iPE++) {
  //  ERNeuRadPhotoElectron* curPhotoElectron = (ERNeuRadPhotoElectron*)fNeuRadPhotoElectron->At(iPE);
  //  curPhotoElectron->Print();
  //}

  // iPS - i pixel signal
  for (Int_t iPS=0; iPS<fNeuRadPixelSignal->GetEntries(); iPS++) {
    ERNeuRadPixelSignal* curPixelSignal = (ERNeuRadPixelSignal*)fNeuRadPixelSignal->At(iPS);
    //curPixelSignal->Print();

    Int_t side = curPixelSignal->GetSide();
    if (side == 0) {
      // Process only signals of the front (smaller Z) side
      LOG(INFO) << "side=" << side << "\t"
                << "pixelNb=" << curPixelSignal->GetPixelNb() << "\t"
                << "amp=" << curPixelSignal->GetAmplitudesSum() << "\t"
                << FairLogger::endl;

      fHistoChannelsF->Fill(curPixelSignal->GetPixelNb());
      fHistoAmpsF->Fill(curPixelSignal->GetAmplitudesSum());
    } else if (side == 1) {
      // Process only signals of the back (biggger Z) side
      LOG(INFO) << "side=" << side << "\t"
                << "pixelNb=" << curPixelSignal->GetPixelNb() << "\t"
                << "amp=" << curPixelSignal->GetAmplitudesSum() << "\t"
                << FairLogger::endl;

      fHistoChannelsB->Fill(curPixelSignal->GetPixelNb());
      fHistoAmpsB->Fill(curPixelSignal->GetAmplitudesSum());
    }

  }
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Finish()
{
  //FIXME hardcode!!!!!!
  this->ExportROOT("results/analysisOutput.root");
  this->ExportPictures();
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Reset()
{
}

// ----------------------------------------------------------------------------
ClassImp(ERNeuRadWBTanalyser)
