/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERSENSPLANE_H
#define ERSENSPLANE_H

#include "FairDetector.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TString.h"

class TClonesArray;
class ERPoint;

class ERSensPlane : public FairDetector {
public:
  struct SensPlaneGeometry {
    float half_x;
    float half_y;
    float half_z;
    TString material;
  };
  ERSensPlane();
  virtual ~ERSensPlane();

  void SetDetectorPositioning(float x, float y, float z, 
                              float x_rotation = 0., float y_rotation = 0.,
                              float z_rotation = 0.);
  void SetDetectorGeometry(float half_x, float half_y, float half_z, 
                           const TString& material="vacuum");

  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  virtual void EndOfEvent();
  virtual void Register();
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  virtual void Reset();
  virtual void ConstructGeometry();
  virtual Bool_t CheckIfSensitive(std::string name);
private:
  TClonesArray* fPoints = nullptr;
  ERPoint* AddPoint(int eventID, int trackID, int mot0trackID,
                    double mass, const TVector3& posIn, const TVector3& posInLoc,
                    const TVector3& posOut, const TVector3& momIn,
                    const TVector3& momOut, double timeIn, double timeOut,
                    double length, double eLoss, int pid, double charge);
  TGeoCombiTrans fPositionRotation;
  SensPlaneGeometry fGeometry = {0.5, 0.5, 0.1, "vacuum"};
  ClassDef(ERSensPlane, 1);
};

#endif // ERSENSPLANE_H
