/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescope_H
#define ERQTelescope_H

#include "ERDetector.h"
#include "ERQTelescopeSiPoint.h"
#include "ERQTelescopeCsIPoint.h"
#include "ERQTelescopeSetup.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERQTelescope : public ERDetector {
  using BranchName = TString;
  using ComponentPoints = std::map<BranchName, TClonesArray*>;
  using PointsPerComponent = std::map<TString, ComponentPoints>;
public:
  /** Default constructor **/
  ERQTelescope();
  /** Standard constructor.
   *@param name    ERQTelescope ERQTelescope name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERQTelescope(const char* name, Bool_t active, Int_t verbose);
  /** Destructor **/
  virtual ~ERQTelescope();
  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERQTelescopePoint and adds it to the
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
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);
   /** Virtaul method Initialize
   **
   ** Initialize ERQTelescope data
   **/
  virtual void Initialize();
  /** @brief Builds geometry and writes it to temporary file
   ** trough parameters from ERBeamDetSetup class object.
   ** Virtual from FairDetector.
  **/
  virtual void ConstructGeometry();
  /** Virtaul method CheckIfSensitive
	**Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name.
  **/
  virtual Bool_t CheckIfSensitive(std::string name);
  /** Virtaul method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
private:
  ERQTelescopeSetup* fQTelescopeSetup;
  PointsPerComponent fPoints;
  Int_t                     fVersion;           //! geometry version
  Int_t                     fEventID;           //!  event index
  Int_t                     fTrackID;           //!  track index
  Int_t                     fMot0TrackID;       //!  0th mother track index
  Double_t                  fMass;              //!  mass
  TLorentzVector            fPosIn, fPosOut;    //!  position
  TLorentzVector            fMomIn, fMomOut;    //!  momentum
  Double32_t                fTime;              //!  time
  Double32_t                fLength;            //!  length
  Double32_t                fEloss;             //!  energy loss
  Int_t                     fSiStationNb;         // Si
  Int_t                     fSiStripNb;
  Int_t                     fCsIBoxNb;           // CsI (0-3)
  Int_t                     fCsIStationNb;
  Int_t                     fPDG;
private:
  /** Private method AddPoint
   **
   ** Adds a telescope to the Point Collection
   **/
  void AddPoint(TClonesArray& clref);
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();

  ClassDef(ERQTelescope, 1);
};

#endif
