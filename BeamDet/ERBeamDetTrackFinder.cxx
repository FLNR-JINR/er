#include "ERBeamDetTrackFinder.h"

#include <vector>
#include <iostream>
using namespace std;

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>

#include "ERDetectorList.h"

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder()
  : FairTask("ER BeamDet track finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder(Int_t verbose)
  : FairTask("ER BeamDet track finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::~ERBeamDetTrackFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetTrackFinder::SetParContainers()
{
  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetTrackFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetMWPCDigi = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigi");

  // Register output array fBeamDetHits
  fBeamDetTrack = (ERBeamDetTrack*)new ERBeamDetTrack();

  ioman->Register("BeamDetTrack.", "BeamDet track", fBeamDetTrack, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt)
{ 
  Reset();
  std::cout << std::endl;

  Int_t digiCount = fBeamDetMWPCDigi->GetEntriesFast();

  if(digiCount > 4) {
    std::cout << "Multiplicity more than one" << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  if(digiCount < 4) {
    std::cout << "Multiplicity less than one" << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  Int_t mwpcNb, planeNb, wireNb;
  Double_t xFar, yFar, zFar; 
  Double_t xClose, yClose, zClose;
  Double_t coordinate;
  for(Int_t iDigi = 0; iDigi < fBeamDetMWPCDigi->GetEntriesFast(); iDigi++) {
    ERBeamDetMWPCDigi* digi = (ERBeamDetMWPCDigi*)fBeamDetMWPCDigi->At(iDigi);
    mwpcNb = digi->GetMWPCNb() - 1;
    planeNb = digi->GetPlaneNb() - 1;
    wireNb = digi->GetWireNb() - 1; 
    switch(iDigi) {
      case 0:
        xFar = fBeamDetSetup->WireX(mwpcNb, planeNb, wireNb);
        break;
      case 1:
        yFar = fBeamDetSetup->WireY(mwpcNb, planeNb, wireNb);
        zFar = fBeamDetSetup->WireZ(mwpcNb, planeNb, wireNb);
        break;
      case 2:
        xClose = fBeamDetSetup->WireX(mwpcNb, planeNb, wireNb);
        break;
      case 3:
        yClose = fBeamDetSetup->WireY(mwpcNb, planeNb, wireNb);
        zClose = fBeamDetSetup->WireZ(mwpcNb, planeNb, wireNb);
        break;
    }
  }

  TVector3 vertexFar(xFar, yFar, zFar);
  TVector3 vertexClose(xClose, yClose, zClose);
  TVector3 vectorOnTarget = vertexClose - vertexFar;

  Double_t xTarget = xClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Cos(vectorOnTarget.Phi());
  Double_t yTarget = yClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Sin(vectorOnTarget.Phi());

  if(TMath::Sqrt(xTarget*xTarget + yTarget*yTarget) <= fBeamDetSetup->TargetR()) {
    AddTrack(xTarget, yTarget, 0, vectorOnTarget);
  }
  std::cout << "Point on target " << "(" << xTarget << ", " 
                                         << yTarget << ") cm" << endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Reset()
{
  if (fBeamDetTrack) {
    fBeamDetTrack->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Finish()
{   
  
}
// ----------------------------------------------------------------------------
ERBeamDetTrack* ERBeamDetTrackFinder::AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v)
{
  fBeamDetTrack->AddParameters(xt, yt, zt, v);
}
//-----------------------------------------------------------------------------
ClassImp(ERBeamDetTrackFinder)