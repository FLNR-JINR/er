#include "ERNeuRadMatcher.h"

#include <iostream>
#include <vector>
#include <map>

#include "TVector3.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERNeuRadHit.h"
#include "ERNeuRadStep.h"

// ----------------------------------------------------------------------------
ERNeuRadMatcher::ERNeuRadMatcher()
  : fNeuRadHits(NULL),
    fNeuRadFirstSteps(NULL),
    fHdxy(NULL),
    fHdxyLess6(NULL),
    fHdxyOF(NULL),
    fHdxyLess6OF(NULL),
    fHdxyTF(NULL),
    fHdxyLess6TF(NULL),
    FairTask("ER muSi hit producing scheme") //TODO muSi ???
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadMatcher::ERNeuRadMatcher(Int_t verbose)
  : fNeuRadHits(NULL),
    fNeuRadFirstSteps(NULL),
    fHdxy(NULL),
    fHdxyLess6(NULL),
    fHdxyOF(NULL),
    fHdxyLess6OF(NULL),
    fHdxyTF(NULL),
    fHdxyLess6TF(NULL),
    FairTask("ER muSi hit producing scheme ", verbose) //TODO muSi ???
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadMatcher::~ERNeuRadMatcher()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadMatcher::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
InitStatus ERNeuRadMatcher::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadHits = (TClonesArray*) ioman->GetObject("NeuRadHit");
  fNeuRadFirstSteps = (TClonesArray*) ioman->GetObject("NeuRadFirstStep");
  //todo check

  fHdxy = new TH1F("fHdxy", "XY distance",4000,0.,4.);
  fHdxy->GetXaxis()->SetTitle("XY disance [cm]");

  fHdxyLess6 = new TH1F("fHdxyLess6", "XY distance less 6mm",100.,0.,1.);
  fHdxyLess6->GetXaxis()->SetTitle("XY disance [cm]");

  fHdxyOF = new TH1F("fHdxyOF", "XY distance, One fiber mode",4000,0.,4.);
  fHdxyOF->GetXaxis()->SetTitle("XY disance [cm]");

  fHdxyLess6OF = new TH1F("fHdxyLess6OF", "XY distance less 6mm, One fibre mode",100.,0.,1.);
  fHdxyLess6OF->GetXaxis()->SetTitle("XY disance [cm]");

  fHdxyTF = new TH1F("fHdxyTF", "XY distance, Two fiber mode",4000,0.,4.);
  fHdxyTF->GetXaxis()->SetTitle("XY disance [cm]");

  fHdxyLess6TF = new TH1F("fHdxyLess6TF", "XY distance less 6mm, Two fibre mode",100.,0.,1.);
  fHdxyLess6TF->GetXaxis()->SetTitle("XY disance [cm]");
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadMatcher::Exec(Option_t* opt)
{
  ERNeuRadStep* step = (ERNeuRadStep*)fNeuRadFirstSteps->At(0);

  //Однофайберная мода
  if (fNeuRadHits->GetEntriesFast() == 1){
    ERNeuRadHit* hit = (ERNeuRadHit*)fNeuRadHits->At(0);
    Double_t dist = TMath::Sqrt((hit->GetX()-step->GetX())*(hit->GetX()-step->GetX()) + 
                                (hit->GetY()-step->GetY())*(hit->GetY()-step->GetY()));
    std::cerr << dist << std::endl;
    fHdxyOF->Fill(dist);
    if (dist < 0.6) {
      fHdxyLess6OF->Fill(dist);
    }
  }

  //Двухфайберная мода
  if (fNeuRadHits->GetEntriesFast() == 2 || fNeuRadHits->GetEntriesFast() == 1){
    ERNeuRadHit* hit1 = (ERNeuRadHit*)fNeuRadHits->At(0);
    ERNeuRadHit* hit2 = (ERNeuRadHit*)fNeuRadHits->At(0);
    Double_t dist = 0.;
    if (hit1->Time() < hit2->Time()) {
      dist = TMath::Sqrt((hit1->GetX()-step->GetX())*(hit1->GetX()-step->GetX()) + 
                                (hit1->GetY()-step->GetY())*(hit1->GetY()-step->GetY()));
    } else {
      dist = TMath::Sqrt((hit2->GetX()-step->GetX())*(hit2->GetX()-step->GetX()) + 
                                (hit2->GetY()-step->GetY())*(hit2->GetY()-step->GetY()));
    }

    fHdxyTF->Fill(dist);
    if(dist < 0.6) {
      fHdxyLess6TF->Fill(dist);
    }
  }

  //Многофайберная мода
  if (fNeuRadHits->GetEntriesFast() > 0) {
    //Находим самый первый хит
    ERNeuRadHit* firstHit;
    Float_t minTime = 999999999.;
    for(Int_t iHit = 0; iHit < fNeuRadHits->GetEntriesFast(); iHit++) {
      ERNeuRadHit* hit = (ERNeuRadHit*)fNeuRadHits->At(iHit);
      if (hit->Time() < minTime){
        minTime = hit->Time();
        firstHit = hit;
      }
    }
    Double_t dist = TMath::Sqrt((firstHit->GetX()-step->GetX())*(firstHit->GetX()-step->GetX()) + 
                                  (firstHit->GetY()-step->GetY())*(firstHit->GetY()-step->GetY()));
    fHdxy->Fill(dist);
    if(dist < 0.6)
      fHdxyLess6->Fill(dist);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadMatcher::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadMatcher::Finish()
{   
  fHdxy->Write();
  fHdxyLess6->Write();
  fHdxyOF->Write();
  fHdxyLess6OF->Write();
  fHdxyTF->Write();
  fHdxyLess6TF->Write();
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadMatcher)
