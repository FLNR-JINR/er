/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERN15B11Digitizer.h"

#include <TClonesArray.h>

#include "ERDigi.h"
#include "ERPoint.h"

ERN15B11Digitizer::ERN15B11Digitizer()
  : FairTask()
{
}

ERN15B11Digitizer::~ERN15B11Digitizer()
{
}

InitStatus ERN15B11Digitizer::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* BranchsNamesList = ioman->GetBranchNameList();
  TIter next(BranchsNamesList);
  TObjString* branchName;
  while (branchName = (TObjString*)next()) {
    TString curBranchName = branchName->GetString();
    if (curBranchName.Contains("Point")) {
      // Input
      fPoints[curBranchName] = (TClonesArray*) ioman->GetObject(curBranchName); // input branch name
      if (!fPoints[curBranchName]) {
        Fatal("Init", "Can't find input collection DetectorvDetGasPartPoint!");
      }

      // Output
      fDigis[curBranchName] = new TClonesArray("ERDigi"); // class name
      ioman->Register(curBranchName(0, curBranchName.Length()-5)+"Digi", curBranchName, fDigis[curBranchName], kTRUE);
    }
  }
  return kSUCCESS;
}

void ERN15B11Digitizer::Exec(Option_t* option)
{
  for(const auto &itPoints: fPoints) {
    TClonesArray* curBranchArray = itPoints.second;
    Double_t curEdep = 0.;
    Double_t curTime = 0.;
    Int_t curEntries = curBranchArray->GetEntries();
    for (UInt_t i = 0; i < curEntries; i++) {
      ERPoint* curPoint = (ERPoint*)curBranchArray->At(i);
      curEdep = curEdep + curPoint->GetEnergyLoss();
      curTime = curTime + curPoint->GetTime();
    } // for end
    TClonesArray* curOutBra = fDigis[itPoints.first];
    AddOutputDigi(*curOutBra, curEdep, curTime);
  } // for end
}

void ERN15B11Digitizer::AddOutputDigi(TClonesArray& clref, Double_t Edep, Double_t Time)
{
  UInt_t size = clref.GetEntries();
  new(clref[size])ERDigi(Edep, Time, 1); // id = 1, Edep = Edep, time = Time, volNb = 1
}

void ERN15B11Digitizer::FinishEvent()
{
  for (const auto &itfDigis: fDigis) {
    itfDigis.second->Clear();
  }
}

ClassImp(ERN15B11Digitizer)
