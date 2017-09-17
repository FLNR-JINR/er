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

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetTrackFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetMWPCDigiX1 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiX1");
  fBeamDetMWPCDigiX2 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiX2");
  fBeamDetMWPCDigiY1 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiY1");
  fBeamDetMWPCDigiY2 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiY2");

  // Register output array fBeamDetHits
  fBeamDetTrack = (ERBeamDetTrack*)new ERBeamDetTrack();

  ioman->Register("BeamDetTrack.", "BeamDet track", fBeamDetTrack, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt)
{ 
  Reset();
  std::cout << std::endl;

  if(fBeamDetMWPCDigiX1->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() > 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() > 1 ) {
    std::cout << "Multiplicity more than one" << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }
 
  if(fBeamDetMWPCDigiX1->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() < 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() < 1 ) {
    std::cout << "Multiplicity less than one" << std::endl;
    FairRun* run = FairRun::Instance();
    run->MarkFill(kFALSE);
    return ;
  }

  Double_t xFar, yFar, zFar; 
  Double_t xClose, yClose, zClose;
  Double_t coordinate;

  ERBeamDetMWPCDigi* digi; 

  digi = (ERBeamDetMWPCDigi*)fBeamDetMWPCDigiX1->At(0); 
  xFar = fBeamDetSetup->WireX(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);

  digi = (ERBeamDetMWPCDigi*)fBeamDetMWPCDigiY1->At(0);
  yFar = fBeamDetSetup->WireY(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);
  zFar = fBeamDetSetup->WireZ(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);

  digi = (ERBeamDetMWPCDigi*)fBeamDetMWPCDigiX2->At(0);
  xClose = fBeamDetSetup->WireX(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);
  
  digi = (ERBeamDetMWPCDigi*)fBeamDetMWPCDigiY2->At(0);
  yClose = fBeamDetSetup->WireY(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);
  zClose = fBeamDetSetup->WireZ(digi->GetMWPCNb()-1, digi->GetPlaneNb()-1, digi->GetWireNb()-1);

  TVector3 hitFar(xFar, yFar, zFar);
  TVector3 hitClose(xClose, yClose, zClose);
  TVector3 vectorOnTarget = hitClose - hitFar;

  cout << "Theta = " << vectorOnTarget.Theta() << "; Phi = " << vectorOnTarget.Phi() << endl;

  Double_t xTarget = xClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Cos(vectorOnTarget.Phi());
  Double_t yTarget = yClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Sin(vectorOnTarget.Phi());

  std::cout << "xFar = " <<  xFar << "; yFar = " << yFar << "; zFar = " << zFar << endl
            << "xClose = " <<  xClose << "; yClose = " << yClose << "; zClose = " << zClose << endl;

  if(TMath::Sqrt(xTarget*xTarget + yTarget*yTarget) <= fBeamDetSetup->TargetR()) {
    AddTrack(xTarget, yTarget, 0, vectorOnTarget.Unit());
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