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
  fPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fPositionRotation = new TGeoCombiTrans("ERSensPlanePosRot", 0., 0., 0.,
                      new TGeoRotation("ERSensPlaneRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Standard constructor   -----------------------------------------------
ERSensPlane::ERSensPlane(const char* name, Bool_t active) 
  : FairDetector(name, active)
{
  fPoints = new TClonesArray("ERSensPlanePoint", 1000);
  fPositionRotation = new TGeoCombiTrans("ERSensPlanePosRot", 0., 0., 0.,
                      new TGeoRotation("ERSensPlaneRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Destructor   ---------------------------------------------------------
ERSensPlane::~ERSensPlane()
{
  if (fPoints) {
    fPoints->Delete();
    delete fPoints;
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
  }

  eLoss += gMC->Edep(); // GeV // Return the energy lost in the current step

  if (gMC->IsTrackExiting()    || // Return true if this is the last step of the track in the current volume 
      gMC->IsTrackStop()       || // Return true if the track energy has fallen below the thresho
      gMC->IsTrackDisappeared()) 
  { 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    //if (eLoss > 0.) // TODO tune
    {
      AddPoint (eventID,
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
  ioman->Register("ERSensPlanePoint", "ERSensPlane", fPoints, kTRUE);
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
TClonesArray* ERSensPlane::GetCollection(Int_t iColl) const {
  if (iColl == 0) {
    return fPoints;
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
  fPoints->Clear();
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

// -----   Private method AddPoint   --------------------------------------------
ERSensPlanePoint* ERSensPlane::AddPoint(Int_t eventID,
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
  //LOG(INFO) << "ERSensPlane::AddPoint eventID=" << eventID << FairLogger::endl;
  TClonesArray& clref = *fPoints;
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
