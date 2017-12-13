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

Double_t ERNeuRadWBTanalyser::fThresholdStep = 500.;
Double_t ERNeuRadWBTanalyser::fThresholdMax = 10000.;
Int_t ERNeuRadWBTanalyser::fNthresholdSteps = ERNeuRadWBTanalyser::fThresholdMax / ERNeuRadWBTanalyser::fThresholdStep;

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
  if (fHistoCounterF)  { delete fHistoCounterF;  fHistoCounterF = NULL; }
  if (fHistoCounterB)  { delete fHistoCounterB;  fHistoCounterB = NULL; }
  if (fHeatMapF)       { delete fHeatMapF;       fHeatMapF = NULL; }
  if (fHeatMapB)       { delete fHeatMapB;       fHeatMapB = NULL; }
  //if (fHistoThrScanF)  { delete fHistoThrScanF;  fHistoThrScanF = NULL; }
  if (fHistoThrScanB)  { delete fHistoThrScanB;  fHistoThrScanB = NULL; }

  //if (fHistoThrScanCounterF) { delete fHistoThrScanCounterF; fHistoThrScanCounterF = NULL; }
  if (fHistoThrScanCounterB) { delete fHistoThrScanCounterB; fHistoThrScanCounterB = NULL; }
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
  fHistoCounterF = new TH1D("fHistoCounterF", "fHistoCounterF", 64, 0., 64.);
  fHistoChannelsF = new TH1D("fHistoChannelsF", "fHistoChannelsF", 64, 0., 64.);
  fHistoAmpsF = new TH1D("fHistoAmpsF", "fHistoAmpsF", 300, 0., 30000.);

  fHistoCounterB = new TH1D("fHistoCounterB", "fHistoCounterB", 64, 0., 64.);
  fHistoChannelsB = new TH1D("fHistoChannelsB", "fHistoChannelsB", 64, 0., 64.);
  fHistoAmpsB = new TH1D("fHistoAmpsB", "fHistoAmpsB", 300, 0., 30000.);

  fHeatMapF = new TH2D("fHeatMapF", "fHeatMapF", 8, 0., 8., 8, 0., 8.);
  fHeatMapB = new TH2D("fHeatMapB", "fHeatMapB", 8, 0., 8., 8, 0., 8.);

  //TODO same should be for the front side
  //fHistoThrScanF = ...
  fHistoThrScanB = new TH2D("fHistoThrScanB", "fHistoThrScanB",
                             fNthresholdSteps, 0., (Double_t)fNthresholdSteps,
                             300, 0., 30000.);

  //fHistoThrScanCounterF = ...
  fHistoThrScanCounterB = new TH2D("fHistoThrScanCounterB", "fHistoThrScanCounterB",
                                    fNthresholdSteps, 0., (Double_t)fNthresholdSteps,
                                    64, 0., 64.);

}

void ERNeuRadWBTanalyser::ExportROOT(TString filename)
{
  TFile* outputFile = new TFile(filename, "RECREATE");

  fHistoCounterF->Write();
  fHistoChannelsF->Write();
  fHistoAmpsF->Write();

  fHistoCounterB->Write();
  fHistoChannelsB->Write();
  fHistoAmpsB->Write();

  fHeatMapF->Write();
  fHeatMapB->Write();

  //fHistoThrScanF->Write();
  fHistoThrScanB->Write();

  //fHistoThrScanCounterF->Write();
  fHistoThrScanCounterB->Write();

  outputFile->Close();
}

