/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRad.h"

#include "TClonesArray.h"

#include "TVirtualMC.h"
#include "TGeoMatrix.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERNeuRadGeoPar.h"
#include "ERMCEventHeader.h"
#include "ERNeuRadPoint.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadSetup.h"

//-------------------------------------------------------------------------------------------------
ERNeuRad::ERNeuRad()
  : ERDetector("ERNeuRad", kTRUE),
    fStorePrimarySteps(kFALSE),
    fStoreAllSteps(kFALSE),
    fNeuRadPoints(NULL),
    fNeuRadFirstStep(NULL),
    fNeuRadSteps(NULL)
{
  fNeuRadPoints = new TClonesArray("ERNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ERNeuRadStep");
  fNeuRadSteps = new TClonesArray("ERNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}

//-------------------------------------------------------------------------------------------------
ERNeuRad::ERNeuRad(const char* name, Bool_t active, Int_t verbose)
  : ERDetector(name, active),
    fVerbose(verbose),
    fStorePrimarySteps(kFALSE),
    fStoreAllSteps(kFALSE)
{
  fNeuRadPoints = new TClonesArray("ERNeuRadPoint");
  fNeuRadFirstStep = new TClonesArray("ERNeuRadStep");
  fNeuRadSteps = new TClonesArray("ERNeuRadStep");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
}

//-------------------------------------------------------------------------------------------------
ERNeuRad::~ERNeuRad() {
  if (fNeuRadPoints) {
    fNeuRadPoints->Delete();
    delete fNeuRadPoints;
  }
}

//-------------------------------------------------------------------------------------------------
TClonesArray* ERNeuRad::GetCollection(Int_t iColl) const {
  if (iColl == 0) {
    return fNeuRadPoints;
  } else {
    return NULL;
  }
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::Initialize() {
  FairDetector::Initialize();
  FairRuntimeDb* rtdb = FairRun::Instance()->GetRuntimeDb();
  ERNeuRadGeoPar* par = (ERNeuRadGeoPar*)(rtdb->getContainer("ERNeuRadGeoPar"));
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(FATAL) << "IO manager is not set" << FairLogger::endl;
  }
  ioman->Register("NeuRadPoint", "NeuRad", fNeuRadPoints, kTRUE);
  ioman->Register("NeuRadFirstStep", "NeuRad", fNeuRadFirstStep, kTRUE);
  ioman->Register("NeuRadStep", "NeuRad", fNeuRadSteps, kTRUE);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERNeuRad::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation
  static const Double_t dP = 1.032;
  static const Double_t BirkC1 = 0.013/dP;
  static const Double_t BirkC2 = 9.6e-6/(dP*dP);

  /** Track information to be stored until the track leaves the
      active volume. **/

  if (gMC->IsTrackEntering()) { // Return true if this is the first step of the track in the current volume
    StartNewPoint();
    if (fNeuRadFirstStep->GetEntriesFast() == 0)
      AddFirstStep();
  }

  if (fStorePrimarySteps && fMot0TrackID == -1 && fNeuRadSteps->GetEntriesFast() == 0) {
    ERNeuRadStep* step = AddStep();
      if (fVerbose > 2)
        step->Print();
  }

  fELoss += gMC->Edep(); // GeV // Return the energy lost in the current step
  fStepNb++;

  if (fStoreAllSteps) {
    ERNeuRadStep* step = AddStep();
    if (fVerbose > 2)
      step->Print();
  }
  Double_t curLightYield = 0.;
  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  // Correction for all charge states
  if (gMC->TrackCharge() != 0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0.;
    // Apply correction for higher charge states
    if (TMath::Abs(gMC->TrackCharge()) >= 2) {
      BirkC1Mod = BirkC1 * 7.2 / 12.6;
    } else {
      BirkC1Mod = BirkC1;
    }

    if (gMC->TrackStep() > 0) {
      Double_t dedxcm = gMC->Edep() * 1000. / gMC->TrackStep(); // [MeV/cm]
      curLightYield = gMC->Edep() * 1000. / (1. + BirkC1Mod*dedxcm + BirkC2*dedxcm*dedxcm); //[MeV]
      curLightYield /= 1000.; // [GeV]
      fLightYield += curLightYield;
    }
  }

  if (gMC->IsTrackExiting() || //true if this is the last step of the track in the current volume
      gMC->IsTrackStop()    || //true if the track energy has fallen below the threshold
      gMC->IsTrackDisappeared()) {
    FinishNewPoint();
  }

  // Set the limit on the length of a point
  if (CurPointLen(fPosIn) > 4.) {
    FinishNewPoint();
    StartNewPoint();
  }

  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::EndOfEvent() {
  if (fVerbose > 1) {
    Print();
  }

  FairRunSim* run = FairRunSim::Instance();
  ERMCEventHeader* header = (ERMCEventHeader*)run->GetMCEventHeader();
  header->SetNeuRadEloss(fFullEnergy);
  header->SetNeuRadLY(fFullLY);

  Reset();
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::Print(Option_t *option) const {
  for (Int_t i_point = 0; i_point < fNeuRadPoints->GetEntriesFast(); i_point++) {
    ERNeuRadPoint* point = (ERNeuRadPoint*)fNeuRadPoints->At(i_point);
    point->Print();
  }
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::Reset() {
  fNeuRadPoints->Clear();
  fNeuRadSteps->Clear();
  fNeuRadFirstStep->Clear();
  fFullEnergy = 0.;
  fFullLY = 0.;
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "NeuRad: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERNeuRadPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (ERNeuRadPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[cl2->GetEntriesFast()]) ERNeuRadPoint(*oldpoint);
  }
  LOG(DEBUG) << "NeuRad: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERNeuRad::CheckIfSensitive(std::string name) {
  TString volName = name;
  if(volName.Contains("fiber") /*&& !volName.Contains("dead") */) {
    return kTRUE;
  }
  return kFALSE;
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::StartNewPoint() {

  fELoss = 0.;
  fLightYield = 0.;
  fStepNb = 0;
  fEventID = gMC->CurrentEvent();

  // Get in position, in momentum and in time from the MC engine
  gMC->TrackPosition(fPosIn);
  gMC->TrackMomentum(fMomIn);
  fTimeIn = gMC->TrackTime() * 1.0e09; // current time of flight of the track being transported

  fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
  fTrackLength = gMC->TrackLength(); // length of the current track from its origin (in cm)
  fMot0TrackID = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2

  ObtainChIdfromGMC();

  //Int_t curVolId;
  //Int_t corOffVolId;

  /*if(!(TString(gMC->CurrentVolOffName(1)).Contains("dead") &&
       TString(gMC->CurrentVolOffName(2)).Contains("pixel"))) {
    LOG(FATAL) << "Old version of geometry structure is used" << FairLogger::endl;
  }*/

  //TODO Bullshit!!!!!
  //curVolId = gMC->CurrentVolOffID(1, fFiberNb);
  //corOffVolId = gMC->CurrentVolOffID(2, fPixelNb);
  //corOffVolId = gMC->CurrentVolOffID(3, fModuleNb);

  //TODO check!!!
  // Пересчитываем номер пикселя если введены субмодули
/*
  if (TString(gMC->CurrentVolOffName(3)).Contains("submodul"))
  {
    Int_t pixel_in_submodule_X = 4;
    Int_t pixel_in_submodule_Y = 4;
    Int_t submodule_in_module_X = 2;
    Int_t submodule_in_module_Y = 2;
    
    Int_t pixel_row = fPixelNb / pixel_in_submodule_X;
    Int_t pixel_col = fPixelNb % pixel_in_submodule_X;
    Int_t subm_row = fModuleNb / submodule_in_module_X;
    Int_t subm_col = fModuleNb % submodule_in_module_X;

    fPixelNb = subm_row * submodule_in_module_X * pixel_in_submodule_X * pixel_in_submodule_Y
                + pixel_row * submodule_in_module_X * pixel_in_submodule_X
                + subm_col * pixel_in_submodule_X + pixel_col;

    corOffVolId = gMC->CurrentVolOffID(4, fModuleNb);
  }
*/
  
  Double_t globalPos[3];
  Double_t localPos[3];
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos, localPos);
  fPosInLocal.SetXYZ(localPos[0], localPos[1], localPos[2]);
}

void ERNeuRad::ObtainChIdfromGMC() {

  // Get full path to the current node
  TString curGeoPath;
  curGeoPath = gMC->CurrentVolPath();
  //LOG(DEBUG) << curGeoPath << FairLogger::endl;

  // Parse it into /-separated tokens
  TObjArray* tokens = curGeoPath.Tokenize("/");
  //tokens->Print();

  Int_t fiberID = -1;
  Int_t claddingID = -1;
  Int_t pixelID = -1;
  Int_t submoduleID = -1;
  Int_t moduleID = -1;

  // You may want to implement some checks of the geometry structure here

  // Loop over tokens and extract indices
  TIterator* tokensIter = tokens->MakeIterator(kIterBackward);

  while (TObjString* curNodeNameObj = (TObjString*)tokensIter->Next()) {
    TString curNodeName = curNodeNameObj->GetString();
    Ssiz_t underscorePos = curNodeName.Last('_');
    TSubString indexStr = curNodeName(underscorePos+1, 10);
    Int_t index = atoi(indexStr.Data()) - 1; //!!!!!!!!!!!!!!!!!!!

    if (curNodeName.Contains("fiber", TString::kIgnoreCase)) {
      fiberID = index;
    } else if (curNodeName.Contains("cladding", TString::kIgnoreCase)) {
      claddingID = index;
    } else if (curNodeName.Contains("pixel", TString::kIgnoreCase)) {
      pixelID = index;
    } else if (curNodeName.Contains("submodul", TString::kIgnoreCase)) {
      submoduleID = index;
    } else if (curNodeName.Contains("module", TString::kIgnoreCase)) {
      moduleID = index;
    } else {
      //
    }
  }


  //TODO ??? Ask ERNeuRadSetup object to obtain total number of pixels/fibers/submodules/etc...
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  // The problem is that currently setup does not know anythng about submodules...
  Int_t nSubmodulesInModuleX = 2; // Only in one direction!
  Int_t nPixelsInSubmoduleX = 4; // Only in one direction!
  //Int_t nFibersInPixelX = 2; // Only in one direction!

  Int_t xSubmodule = submoduleID % nSubmodulesInModuleX;
  Int_t ySubmodule = submoduleID / nSubmodulesInModuleX;
  Int_t xPixel = pixelID % nPixelsInSubmoduleX;
  Int_t yPixel = pixelID / nPixelsInSubmoduleX;
  //LOG(DEBUG) << "xSubmodule=" << xSubmodule << "\tySubmodule=" << ySubmodule
  //     << "\txPixel=" << xPixel << "\tyPixel=" << yPixel
  //     << FairLogger::endl;

  Int_t index1 = xPixel + yPixel * nPixelsInSubmoduleX * nSubmodulesInModuleX;
  Int_t index2 = index1 + xSubmodule * nPixelsInSubmoduleX 
                        + ySubmodule * nPixelsInSubmoduleX * (nPixelsInSubmoduleX * nSubmodulesInModuleX);

  /*LOG(DEBUG) << "Indices: " << "fiber =" << fiberID << "\t"
                            << "cladd =" << claddingID << "\t"
                            << "pixel =" << pixelID << "\t"
                            << "submod=" << submoduleID << "\t"
                            << "module=" << moduleID << "\t"
                            << "index1 =" << index1 << "\t"
                            << "index2 =" << index2
                            << FairLogger::endl;*/


  // Compute resulting PMT ID and channel ID

  fPmtId = moduleID;
  fChId = index2;
}

//-------------------------------------------------------------------------------------------------
void ERNeuRad::FinishNewPoint() {

  // Get out position, out momentum and out time from the MC engine
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09; // convert from seconds to nanoseconds

  if (fELoss > 0.) {
    AddPoint();
    fFullEnergy += fELoss;
    fFullLY += fLightYield;
  }
}

//-------------------------------------------------------------------------------------------------
ERNeuRadPoint* ERNeuRad::AddPoint() {
  TClonesArray& clref = *fNeuRadPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERNeuRadPoint(fEventID,
                                        fTrackID,
                                        fMot0TrackID,

                                        //fFiberNb,
                                        //fPixelNb,
                                        //fModuleNb,
                                        fPmtId,
                                        fChId,

                                        fMass,
                                        fPosIn.Vect(),
                                        fPosInLocal,
                                        fPosOut.Vect(),
                                        fMomIn.Vect(),
                                        fMomOut.Vect(),
                                        fTimeIn,  //TODO convert from seconds to nanoseconds?! Already converted here?
                                        fTimeOut,  //TODO convert from seconds to nanoseconds?! Already converted here?
                                        fTrackLength,
                                        fELoss,
                                        fLightYield,
                                        gMC->TrackPid(),
                                        gMC->TrackCharge());
}

//-------------------------------------------------------------------------------------------------
ERNeuRadStep* ERNeuRad::AddFirstStep() {
  TClonesArray& clref = *fNeuRadFirstStep;
  fTrackStatus = ERNeuRadStep::GetTrackStatus();
  gMC->StepProcesses(fProcessesID);
  gMC->TrackPosition(fCurPosIn);
  gMC->TrackMomentum(fCurMomIn);
  return new(clref[0])
    ERNeuRadStep(fEventID,
                  fStepNb,
                  fTrackID,
                  fMot0TrackID,

                  //fFiberNb,
                  //fPixelNb,
                  //fModuleNb,
                  fPmtId,
                  fChId,

                  fCurPosIn.Vect(),
                  fCurMomIn.Vect(),
                  gMC->TrackTime() * 1.0e09, // convert from seconds to nanoseconds
                  gMC->TrackStep(),
                  gMC->TrackPid(),
                  gMC->TrackMass(),
                  fTrackStatus,
                  gMC->Edep(),
                  gMC->TrackCharge(),
                  fProcessesID);
}

//-------------------------------------------------------------------------------------------------
ERNeuRadStep* ERNeuRad::AddStep() {
  TClonesArray& clref = *fNeuRadSteps;
  fTrackStatus = ERNeuRadStep::GetTrackStatus();
  gMC->StepProcesses(fProcessesID);
  gMC->TrackPosition(fCurPosIn);
  gMC->TrackMomentum(fCurMomIn);
  return new(clref[fNeuRadSteps->GetEntriesFast()])
    ERNeuRadStep(fEventID,
                  fStepNb,
                  fTrackID,
                  fMot0TrackID,

                  //fFiberNb,
                  //fPixelNb,
                  //fModuleNb,
                  fPmtId,
                  fChId,

                  fCurPosIn.Vect(),
                  fCurMomIn.Vect(),
                  gMC->TrackTime() * 1.0e09, // convert from seconds to nanoseconds
                  gMC->TrackStep(),
                  gMC->TrackPid(),
                  gMC->TrackMass(),
                  fTrackStatus,
                  gMC->Edep(),
                  gMC->TrackCharge(),
                  fProcessesID);
}

//-------------------------------------------------------------------------------------------------
Double_t ERNeuRad::CurPointLen(TLorentzVector& posIn) {
  TLorentzVector posOut;
  gMC->TrackPosition(posOut);
  return TMath::Sqrt((posIn.X() - posOut.X())*(posIn.X() - posOut.X()) +
                    (posIn.Y() - posOut.Y())*(posIn.Y() - posOut.Y()) +
                    (posIn.Z() - posOut.Z())*(posIn.Z() - posOut.Z()));
}

//-------------------------------------------------------------------------------------------------
ClassImp(ERNeuRad)
