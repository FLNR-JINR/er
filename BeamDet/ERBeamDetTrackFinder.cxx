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

  if (fTargetVolName == "") 
    Fatal("Init", "TragetVolumeName for ERBeamDetTrackFinder not defined! ");
  
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

  Bool_t claster = kFALSE;

  if(fBeamDetMWPCDigiX1->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiX2->GetEntriesFast() > 1 ||
     fBeamDetMWPCDigiY1->GetEntriesFast() > 1 || 
     fBeamDetMWPCDigiY2->GetEntriesFast() > 1 ) {
    claster = kTRUE;
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
    run->MarkFill(kFALSE);
    return ;
  }

  Double_t xFar, yFar, zFar; 
  Double_t xClose, yClose, zClose;
  Double_t coordinate;

  ERBeamDetMWPCDigi* digi; 

  if(!claster){
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
  }
  else {
    if(fBeamDetMWPCDigiX1->GetEntriesFast()>1) {

      vector<ERBeamDetMWPCDigi*> MWPCX1_digi;
      for(Int_t i = 0;i<fBeamDetMWPCDigiX1->GetEntries();i++){
         MWPCX1_digi.push_back((ERBeamDetMWPCDigi*)fBeamDetMWPCDigiX1->At(i));
      }
      for(Int_t j = 0; j<fBeamDetMWPCDigiX1->GetEntriesFast()-1; j++) {
        if(MWPCX1_digi[j+1]->GetWireNb() - MWPCX1_digi[j]->GetWireNb()!=1){;
          claster = kFALSE;
          FairRun* run = FairRun::Instance();
          run->MarkFill(kFALSE);
          break;
        }
      }
      if(claster) {
        xFar = 0.5*(fBeamDetSetup->WireX(MWPCX1_digi[0]->GetMWPCNb()-1, MWPCX1_digi[0]->GetPlaneNb()-1, MWPCX1_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireX(MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetMWPCNb()-1, MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetPlaneNb()-1, MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetWireNb()-1));    

        zFar = 0.5*(fBeamDetSetup->WireZ(MWPCX1_digi[0]->GetMWPCNb()-1, MWPCX1_digi[0]->GetPlaneNb()-1, MWPCX1_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireZ(MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetMWPCNb()-1, MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetPlaneNb()-1, MWPCX1_digi[fBeamDetMWPCDigiX1->GetEntries()-1]->GetWireNb()-1));    
      }
    }
    if(fBeamDetMWPCDigiX2->GetEntriesFast()>1 && claster) {

      vector<ERBeamDetMWPCDigi*> MWPCX2_digi;
      for(Int_t j = 0; j<fBeamDetMWPCDigiX2->GetEntries(); j++) {
         MWPCX2_digi.push_back((ERBeamDetMWPCDigi*)fBeamDetMWPCDigiX2->At(j));
      }
      for(Int_t j = 0; j<fBeamDetMWPCDigiX2->GetEntriesFast()-1; j++) {
        if(MWPCX2_digi[j+1]->GetWireNb() - MWPCX2_digi[j]->GetWireNb()!=1){
          claster = kFALSE;
          FairRun* run = FairRun::Instance();          
          run->MarkFill(kFALSE);
          break;
        }
      }
      if(claster) {
        xClose = 0.5*(fBeamDetSetup->WireX(MWPCX2_digi[0]->GetMWPCNb()-1, MWPCX2_digi[0]->GetPlaneNb()-1, MWPCX2_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireX(MWPCX2_digi[fBeamDetMWPCDigiX2->GetEntries()-1]->GetMWPCNb()-1, MWPCX2_digi[fBeamDetMWPCDigiX2->GetEntries()-1]->GetPlaneNb()-1, MWPCX2_digi[fBeamDetMWPCDigiX2->GetEntries()-1]->GetWireNb()-1));    
      }
    }
    if(fBeamDetMWPCDigiY1->GetEntriesFast()>1 && claster){

      vector<ERBeamDetMWPCDigi*> MWPCY1_digi;// = new ERBeamDetMWPCDigi*[fBeamDetMWPCDigiY1->GetEntries()];
      for(Int_t j = 0; j<fBeamDetMWPCDigiY1->GetEntries(); j++) {
         MWPCY1_digi.push_back((ERBeamDetMWPCDigi*)fBeamDetMWPCDigiY1->At(j));
      }
      for(Int_t j = 0; j<fBeamDetMWPCDigiY1->GetEntriesFast()-1; j++) {
        if(MWPCY1_digi[j+1]->GetWireNb() - MWPCY1_digi[j]->GetWireNb()!=1) {
          claster = kFALSE;
          FairRun* run = FairRun::Instance();          
          run->MarkFill(kFALSE);
          break;
        }
      }
      if(claster) {
        yFar = 0.5*(fBeamDetSetup->WireX(MWPCY1_digi[0]->GetMWPCNb()-1, MWPCY1_digi[0]->GetPlaneNb()-1, MWPCY1_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireX(MWPCY1_digi[fBeamDetMWPCDigiY1->GetEntries()-1]->GetMWPCNb()-1, MWPCY1_digi[fBeamDetMWPCDigiY1->GetEntries()-1]->GetPlaneNb()-1, MWPCY1_digi[fBeamDetMWPCDigiY1->GetEntries()-1]->GetWireNb()-1));    
      }
    }
    if(fBeamDetMWPCDigiY2->GetEntriesFast()>1 && claster){

      vector<ERBeamDetMWPCDigi*> MWPCY2_digi;// = new ERBeamDetMWPCDigi*[fBeamDetMWPCDigiY2->GetEntries()];
      for(Int_t j = 0; j<fBeamDetMWPCDigiY2->GetEntries(); j++) {
         MWPCY2_digi.push_back((ERBeamDetMWPCDigi*)fBeamDetMWPCDigiY2->At(j));
      }
      for(Int_t j = 0; j<fBeamDetMWPCDigiY2->GetEntriesFast()-1; j++) {
        if(MWPCY2_digi[j+1]->GetWireNb() - MWPCY2_digi[j]->GetWireNb()!=1) {
          claster = kFALSE;
          FairRun* run = FairRun::Instance();          
          run->MarkFill(kFALSE);
          break;
        }
      }
      if(claster) {
        yClose = 0.5*(fBeamDetSetup->WireX(MWPCY2_digi[0]->GetMWPCNb()-1, MWPCY2_digi[0]->GetPlaneNb()-1, MWPCY2_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireX(MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetMWPCNb()-1, MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetPlaneNb()-1, MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetWireNb()-1));    

        zClose = 0.5*(fBeamDetSetup->WireZ(MWPCY2_digi[0]->GetMWPCNb()-1, MWPCY2_digi[0]->GetPlaneNb()-1, MWPCY2_digi[0]->GetWireNb()-1) + 
        fBeamDetSetup->WireZ(MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetMWPCNb()-1, MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetPlaneNb()-1, MWPCY2_digi[fBeamDetMWPCDigiY2->GetEntries()-1]->GetWireNb()-1));
      }
    }
  }//else

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
    LOG(WARNING) << "Tatget is not affected" << FairLogger::endl;
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