void ERNeuRadWBTanalyser::ExportPictures(void)
{
  //FIXME hardcode!!!!!!
  TCanvas* canv = new TCanvas("canv", "canv", 1600, 800);


  canv->SetTitle("fHistoCounterF");
  fHistoCounterF->Draw();
  canv->SaveAs("results/pictures/fHistoCounterF.png");

  canv->SetTitle("fHistoChannelsF");
  fHistoChannelsF->Draw();
  canv->SaveAs("results/pictures/fHistoChannelsF.png");

  canv->SetTitle("fHistoAmpsF");
  fHistoAmpsF->Draw();
  canv->SaveAs("results/pictures/fHistoAmpsF.png");


  canv->SetTitle("fHistoCounterB");
  fHistoCounterB->Draw();
  canv->SaveAs("results/pictures/fHistoCounterB.png");

  canv->SetTitle("fHistoChannelsB");
  fHistoChannelsB->Draw();
  canv->SaveAs("results/pictures/fHistoChannelsB.png");

  canv->SetTitle("fHistoAmpsB");
  fHistoAmpsB->Draw();
  canv->SaveAs("results/pictures/fHistoAmpsB.png");


  canv->SetTitle("fHeatMapF");
  fHeatMapF->Draw("COLTEXT");
  canv->SaveAs("results/pictures/fHeatMapF.png");

  canv->SetTitle("fHeatMapB");
  fHeatMapB->Draw("COLTEXT");
  canv->SaveAs("results/pictures/fHeatMapB.png");

  //canv->SetTitle("fHistoThrScanF");
  //fHistoThrScanF->Draw("COL");
  //canv->SaveAs("results/pictures/fHistoThrScanF.png");

  canv->SetTitle("fHistoThrScanB");
  fHistoThrScanB->Draw("COL");
  canv->SaveAs("results/pictures/fHistoThrScanB.png");

  //canv->SetTitle("fHistoThrScanCounterF");
  //fHistoThrScanCounterF->Draw("COL");
  //canv->SaveAs("results/pictures/fHistoThrScanCounterF.png");

  canv->SetTitle("fHistoThrScanCounterB");
  fHistoThrScanCounterB->Draw("COL");
  canv->SaveAs("results/pictures/fHistoThrScanCounterB.png");

  delete canv;
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Exec(Option_t* opt)
{
  //TODO is this the correct way to get the current event number?
  Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();

  // If there is nothing in the input TClonesArrays - the event is empty, skip it
  if (fNeuRadPhotoElectron->GetEntries() == 0 && fNeuRadPixelSignal->GetEntries() == 0) {
    LOG(DEBUG2) << "Event " << iEvent << " is empty" << FairLogger::endl;
    return;
  } else {
    LOG(INFO) << "Event " << iEvent << "\t"
              << "NeuRadPhotoElectron.Count=" << fNeuRadPhotoElectron->GetEntries() << "\t"
              << "NeuRadPixelSignal.Count=" << fNeuRadPixelSignal->GetEntries() << FairLogger::endl;
  }

  // iPE - i photoelectron
  //for (Int_t iPE=0; iPE<fNeuRadPhotoElectron->GetEntries(); iPE++) {
  //  ERNeuRadPhotoElectron* curPhotoElectron = (ERNeuRadPhotoElectron*)fNeuRadPhotoElectron->At(iPE);
  //  curPhotoElectron->Print();
  //}

  Double_t sumF = 0.;
  Double_t sumB = 0.;

  Int_t counterF = 0;
  Int_t counterB = 0;

  // iPS - i pixel signal
  for (Int_t iPS=0; iPS<fNeuRadPixelSignal->GetEntries(); iPS++) {
    ERNeuRadPixelSignal* curPixelSignal = (ERNeuRadPixelSignal*)fNeuRadPixelSignal->At(iPS);
    //curPixelSignal->Print();

    Int_t side = curPixelSignal->GetSide();
    Int_t x = curPixelSignal->GetPixelNb() % 8; //TODO implement SETUP acquiring
    Int_t y = curPixelSignal->GetPixelNb() / 8; //TODO implement SETUP acquiring

    if (side == 0) {
      // Process only signals of the front (smaller Z) side
      LOG(INFO) << "side=" << side << "\t"
                 << "pixelNb=" << curPixelSignal->GetPixelNb() << "\t"
                 << "x=" << x << "\t" << "y=" << y << "\t"
                 << "amp=" << curPixelSignal->GetAmplitudesSum() << "\t"
                 << FairLogger::endl;

      //fHistoChannelsF->Fill(curPixelSignal->GetPixelNb());
      //fHistoAmpsF->Fill(curPixelSignal->GetAmplitudesSum());
      sumF += curPixelSignal->GetAmplitudesSum();
      counterF++;
      fHeatMapF->Fill(x, y);

    } else if (side == 1) {
      // Process only signals of the back (biggger Z) side
      LOG(INFO) << "side=" << side << "\t"
                 << "pixelNb=" << curPixelSignal->GetPixelNb() << "\t"
                 << "x=" << x << "\t" << "y=" << y << "\t"
                 << "amp=" << curPixelSignal->GetAmplitudesSum() << "\t"
                 << FairLogger::endl;

      //fHistoChannelsB->Fill(curPixelSignal->GetPixelNb());
      //fHistoAmpsB->Fill(curPixelSignal->GetAmplitudesSum());
      sumB += curPixelSignal->GetAmplitudesSum();
      counterB++;
      fHeatMapB->Fill(x, y);

    }
  }

  fHistoCounterF->Fill(counterF);
  fHistoAmpsF->Fill(sumF);

  fHistoCounterB->Fill(counterB);
  fHistoAmpsB->Fill(sumB);

  for (Int_t iThr=0; iThr<fNthresholdSteps; iThr++) {

    Double_t curThr = fThresholdStep * iThr;

    sumF = 0.;
    sumB = 0.;
    counterF = 0;
    counterB = 0;

    // iPS - i pixel signal
    for (Int_t iPS=0; iPS<fNeuRadPixelSignal->GetEntries(); iPS++) {
      ERNeuRadPixelSignal* curPixelSignal = (ERNeuRadPixelSignal*)fNeuRadPixelSignal->At(iPS);

      Int_t side = curPixelSignal->GetSide();
      Int_t chId = curPixelSignal->GetPixelNb();
      Int_t x = chId % 8; //TODO implement SETUP acquiring
      Int_t y = chId / 8; //TODO implement SETUP acquiring

      if (side == 1) {
        // Process only signals of the back (biggger Z) side
        /*LOG(INFO) << "side=" << side << "\t"
                   << "thr=" << curThr << "\t"
                   << "pixelNb=" << curPixelSignal->GetPixelNb() << "\t"
                   << "x=" << x << "\t" << "y=" << y << "\t"
                   << "amp=" << curPixelSignal->GetAmplitudesSum() << "\t"
                   << FairLogger::endl;*/

        if (curPixelSignal->GetAmplitudesSum() > curThr) {
          //LOG(INFO) << "This signal is above threshold!" << FairLogger::endl;
          sumB += curPixelSignal->GetAmplitudesSum();
          counterB++;
        }
      }
    }

    if (sumB > 0.) {
      fHistoThrScanB->Fill((Double_t)iThr+0.5, sumB);
    }

    //fHistoThrScanCounterF->Fill((Double_t)iThr+0.5, counterF);
    fHistoThrScanCounterB->Fill((Double_t)iThr+0.5, counterB);

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
