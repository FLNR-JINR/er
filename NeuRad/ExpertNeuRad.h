// -------------------------------------------------------------------------
// -----                        ExpertNeuRad header file               -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

/**  ExpertNeuRad.h
 **/


#ifndef ExpertNeuRad_H
#define ExpertNeuRad_H

#include "TLorentzVector.h"
#include <iostream>
#include <map>
#include <TGeoMedium.h>

#include "FairDetector.h"

#include "ExpertNeuRadPoint.h"

using namespace std;

class TClonesArray;
class FairVolume;

class ExpertNeuRad : public FairDetector
{
  
 public:
  
  /** Default constructor **/
  ExpertNeuRad();
  
  
  /** Standard constructor.
   *@param name    ExpertNeuRadetcor name
   *@param active  sensitivity flag
   **/
  ExpertNeuRad(const char* name, Bool_t active);
  
  
  /** Destructor **/
  virtual ~ExpertNeuRad();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a R3BNeuNeuLandPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  
  
  /** Virtual method BeginEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  
  virtual void BeginEvent();
  
  /** Virtual method EndOfEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  
  virtual void EndOfEvent();
  
  
  
  /** Virtual method Register
   **
   ** Registers the hit collection in the ROOT manager.
   **/
  virtual void Register();
  
  
  /** Accessor to the hit collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  
  
  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print(Option_t *option="") const;
  
  
  /** Virtual method Reset
   **
   ** Clears the hit collection
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
  
  
  /** Virtaul method Construct geometry
   **
   ** Constructs the STS geometry
   **/
  virtual void ConstructGeometry();
  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts(){;}
  
  virtual Bool_t CheckIfSensitive(std::string name);
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
  
  //  void SaveGeoParams();
  
 private:
  
  /** Track information to be stored until the track leaves the
      active volume. **/
  Int_t          fEventID;           //!  track index
  Int_t          fTrackID;           //!  track index
  Int_t          fMot0TrackID;       //!  0th mother track index
  Double_t       fMass;              //!  mass
  TLorentzVector fPosIn, fPosOut;    //!  position
  TLorentzVector fMomIn, fMomOut;    //!  momentum
  Double32_t     fTime;              //!  time
  Double32_t     fLength;            //!  length
  Double32_t     fELoss;             //!  energy loss
  Double_t       fLightYield;        //!  light yield
  Int_t          fModuleInBundleNb;   //!  number of module in bundle
  
  Int_t          fPosIndex;          //!
  TClonesArray*  fNeuRadCollection;    //!  The hit collection
  Bool_t         kGeoSaved;          //!
  TList *flGeoPar;                   //!
  Int_t fVersion;                    //! geometry version
  
  
  /** Private method AddHit
   **
   ** Adds a NeuRadPoint to the HitCollection
   **/
  ExpertNeuRadPoint* AddHit(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
        Int_t moduleInBundleNb,
			  Double_t mass,
			  TVector3 posIn,
			  TVector3 pos_out, TVector3 momIn,
			  TVector3 momOut, Double_t time,
			  Double_t length, Double_t eLoss, Double_t lightYield);
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  /** Map of MCis with fixed VolID */
  map <Int_t,Int_t> fMapMcId;                                        //!

  Int_t fIDMedGas;                                                   //!


  ClassDef(ExpertNeuRad,1);
};




#endif 
