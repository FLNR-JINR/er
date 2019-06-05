#include "ERDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERPoint.h"
#include "ERDecayMCEventHeader.h"

#include <iostream>
#include <algorithm>
using namespace std;

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERDigitizer::ERDigitizer()
  : ERTask("ER common digitization")
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERDigitizer::ERDigitizer(TString name)
  : ERTask(name)
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERDigitizer::ERDigitizer(TString name, Int_t verbose)
  : ERTask(name, verbose)
{
  fAvailibleRunManagers.push_back("ERRunSim");
  fAvailibleRunManagers.push_back("ERRunAna");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERDigitizer::~ERDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  TList* allBranchNames = ioman->GetBranchNameList();;
  TIter nextBranch(allBranchNames);
  TObjString* bName;
  vector<TString> pointBranches;
  while (bName = (TObjString*)nextBranch()){
    if (bName->GetString().Contains(fName) && bName->GetString().Contains("Point")){
      pointBranches.push_back(bName->GetString());
    }
  }

  for (Int_t iBranch = 0; iBranch < pointBranches.size(); iBranch++){
    TString pBranch = pointBranches[iBranch];
    fSenVolPoints[pBranch] = (TClonesArray*) ioman->GetObject(pBranch);
    fSenVolDigis[pBranch] = new TClonesArray("ERDigi");
    ioman->Register(pBranch(0,pBranch.Length()-5)+"Digi", fName, fSenVolDigis[pBranch], kTRUE);
  }
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERDigitizer::Exec(Option_t* opt){

  Reset();


  for(const auto &itSenVol: fSenVolPoints){
    TClonesArray* senVolPoints = itSenVol.second;
    //Sort the points by volNb
    map<Int_t, vector<Int_t>> points;
    for (Int_t iPoint = 0; iPoint < senVolPoints->GetEntriesFast(); iPoint++){
      ERPoint* point = (ERPoint*)senVolPoints->At(iPoint);
      points[point->GetVolNb()].push_back(iPoint);
    }

    for(const auto &itVol: points){
      fEdep = 0.; //sum edep in vol
      fTime = numeric_limits<float>::max(); // min time in vol
      
      for (const auto itPoint: itVol.second){
        ERPoint* point = (ERPoint*)(senVolPoints->At(itPoint));
        fEdep += point->GetEnergyLoss();

        if (point->GetTime() < fTime){
          fTime = point->GetTime();
        }
      }


      TString volName = itSenVol.first(fName.Length(),itSenVol.first.Length()-fName.Length()-5);
      
      if (fSenVolErrors.find(volName) == fSenVolErrors.end()){
        cout << "Error rule for vol name " << volName << " not found!!!" << endl;
      }
      else{ 
        ERDigitizerError error = fSenVolErrors[volName];
        Float_t sigma = error.a + error.b*TMath::Sqrt(fEdep) + error.c*fEdep;
        fEdep = gRandom->Gaus(fEdep, sigma);
      }
      /*
      if (edep < fElossThreshold)
        continue;

      time = gRandom->Gaus(time, fTimeSigma);
      */
      fVolNb = itVol.first;
      ERDigi *digi = AddDigi(fSenVolDigis[itSenVol.first]);
      /*
      for (const auto itPoint: itSensor.second){
        digi->AddLink(FairLink("RTelescopePoint", itPoint));
      }*/
    }
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERDigitizer::AddTrigger(TString stationSID, Int_t value, Int_t priority)
{

  for (const auto& trigger : fTriggers){
    if (trigger.first == stationSID)
      LOG(FATAL) << "Trigger with station SID " << stationSID 
                 << " already exist!" << FairLogger::endl;
    if (trigger.second.fValue == value)
      LOG(FATAL) << "Trigger with value " << value 
                  << " already exist!" << FairLogger::endl;
    if (trigger.second.fPriority == priority)
      LOG(FATAL) << "Trigger with priority " << priority 
                  << " already exist!" << FairLogger::endl;
  }

  fTriggers[stationSID] = ERTrigger(value, priority);

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERDigitizer::ApplyTrigger(TString stationSID, TClonesArray* digiCollection)
{
  // The functionality of working with triggers is currently implemented only for ERDecayMCEventHeader
  // and, accordingly, ERRunSim. In the future, this functionality should be made available for use 
  // from ERRunAna.

  if (fRun->ClassName() == TString("ERRunSim")){ // check Run class
    // check header class
    if ((((FairRunSim*)fRun)->GetMCEventHeader())->ClassName() != TString("ERDecayMCEventHeader")) 
      return;

    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)((FairRunSim*)fRun)->GetMCEventHeader();

    // Trigger only applied if stationSID is SID of trigger station
    // and if there is signal in station (which means there are digis)
    if (  (fTriggers.find(stationSID) != fTriggers.end()) && (digiCollection->GetEntriesFast() > 0) ){

      // It is possible to overwrite the current trigger value only if the priority is higher 
      // than the recorded one.
      if (fTriggers[stationSID].fPriority > header->GetTriggerPriority()){

        header->SetTrigger(fTriggers[stationSID].fValue);
        header->SetTriggerPriority(fTriggers[stationSID].fPriority);
      }

    }

  }

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERDigitizer::Reset()
{
  for(const auto &itSenVolDigi: fSenVolDigis){
    itSenVolDigi.second->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERDigitizer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERDigi* ERDigitizer::AddDigi(TClonesArray* digis)
{
  ERDigi *digi = new((*digis)[digis->GetEntriesFast()])
              ERDigi(digis->GetEntriesFast(), fEdep, fTime, fVolNb);
  return digi;
}
// ----------------------------------------------------------------------------
void ERDigitizer::AddError(TString volName,Float_t a, Float_t b, Float_t c){
  fSenVolErrors[volName] = ERDigitizerError(a,b,c);
}
//-----------------------------------------------------------------------------
ClassImp(ERDigitizer)