// -------------------------------------------------------------------------
// -----                        ERQTelescope header file                      -----
// -----                  Created data  by developer name              -----
// -------------------------------------------------------------------------


#ifndef ERQTelescope_H
#define ERQTelescope_H

#include "ERDetector.h"
#include "ERQTelescopeSiPoint.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

class ERQTelescope : public ERDetector
{
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

  /** Virtaul method CheckIfSensitive
        **Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name.
  **/
  virtual Bool_t CheckIfSensitive(std::string name);

private:
  TClonesArray*  fSiPoints;

  Int_t          fEventID;           //!  event index
  Int_t          fTrackID;           //!  track index
  Int_t          fMot0TrackID;       //!  mother track index
  Int_t          fPID;               //!  particle PDG
  TLorentzVector fPosIn, fPosOut;     //!  position
  TLorentzVector fMomIn, fMomOut;     //!  momentum
  Double32_t     fTime;              //!  time
  Double32_t     fLength;            //!  length
  Double32_t     fELoss;             //!  energy loss
  Int_t          fStationNb;
  Int_t          fStripNb;
 
  
private:
  /** Private method AddPoint
   **
   ** Adds a ERQTelescopePoint to the Point Collection
   **/
  ERQTelescopeSiPoint*   AddSiPoint();
  ClassDef(ERQTelescope,1);
};

#endif