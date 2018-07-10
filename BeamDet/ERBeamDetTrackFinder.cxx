/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERBeamDetTrackFinder.h"

#include "TVector3.h"
#include "TMath.h"
#include "TGeoNode.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder()
  : FairTask("ER BeamDet track finding scheme"),
  fTargetVolName("")
{
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder(Int_t verbose)
  : FairTask("ER BeamDet track finding scheme ", verbose),
  fTargetVolName("")
{
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrackFinder::~ERBeamDetTrackFinder()
{
}
//--------------------------------------------------------------------------------------------------
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
  fBeamDetTrack = new TClonesArray("ERBeamDetTrack",1000);
  //fBeamDetTrack = (ERBeamDetTrack*)new ERBeamDetTrack();

  ioman->Register("BeamDetTrack", "BeamDet track", fBeamDetTrack, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
  fBeamDetSetup->GetGeoParamsFromParContainer();
   
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt)
{ 
  Reset();
  LOG(DEBUG) << FairLogger::endl;

  if(fBeamDetMWPCDigiX1->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() > 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() > 1 ) {
    LOG(DEBUG) << "Multiplicity more than one" << FairLogger::endl;
    FairRun* run = FairRun::Instance();
    // run->MarkFill(kFALSE);
    return ;
  }
 
  if(fBeamDetMWPCDigiX1->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() < 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() < 1 ) {
    LOG(DEBUG) << "Multiplicity less than one" << FairLogger::endl;
    FairRun* run = FairRun::Instance();
    // run->MarkFill(kFALSE);
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

  LOG(DEBUG) << "Theta = " << vectorOnTarget.Theta() << "; Phi = " << vectorOnTarget.Phi() << FairLogger::endl;

  Double_t xTarget = xClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Cos(vectorOnTarget.Phi());
  Double_t yTarget = yClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Sin(vectorOnTarget.Phi());

  LOG(DEBUG) << "xFar = " <<  xFar << "; yFar = " << yFar << "; zFar = " << zFar << FairLogger::endl
            << "xClose = " <<  xClose << "; yClose = " << yClose << "; zClose = " << zClose << FairLogger::endl;

  TGeoNode* node;
  node = gGeoManager->InitTrack(xClose, yClose, zClose, vectorOnTarget.Unit().X(),
                                                        vectorOnTarget.Unit().Y(),
                                                        vectorOnTarget.Unit().Z());
  Double_t targetMiddleThicknessX;
  Double_t targetMiddleThicknessY;
  Double_t targetMiddleThicknessZ;
  while(!gGeoManager->IsOutside()){    
    node = gGeoManager->FindNextBoundaryAndStep();
    if ((TString(node->GetName()).Contains(fTargetVolName))) {
      break;
    }
  }
  node = gGeoManager->FindNextBoundary();

  Double_t matThickness = gGeoManager->GetStep();
  gGeoManager->SetStep(matThickness / 2.);
  gGeoManager->Step();

  targetMiddleThicknessX = gGeoManager->GetCurrentPoint()[0];
  targetMiddleThicknessY = gGeoManager->GetCurrentPoint()[1];
  targetMiddleThicknessZ = gGeoManager->GetCurrentPoint()[2];

  AddTrack(targetMiddleThicknessX, 
           targetMiddleThicknessY, 
           targetMiddleThicknessZ, vectorOnTarget.Unit());

  LOG(INFO) << "Point on target " << "(" << targetMiddleThicknessX << ", " 
                                          << targetMiddleThicknessY << ", "
                                          << targetMiddleThicknessZ << ") cm" 
                                          << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::Reset()
{
  if (fBeamDetTrack) {
    fBeamDetTrack->Clear();
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::Finish()
{   
  
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrack* ERBeamDetTrackFinder::AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v)
{
  return new((*fBeamDetTrack)[fBeamDetTrack->GetEntriesFast()])
              ERBeamDetTrack(xt, yt, zt, v);
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::SetParContainers()
{

}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetTrackFinder)