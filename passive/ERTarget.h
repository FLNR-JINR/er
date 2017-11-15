/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTarget_H
#define ERTarget_H

#include "ERDetector.h"

class TClonesArray;
class TGeoCombiTrans;

class ERTarget : public ERDetector
{
  static Double_t fThickness;
  Int_t fVersion;
  TClonesArray* fTargetPoints;     //!  The point collection //fix - what?!

  TGeoCombiTrans* fPositionRotation;

public:

  /** Default constructor **/
  ERTarget();

  /** Standard constructor.
   *@param name    ERTarget ERTarget name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERTarget(const char* name, Bool_t active, Int_t verbose);

  /** Destructor **/
  virtual ~ERTarget();

  void SetModulePosition(Double_t x, Double_t y, Double_t z);

  /** Virtual method ProcessHits
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERTargetPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);

  /** Virtual method BeginEvent
   **
   **/
  virtual void BeginEvent();

  /** Virtual method EndOfEvent
   **
   ** If verbosity level is set, print point collection at the
   ** end of the event.
   **/
  virtual void EndOfEvent();

  /** Virtual method Register
   **
   ** Registers the point collection in the ROOT manager.
   **/
  virtual void Register();

  /** Accessor to the point collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;

  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print(Option_t *option="") const;

  /** Virtual method Reset
   **
   ** Clears the point collection
   **/
  virtual void Reset();

  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1,
                          TClonesArray* cl2,
                          Int_t offset);

  /** Virtual method Construct geometry
   **
   ** Constructs the ERTarget geometry
   **/
  virtual void ConstructGeometry();

   /** Virtual method Initialize
   **
   ** Initialize ERTarget data
   **/
  virtual void Initialize();

  /** Virtual method CheckIfSensitive
  ** Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name.
  **/
  virtual Bool_t CheckIfSensitive(std::string name);

  /** Virtual method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }

  static Double_t Thickness() { return fThickness; }

  ClassDef(ERTarget, 1);
};

#endif // ERTarget_H
