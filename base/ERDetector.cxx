/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDetector.h"

#include "TGeoMatrix.h"
#include "TVirtualMC.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "ERDetectorGeoPar.h"
//-------------------------------------------------------------------------------------------------
ERDetector::ERDetector()
: FairDetector("ERDetector", kTRUE, -1)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
ERDetector::ERDetector(const char* Name, Bool_t Active, Int_t DetId/*=0*/)
: FairDetector(Name, Active, DetId)
{
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//-------------------------------------------------------------------------------------------------
void ERDetector::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  }
  else if (fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing geometry from GDML file " << fileName.Data() << FairLogger::endl;
    TGeoRotation *zeroRotation = new TGeoRotation();
    zeroRotation->RotateX(0.);
    zeroRotation->RotateY(0.);
    zeroRotation->RotateZ(0.);
    ConstructGDMLGeometry(new TGeoCombiTrans(.0,.0,.0, zeroRotation));
  }
  else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
}
//-------------------------------------------------------------------------------------------------
void ERDetector::AddSensetive(TString name){
  fSenNames.push_back(name);
}
//-------------------------------------------------------------------------------------------------
TClonesArray* ERDetector::GetCollection(Int_t iColl) const {
  if (fSenVolumes.size() > iColl){
    auto it = fSenVolumes.begin();
    for (Int_t i = 0; i<iColl; i++){it++;}
    return it->second;
  }
  else {
    return NULL;
  }
}
//-------------------------------------------------------------------------------------------------
void ERDetector::Initialize(){
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  ERDetectorGeoPar* par=(ERDetectorGeoPar*)(rtdb->getContainer("ERDetectorGeoPar"));
}
//-------------------------------------------------------------------------------------------------
void ERDetector::Register(){
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
    LOG(FATAL) << "IO manager is not set" << FairLogger::endl;
  
  for(const auto &itSen: fSenVolumes){
    ioman->Register(fName+itSen.first+TString("Point"),fName, itSen.second, kTRUE);
  }
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDetector::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation
  static const Double_t dP = 1.032 ;
  static const Double_t BirkC1 =  0.013/dP;
  static const Double_t BirkC2 =  9.6e-6/(dP * dP);
  
  /** Track information to be stored until the track leaves the
      active volume. **/
  
  if (gMC->IsTrackEntering()) { // Return true if this is the first step of the track in the current volume
    StartNewPoint();
  }
  
  fELoss += gMC->Edep(); // GeV //Return the energy lost in the current step

  Double_t curLightYield = 0.;
  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  // Correction for all charge states
  if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0;
    // Apply correction for higher charge states
    if (TMath::Abs(gMC->TrackCharge())>=2)
      BirkC1Mod=BirkC1*7.2/12.6;
    else
      BirkC1Mod=BirkC1;

    if (gMC->TrackStep()>0){
      Double_t dedxcm=gMC->Edep()*1000./gMC->TrackStep(); //[MeV/cm]
      curLightYield=gMC->Edep()*1000./(1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
      curLightYield /= 1000.; //[GeV]
      fLightYield+=curLightYield;
    }
  }
  
  if (gMC->IsTrackExiting() || //true if this is the last step of the track in the current volume 
      gMC->IsTrackStop()    || //true if the track energy has fallen below the threshold
      gMC->IsTrackDisappeared()) { 
    FinishNewPoint();
  }
  
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
void ERDetector::EndOfEvent() {
  if (fVerbose > 1){
    Print();
  }
  Reset();
}
//-------------------------------------------------------------------------------------------------
void ERDetector::Print(Option_t *option) const {
  for(const auto &itSen: fSenVolumes){
    TClonesArray* points = itSen.second; 
    for (Int_t i_point = 0; i_point < points->GetEntriesFast(); i_point++) {
      ERPoint* point = (ERPoint*)points->At(i_point);
      point->Print();
    }
  }
}
//-------------------------------------------------------------------------------------------------
void ERDetector::Reset() {
  for(const auto &itSen: fSenVolumes){
    TClonesArray* points = itSen.second;
    points->Clear();
  }
  fFullEnergy = 0.;
  fFullLY = 0.;
}
//-------------------------------------------------------------------------------------------------
void ERDetector::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "ERDetector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERPoint(*oldpoint);
  }
  LOG(DEBUG) << "ERDetector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
  
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDetector::CheckIfSensitive(std::string name) {
  TString curVolName = name;
  for(const auto &volNameSubsting: fSenNames){
    if(curVolName.Contains(volNameSubsting)){
      fSenVolumes[curVolName] = new TClonesArray("ERPoint");
      return kTRUE;
    }
  }
  return kFALSE;
}
//-------------------------------------------------------------------------------------------------
void ERDetector::StartNewPoint() {
  fELoss  = 0.;
  fLightYield = 0.;
  fEventID = gMC->CurrentEvent();
  gMC->TrackPosition(fPosIn);
  gMC->TrackMomentum(fMomIn);
  fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
  fTimeIn   = gMC->TrackTime() * 1.0e09;  //current time of flight of the track being transported
  fTrackLength = gMC->TrackLength(); //length of the current track from its origin (in cm)
  fMot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
  fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  Int_t curVolId, corOffVolId;
  curVolId = gMC->CurrentVolID(fVolNb);
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  fPosIn.Vect().GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  fPosInLocal.SetXYZ(localPos[0],localPos[1],localPos[2]);
}
//-------------------------------------------------------------------------------------------------
void ERDetector::FinishNewPoint() {
  gMC->TrackPosition(fPosOut);
  gMC->TrackMomentum(fMomOut);
  fTimeOut = gMC->TrackTime() * 1.0e09; 
  
  if (fELoss > 0.) {
    TClonesArray* points = fSenVolumes[gMC->CurrentVolName()];
    AddPoint(points);
    fFullEnergy+=fELoss;
    fFullLY+=fLightYield;

  }                
}
//-------------------------------------------------------------------------------------------------
ERPoint* ERDetector::AddPoint(TClonesArray* points) {
  TClonesArray& clref = *points;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERPoint(fEventID, fTrackID, fMot0TrackID, fVolNb,
    fMass, fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,
    fTimeOut,fTrackLength, fELoss, fLightYield,gMC->TrackPid(), gMC->TrackCharge());
}

ClassImp(ERDetector)