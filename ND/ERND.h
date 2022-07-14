/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERND_H
#define ERND_H

#include <set>

#include "ERDetector.h"
#include "ERNDPoint.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERND : public FairDetector {
public:
  /** Default constructor **/
  ERND();
  /** Standard constructor.
   *@param name    ERND ERND name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event,
     3 - GEANT Step information **/
  ERND(const char* name, Bool_t active, Int_t verbose);
  /** Destructor **/
  virtual ~ERND();
  /** @brief Set propagation step in sensetive volumes.
   ** Special process stepLimiter must be in TG4RunConfiguration **/
  void SetMaxStep(Double_t step) {fStep = step;}

  void SetCandidatesForParentPDG(const std::set<Int_t>& pdgs) {fCandidatesForParentPdgs = pdgs;}

  /** Virtual method ProcessHits
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERNDPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  /** Virtual method EndOfEvent
   ** If verbosity level is set, print point collection at the
   ** end of the event. **/
  virtual void EndOfEvent();
  /// Registers the point collection in the ROOT manager.
  virtual void Register();
  /// Accessor to the point collection
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  /// Screen output of hit collection
  virtual void Print(Option_t* option="") const;
  /// Clears the point collection
  virtual void Reset();
  /** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);
  /// Constructs the ERND geometry
  virtual void ConstructGeometry();  
  /// Initialize ERND data
  virtual void Initialize();
  /** Virtaul method CheckIfSensitive 
	 **Check whether a volume is sensitive.
   ** @param(name)  Volume name
   ** @value        kTRUE if volume is sensitive, else kFALSE
   ** The decision is based on the volume name. **/
  virtual Bool_t CheckIfSensitive(std::string name);
private:
  TClonesArray* fNDPoints = nullptr; //!  The point collection
  TClonesArray*  fMCTracks = nullptr;
  Double_t fStep = 1.; //! Max lengt of step of track propagetion in sensetive volume
  std::set<Int_t> fCandidatesForParentPdgs = {22, 2112};
private:
  void FindParentParticle(int track_id, int& parentTrackId, int& parentPdg);
  /// Adds a NeuRadPoint to the Point Collection
  ERNDPoint* AddPoint(
    Int_t eventID, Int_t trackID,
    Int_t mot0trackID,
    Int_t pdg,
    TVector3 posIn,
    TVector3 pos_out, TVector3 momIn,
    TVector3 momOut, Double_t time,
    Double_t length, Double_t eLoss, 
    Int_t stilbenNr,Float_t lightYield,
    Int_t parentTrackId, Int_t parentPdg
  );
  ClassDef(ERND,1);
};

#endif 
