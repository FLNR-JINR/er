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
#include "TSystem.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairLogger.h"
#include "FairGeoLoader.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"

#include "ERPoint.h"

// ------------------------------------------------------------------------------
ERSensPlane::ERSensPlane()
  : FairDetector("ERSensPlane", kTRUE) {
}
// ------------------------------------------------------------------------------
ERSensPlane::~ERSensPlane()
{
  if (fPoints) {
    fPoints->Delete();
    delete fPoints;
  }
}
// ------------------------------------------------------------------------------
void ERSensPlane::SetDetectorPositioning(float x, float y, float z, 
                                         float x_rotation, float y_rotation,
                                         float z_rotation) {
  fPositionRotation.SetDx(x);
  fPositionRotation.SetDy(y);
  fPositionRotation.SetDz(z);
  fPositionRotation.RotateX(x_rotation);
  fPositionRotation.RotateY(y_rotation);
  fPositionRotation.RotateZ(z_rotation);
}
// ------------------------------------------------------------------------------
void ERSensPlane::SetDetectorGeometry(float half_x, float half_y, float half_z, 
                                      const TString& material) {
  fGeometry = {half_x, half_y, half_z, material};
}
// ------------------------------------------------------------------------------
TVector3 GlobalToLocal(const TVector3& global) {
  TGeoHMatrix matrix;
  gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
  Double_t globalPos[3],localPos[3];
  global.GetXYZ(globalPos);
  matrix.MasterToLocal(globalPos,localPos);
  TVector3 local;
  local.SetXYZ(localPos[0],localPos[1],localPos[2]);
  return local;
}
// ------------------------------------------------------------------------------
Bool_t ERSensPlane::ProcessHits(FairVolume* vol)
{
  static int trackID, mot0TrackID;
  static double mass, timeIn, timeOut, length, eLoss;             
  static TLorentzVector posIn, posOut; 
  static TLorentzVector momIn, momOut;

  if (gMC->IsTrackEntering()) {
    // Return true if this is the first step of the track in the current volume
    eLoss = 0.;
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID = gMC->GetStack()->GetCurrentTrackNumber();
    timeIn = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
  }

  eLoss += gMC->Edep() * 1e3; // MeV // Return the energy lost in the current step

  if (gMC->IsTrackExiting()    || // Return true if this is the last step of the track in the current volume 
      gMC->IsTrackStop()       || // Return true if the track energy has fallen below the thresho
      gMC->IsTrackDisappeared()) 
  { 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    timeOut = gMC->TrackTime() * 1.0e09; 
    AddPoint(gMC->CurrentEvent(), trackID, mot0TrackID, mass, posIn.Vect(), GlobalToLocal(posIn.Vect()),
             posOut.Vect(), momIn.Vect(), momOut.Vect(), timeIn, timeOut, length, eLoss,
             gMC->TrackPid(), gMC->TrackCharge());
  }

  return kTRUE;
}
// ------------------------------------------------------------------------------
void ERSensPlane::EndOfEvent() {
  Reset();
}
// ------------------------------------------------------------------------------
void ERSensPlane::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) Fatal("Init", "IO manager is not set");
  fPoints = new TClonesArray("ERPoint", 1000);
  ioman->Register("ERPoint", "ERSensPlane", fPoints, kTRUE);
}
// ------------------------------------------------------------------------------
TClonesArray* ERSensPlane::GetCollection(int iColl) const {
  if (iColl == 0) {
    return fPoints;
  } else {
    return nullptr;
  }
}
// ------------------------------------------------------------------------------
void ERSensPlane::Reset() {
  fPoints->Clear();
}
// ------------------------------------------------------------------------------
void ERSensPlane::ConstructGeometry() {
  FairGeoLoader*    geoLoad = FairGeoLoader::Instance();
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  TString geoFileName = geoPath + "/geometry/sensplane.temp.root";
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
  auto* fair_medium = geoMedia->getMedium(fGeometry.material);
  if (!fair_medium)
    LOG(FATAL) << "FAIR medium " << fGeometry.material << " not found\n";
  geoBuild->createMedium(fair_medium);
  auto* medium = gGeoManager->GetMedium(fGeometry.material);
  if (!medium)
    LOG(FATAL) << "Medium " << fGeometry.material << " not found\n";
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  TGeoVolume* sens_plane = new TGeoVolumeAssembly("SensPlane");
  top->AddNode(sens_plane, 0, &fPositionRotation);
  auto* sens_box = gGeoManager->MakeBox("SensPlaneBox", medium, fGeometry.half_x,
                                        fGeometry.half_y, fGeometry.half_z);
  sens_plane->AddNode(sens_box, 0);
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  SetGeometryFileName(geoFileName);
  ConstructRootGeometry();
}
// ------------------------------------------------------------------------------
Bool_t ERSensPlane::CheckIfSensitive(std::string name) {
  return kTRUE;
}
// ------------------------------------------------------------------------------
ERPoint* ERSensPlane::AddPoint(int eventID, int trackID, int mot0trackID,
                               double mass, const TVector3& posIn, const TVector3& posInLoc,
                               const TVector3& posOut, const TVector3& momIn,
                               const TVector3& momOut, double timeIn, double timeOut,
                               double length, double eLoss, int pid, double charge) {
  TClonesArray& clref = *fPoints;
  int size = clref.GetEntriesFast();
  return new(clref[size]) ERPoint(eventID, trackID, mot0trackID, 0., // Volume
                                  mass, posIn, posInLoc, posOut, momIn, momOut,
                                  timeIn, timeOut, length, eLoss, 0., // Light yield
                                  pid, charge);
}
// ------------------------------------------------------------------------------

ClassImp(ERSensPlane)
