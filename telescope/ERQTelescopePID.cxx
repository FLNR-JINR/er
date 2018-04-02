/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopePID.h"

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERBeamDetTrack.h"

#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID()
  : FairTask("ER qtelescope particle identification scheme")
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::ERQTelescopePID(Int_t verbose)
  : FairTask("ER qtelescope particle identification scheme", verbose)
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopePID::~ERQTelescopePID() {
}
//--------------------------------------------------------------------------------------------------
InitStatus ERQTelescopePID::Init() {
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;

  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    
    if (bFullName.Contains("Digi") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeDigi[brName] = (TClonesArray*) ioman->GetObject(bFullName);
    }

    
    if (bFullName.Contains("Track") && bFullName.Contains("QTelescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      fQTelescopeTrack[brName] = (TClonesArray*) ioman->GetObject(bFullName);

      //Creating particle collection for evry track collection
      fQTelescopeParticle[brName] = new TClonesArray("ERQTelescopeParticle");
      ioman->Register("ERQTelescopeParticle_" + brName, "QTelescope", 
                    fQTelescopeParticle[brName], kTRUE);

    }
  }

  fQTelescopeSetup = ERQTelescopeSetup::Instance();
  fQTelescopeSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Exec(Option_t* opt) { 
  Reset();

}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Reset() {
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::Finish() {   
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeParticle* ERQTelescopePID::AddParticle(Double_t deadEloss, TString digiBranchName) 
{
  ERQTelescopeParticle *particle = new((*fQTelescopeParticle[digiBranchName])
                                        [fQTelescopeParticle[digiBranchName]->GetEntriesFast()])
                                        ERQTelescopeParticle(deadEloss);
  return particle;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopePID::SetParContainers() {
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopePID)