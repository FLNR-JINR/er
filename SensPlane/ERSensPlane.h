/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----           SensPlane detector header file                      -----
// -------------------------------------------------------------------------

#ifndef ERSENSPLANE_H
#define ERSENSPLANE_H

#include "FairDetector.h"

#include "TVector3.h"

class TClonesArray;
class ERSensPlanePoint;
class TGeoCombiTrans;

class ERSensPlane : public FairDetector
{
public:

  /** Default constructor **/
  ERSensPlane();

  /** Standard constructor **/
  ERSensPlane(const char* name="SENSPLANE", Bool_t active=kTRUE);

  /** Destructor **/
  virtual ~ERSensPlane();

  void SetDetectorPosition(Double_t x, Double_t y, Double_t z);

  virtual Bool_t ProcessHits(FairVolume* vol = 0);

  virtual void BeginEvent();

  virtual void EndOfEvent();

  virtual void Register();

  virtual TClonesArray* GetCollection(Int_t iColl) const;

  virtual void Print(Option_t *option="") const;

  virtual void Reset();

  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);

  virtual void ConstructGeometry();

  virtual void Initialize();

  virtual Bool_t CheckIfSensitive(std::string name);

private:

  TClonesArray* fPoints;
  ERSensPlanePoint* AddPoint(Int_t eventID,
                             Int_t trackID,
                             Int_t mot0trackID,
                             Double_t mass,
                             TVector3 posIn,
                             TVector3 pos_out,
                             TVector3 momIn,
                             TVector3 momOut,
                             Double_t time,
                             Double_t length,
                             Double_t eLoss);

  TGeoCombiTrans* fPositionRotation;

  ClassDef(ERSensPlane, 1);
};

#endif // ERSENSPLANE_H
