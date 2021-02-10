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
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERRunAna.h"
#include "ERDigi.h"

//--------------------------------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder()
  : ERTask("ER BeamDet track finding scheme")
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder(Int_t verbose)
  : ERTask("ER BeamDet track finding scheme ", verbose)
{
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
InitStatus ERBeamDetTrackFinder::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;

  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  if (fTargetVolName == "") 
    Fatal("Init", "TargetVolumeName for ERBeamDetTrackFinder not defined! ");
  
  fBeamDetMWPCDigiX1 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiX1");
  fBeamDetMWPCDigiX2 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiX2");
  fBeamDetMWPCDigiY1 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiY1");
  fBeamDetMWPCDigiY2 = (TClonesArray*) ioman->GetObject("BeamDetMWPCDigiY2");
  // Register output array fBeamDetHits
  fBeamDetTrack = new TClonesArray("ERBeamDetTrack",10);
  //fBeamDetTrack = (ERBeamDetTrack*)new ERBeamDetTrack();

  ioman->Register("BeamDetTrack", "BeamDet track", fBeamDetTrack, kTRUE);

  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
  fBeamDetSetup->GetGeoParamsFromParContainer();

  fRand = new TRandom3();

  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
Bool_t ERBeamDetTrackFinder::IsCluster(TClonesArray* digiArray) {
  Bool_t isCluster = kTRUE;
  for(Int_t i = 0; i < digiArray->GetEntries() - 1; i++) {
    ERDigi* digi             = (ERDigi*)digiArray->At(i);
    ERDigi* digiNeigborWire  = (ERDigi*)digiArray->At(i + 1);
    if((digiNeigborWire->Channel() - digi->Channel()) != 1) {
      isCluster = kFALSE;
      FairRun* run = FairRun::Instance();
      break;
    }
  }
  return isCluster;
}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetTrackFinder::CalcCoordinateAvg(const TString& digi_branch_name, 
                                                 const TClonesArray* digis, char coordType) {
  if (!digis->GetEntriesFast())
    LOG(FATAL) << "ERBeamDetTrackFinder::CalcCoordinateAvg: empty digi collection" 
               <<  FairLogger::endl;
  ERChannel channel_of_first_digi = dynamic_cast<ERDigi*>(digis->At(0))->Channel();
  ERChannel channel_of_last_digi = channel_of_first_digi;
  for (int iDigi(0); iDigi < digis->GetEntriesFast(); ++iDigi) {
    const auto channel = dynamic_cast<ERDigi*>(digis->At(iDigi))->Channel();;
    if (channel < channel_of_first_digi)
      channel_of_first_digi = channel;
    if (channel > channel_of_last_digi)
      channel_of_last_digi = channel;
  }
  switch (coordType) {
    case 'X':
      return 0.5*(fBeamDetSetup->GetWireGlobX(digi_branch_name, channel_of_first_digi-1) 
                  + fBeamDetSetup->GetWireGlobX(digi_branch_name, channel_of_last_digi-1));  
    case 'Y':
      return 0.5*(fBeamDetSetup->GetWireGlobY(digi_branch_name, channel_of_first_digi-1) 
                  + fBeamDetSetup->GetWireGlobY(digi_branch_name, channel_of_last_digi-1)); 
    case 'Z':
      return 0.5*(fBeamDetSetup->GetWireGlobZ(digi_branch_name, channel_of_first_digi-1) 
                  + fBeamDetSetup->GetWireGlobZ(digi_branch_name, channel_of_last_digi-1)); 
    default:
      LOG(FATAL) << "ERBeamDetTrackFinder::CalcCoordinateAvg: Unknown coordinate type " 
                 << FairLogger::endl;
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt) {
  Reset();
  LOG(DEBUG) << "[ERBeamDetTrackFinder]------------Started-----------------------------------------"
             << FairLogger::endl;

  if(fBeamDetMWPCDigiX1->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() < 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() < 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() < 1 ) {
    LOG(DEBUG) << "Multiplicity less than one" << FairLogger::endl;
    //fRun->MarkFill(kFALSE);
    return ;
  }

  Double_t xFar, yFar, zFar; 
  Double_t xClose, yClose, zClose;
  Double_t coordinate;

  Bool_t cluster;
  ERDigi* digi;
  // Check X1 cluster 
  if(fBeamDetMWPCDigiX1->GetEntriesFast() > 1) { // If multiplicity in wires array is more than one 
    cluster = IsCluster(fBeamDetMWPCDigiX1);    // check that all wires in array have are neigbours
    if(cluster) {
      // calculate average coordinate of wires
      xFar = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiX1", fBeamDetMWPCDigiX1, 'X'); 
      zFar = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiX1", fBeamDetMWPCDigiX1, 'Z');
    } else {
      //fRun->MarkFill(kFALSE);
      return;
    }
  } else {  // only one wire in array
    digi = (ERDigi*)fBeamDetMWPCDigiX1->At(0); 
    xFar = fBeamDetSetup->GetWireGlobX("BeamDetMWPCDigiX1", digi->Channel()-1);  
    xFar = fBeamDetSetup->GetWireGlobX("BeamDetMWPCDigiX1", digi->Channel()-1);  
    zFar = fBeamDetSetup->GetWireGlobZ("BeamDetMWPCDigiX1", digi->Channel()-1);
  }

  // Check X2 cluster
  if(fBeamDetMWPCDigiX2->GetEntriesFast() > 1) { // If multiplicity in wires array is more than one 
    cluster = IsCluster (fBeamDetMWPCDigiX2);    // check that all wires in array have are neigbours
    if(cluster) {
      // calculate average coordinate of wires
      xClose = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiX2",fBeamDetMWPCDigiX2, 'X'); 
    } else {
      //fRun->MarkFill(kFALSE);
      return;
    }
  } else {  // only one wire in array
    digi = (ERDigi*)fBeamDetMWPCDigiX2->At(0);
    xClose = fBeamDetSetup->GetWireGlobX("BeamDetMWPCDigiX2", digi->Channel()-1);      
  }
  // Check Y1 cluster
  if(fBeamDetMWPCDigiY1->GetEntriesFast() > 1 ) { // If multiplicity in wires array is more than one 
    cluster = IsCluster (fBeamDetMWPCDigiY1);     // check that all wires in array have are neigbours
    if(cluster) {
      // calculate average coordinate of wires
      yFar = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiY1", fBeamDetMWPCDigiY1, 'Y'); 
    } else {
      //fRun->MarkFill(kFALSE);
      return;
    }
  } else {  // only one wire in array
    digi = (ERDigi*)fBeamDetMWPCDigiY1->At(0);
    yFar = fBeamDetSetup->GetWireGlobY("BeamDetMWPCDigiY1", digi->Channel()-1);
  }
  // Check Y2 cluster
  if(fBeamDetMWPCDigiY2->GetEntriesFast() > 1 ) { // If multiplicity in wires array is more than one 
    cluster = IsCluster(fBeamDetMWPCDigiY2);     // check that all wires in array have are neigbours
    if(cluster) {
      // calculate average coordinate of wires
      yClose = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiY2", fBeamDetMWPCDigiY2, 'Y'); 
      zClose = ERBeamDetTrackFinder::CalcCoordinateAvg("BeamDetMWPCDigiY2", fBeamDetMWPCDigiY2, 'Z'); 
    } else {
      //fRun->MarkFill(kFALSE);
      return;
    }
  } else { // only one wire in array
    digi = (ERDigi*)fBeamDetMWPCDigiY2->At(0);
    yClose = fBeamDetSetup->GetWireGlobY("BeamDetMWPCDigiY2", digi->Channel()-1);
    zClose = fBeamDetSetup->GetWireGlobZ("BeamDetMWPCDigiY2", digi->Channel()-1);
  }
  TVector3 hitFar(xFar, yFar, zFar);
  TVector3 hitClose(xClose, yClose, zClose);
  TVector3 vectorOnTarget = hitClose - hitFar;

  LOG(DEBUG) << "Theta = " << vectorOnTarget.Theta() << "; Phi = " << vectorOnTarget.Phi() << FairLogger::endl;

  Double_t xTarget = xClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Cos(vectorOnTarget.Phi());
  Double_t yTarget = yClose - zClose*TMath::Tan(vectorOnTarget.Theta())*TMath::Sin(vectorOnTarget.Phi());

  LOG(DEBUG) << "xFar = " <<  xFar << "; yFar = " << yFar << "; zFar = " << zFar
             << "xClose = " <<  xClose << "; yClose = " << yClose << "; zClose = " << zClose << FairLogger::endl;

  TGeoNode* node;
  node = gGeoManager->InitTrack(xClose, yClose, zClose, vectorOnTarget.Unit().X(),
                                                        vectorOnTarget.Unit().Y(),
                                                        vectorOnTarget.Unit().Z());
  Double_t targetMiddleThicknessX;
  Double_t targetMiddleThicknessY;
  Double_t targetMiddleThicknessZ;

  Bool_t targetAffected = kFALSE;
  while(!gGeoManager->IsOutside()){    
    node = gGeoManager->FindNextBoundaryAndStep();
    if (!node)
      break;
    if ((TString(node->GetName()).Contains(fTargetVolName))) {
      targetAffected = kTRUE;
      break;
    }
  }
  if (!targetAffected){
    LOG(WARNING) << "Target is not affected" << FairLogger::endl;
    return;
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

  LOG(DEBUG) << "Point on target " << "(" << targetMiddleThicknessX << ", " 
                                          << targetMiddleThicknessY << ", "
                                          << targetMiddleThicknessZ << ") cm" 
                                          << FairLogger::endl;
  LOG(DEBUG) << "[ERBeamDetTrackFinder]------------Finished-----------------------------------------"
             << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetTrackFinder::Reset() {
  if (fBeamDetTrack) {
    fBeamDetTrack->Clear();
  }
}
//--------------------------------------------------------------------------------------------------
ERBeamDetTrack* ERBeamDetTrackFinder::AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v)
{
  return new((*fBeamDetTrack)[fBeamDetTrack->GetEntriesFast()])
              ERBeamDetTrack(xt, yt, zt, v);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetTrackFinder)