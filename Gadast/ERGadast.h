// -------------------------------------------------------------------------
// -----                        ERGadast header file                   -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------


#ifndef ERGadast_H
#define ERGadast_H

#include "ERDetector.h"
#include "ERGadastPoint.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERGadast : public ERDetector
{
  
public:
  
  /** Default constructor **/
  ERGadast();
  
  
  /** Standard constructor.
   *@param name    ERGadast ERGadast name
   *@param active  sensitivity flag
   **/
  ERGadast(const char* name, Bool_t active);
  
  
  /** Destructor **/
  virtual ~ERGadast();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERGadastPoint and adds it to the
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
  
  
  /** Virtaul method Construct geometry
   **
   ** Constructs the ERGadast geometry
   **/
  //virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERGadast data
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
  
  /** Virtaul method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
  
private:
  TClonesArray*  fDetectorPoints;     //!  The point collection
  Int_t fVersion;                    //! geometry version
private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
   
  ERGadastPoint* AddPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Double_t mass,
			  TVector3 posIn,
			  TVector3 pos_out, TVector3 momIn,
			  TVector3 momOut, Double_t time,
			  Double_t length, Double_t eLoss, 
        ERGadastPointType type,
        Int_t pdg);
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  ClassDef(ERGadast,1);
};

#endif 
