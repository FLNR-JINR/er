/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadWBTanalyser.h"

#include "FairLogger.h"

// ----------------------------------------------------------------------------
ERNeuRadWBTanalyser::ERNeuRadWBTanalyser()
  : FairTask("ER NeuRad WBT analyser")
{
}

// ----------------------------------------------------------------------------
ERNeuRadWBTanalyser::~ERNeuRadWBTanalyser()
{
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

  return kSUCCESS;
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Exec(Option_t* opt)
{
	LOG(INFO) << "ERNeuRadWBTanalyser: event " << FairLogger::endl;
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Finish()
{
}

// ----------------------------------------------------------------------------
void ERNeuRadWBTanalyser::Reset()
{
}

// ----------------------------------------------------------------------------
ClassImp(ERNeuRadWBTanalyser)
