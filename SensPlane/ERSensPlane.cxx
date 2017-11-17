/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSensPlane.h"

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TParticle.h"
#include "TString.h"
#include "TVirtualMC.h"
#include "TGeoMatrix.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "ERSensPlanePoint.h"

// -----   Default constructor   ------------------------------------------------
ERSensPlane::ERSensPlane()
  : FairDetector("ERSensPlane", kTRUE)
{
  fFrontPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fBackPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fPositionRotation = new TGeoCombiTrans("ERSensPlanePosRot", 0., 0., 0.,
                      new TGeoRotation("ERSensPlaneRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Standard constructor   -----------------------------------------------
ERSensPlane::ERSensPlane(const char* name, Bool_t active)
  : FairDetector(name, active)
{
  fFrontPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fBackPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fPositionRotation = new TGeoCombiTrans("ERSensPlanePosRot", 0., 0., 0.,
                      new TGeoRotation("ERSensPlaneRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Destructor   ---------------------------------------------------------
ERSensPlane::~ERSensPlane()
{
  if (fFrontPoints) {
    fFrontPoints->Delete();
    delete fFrontPoints;
  }
  if (fBackPoints) {
    fBackPoints->Delete();
    delete fBackPoints;
  }
  if (fPositionRotation) {
    delete fPositionRotation;
  }
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERSensPlane::SetDetectorPosition(Double_t x, Double_t y, Double_t z)
{
  fPositionRotation->SetDx(x);
  fPositionRotation->SetDy(y);
  fPositionRotation->SetDz(z);
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERSensPlane::Initialize()
{
  FairDetector::Initialize(); //TODO needed?
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
Bool_t ERSensPlane::ProcessHits(FairVolume* vol)
{
  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Double_t       mass;              //!  mass
  static TLorentzVector posIn, posOut;     //!  position
  static TLorentzVector momIn, momOut;     //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss

  //LOG(INFO) << "ERSensPlane::ProcessHits" << FairLogger::endl;

  if (gMC->IsTrackEntering()) {
    // Return true if this is the first step of the track in the current volume
    eLoss = 0.;
    eventID = gMC->CurrentEvent();
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID = gMC->GetStack()->GetCurrentTrackNumber();
    time = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    //LOG(INFO) << "Entering x=" << posIn.X() << " y=" << posIn.Y() << " z=" << posIn.Z() << FairLogger::endl;
  }

  eLoss += gMC->Edep(); // GeV // Return the energy lost in the current step

  if (gMC->IsTrackExiting()    || // Return true if this is the last step of the track in the current volume 
      gMC->IsTrackStop()       || // Return true if the track energy has fallen below the thresho
      gMC->IsTrackDisappeared())
  {
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    //LOG(INFO) << "Exiting x=" << posOut.X() << " y=" << posOut.Y() << " z=" << posOut.Z() << FairLogger::endl;

    if (momOut.Pz() > 0.)
    {
      AddFrontPoint (eventID,
                     trackID,
                     mot0TrackID,
                     mass,
                     TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                     TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                     TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                     TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                     time,
                     length,
                     eLoss);
    } else{
      AddBackPoint (eventID,
                    trackID,
                    mot0TrackID,
                    mass,
                    TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                    TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                    TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                    TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                    time,
                    length,
                    eLoss);
    } 
  }

  return kTRUE;
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERSensPlane::BeginEvent() {
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERSensPlane::EndOfEvent() {
  Print();
  Reset();
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERSensPlane::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) Fatal("Init", "IO manager is not set");
  ioman->Register("ERSensPlaneFrontPoint", "ERSensPlane", fFrontPoints, kTRUE);
  ioman->Register("ERSensPlaneBackPoint", "ERSensPlane", fBackPoints, kTRUE);
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
TClonesArray* ERSensPlane::GetCollection(Int_t iColl) const {
  if (iColl == 0) {
    return fFrontPoints;
  } else if (iColl == 1) {
    return fBackPoints;
  } else {
    return NULL;
  }
}
// ------------------------------------------------------------------------------

// -----   Public method Print   ------------------------------------------------
void ERSensPlane::Print(Option_t *option) const {
}
// ------------------------------------------------------------------------------

// -----   Public method Reset   ------------------------------------------------
void ERSensPlane::Reset() {
//  LOG(INFO) << "ERSensPlane::Reset()" << FairLogger::endl;
  fFrontPoints->Clear();
  fBackPoints->Clear();
}
// ------------------------------------------------------------------------------

// -----   Public method CopyClones   -------------------------------------------
void ERSensPlane::CopyClones(TClonesArray* cl1,
                              TClonesArray* cl2,
                              Int_t offset) {
}
// ------------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ------------------------------------
void ERSensPlane::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if (fileName == "") {
    LOG(FATAL) << "ERSensPlane geometry file name is not set." << FairLogger::endl;
  } else if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing ERSensPlane geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    //TODO Uncomment when the corresponding method is implemented in FairRoot
    ConstructRootGeometry(/*(TGeoMatrix*)(fPositionRotation)*/);
  } else if(fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing ERSensPlane geometry from GDML file " << fileName.Data() << FairLogger::endl;
    ConstructGDMLGeometry(fPositionRotation);
  } else {
    LOG(FATAL) << "ERSensPlane geometry file name is not correct." << FairLogger::endl;
  }
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
Bool_t ERSensPlane::CheckIfSensitive(std::string name)
{
  //LOG(INFO) << "ERSensPlane::CheckIfSensitive for " << name << FairLogger::endl;
  return kTRUE;
}
// ------------------------------------------------------------------------------

// -----   Private method AddFrontPoint   ---------------------------------------
ERSensPlanePoint* ERSensPlane::AddFrontPoint(Int_t eventID,
                                             Int_t trackID,
                                             Int_t mot0trackID,
                                             Double_t mass,
                                             TVector3 posIn,
                                             TVector3 posOut,
                                             TVector3 momIn,
                                             TVector3 momOut,
                                             Double_t time,
                                             Double_t length,
                                             Double_t eLoss)
{
  //LOG(INFO) << "ERSensPlane::AddFrontPoint eventID=" << eventID << FairLogger::endl;
  TClonesArray& clref = *fFrontPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERSensPlanePoint(eventID,
                                           trackID,
                                           mot0trackID,
                                           mass,
                                           posIn,
                                           posOut,
                                           momIn,
                                           momOut,
                                           time,
                                           length,
                                           eLoss);
}
// ------------------------------------------------------------------------------

// -----   Private method AddFrontPoint   ---------------------------------------
ERSensPlanePoint* ERSensPlane::AddBackPoint(Int_t eventID,
                                            Int_t trackID,
                                            Int_t mot0trackID,
                                            Double_t mass,
                                            TVector3 posIn,
                                            TVector3 posOut,
                                            TVector3 momIn,
                                            TVector3 momOut,
                                            Double_t time,
                                            Double_t length,
                                            Double_t eLoss)
{
  //LOG(INFO) << "ERSensPlane::AddFrontPoint eventID=" << eventID << FairLogger::endl;
  TClonesArray& clref = *fBackPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERSensPlanePoint(eventID,
                                           trackID,
                                           mot0trackID,
                                           mass,
                                           posIn,
                                           posOut,
                                           momIn,
                                           momOut,
                                           time,
                                           length,
                                           eLoss);
}
// ------------------------------------------------------------------------------

ClassImp(ERSensPlane)
