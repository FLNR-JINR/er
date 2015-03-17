// -------------------------------------------------------------------------
// -----                        ExpertNeuRad header file               -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

/**  ExpertNeuRad.h
 **/


#ifndef ExpertNeuRad_H
#define ExpertNeuRad_H

#include "FairDetector.h"
#include "ExpertNeuRadPoint.h"

#include "TH1F.h"
#include "TLorentzVector.h"

using namespace std;

class TClonesArray;
class FairVolume;
class TF1;

class ExpertNeuRad : public FairDetector
{
  
public:
  
  /** Default constructor **/
  ExpertNeuRad();
  
  
  /** Standard constructor.
   *@param name    ExpertNeuRad detector name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ExpertNeuRad(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ExpertNeuRad();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ExpertNeuRadPoint and adds it to the
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
   ** Constructs the ExpertNeuRad geometry
   **/
  virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize detector data
   **/
  virtual void Initialize();

  /** Virtaul method CheckIfSensitive 
	**Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name (has to contain "module").
  **/
  virtual Bool_t CheckIfSensitive(std::string name);
  
  /** Virtaul method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
  
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
  TClonesArray*  fNeuRadCollection;  //!  The point collection
  Int_t fVersion;                    //! geometry version
  
  TH1F *fhSumEnergyLossOfAllModules;    //! Histo Events per sum of ELoss
  TH1F *fhModulesPerSumEnergyLoss;      //! Histo Modules with point count per Energy loss
  static Int_t fNEeventsWithoutPoints;  //! Count of events without points

  //Constants
  static const Double_t fHistoELossThreshold; //! Threshold of ELoss for analyze histos
  
private:
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
  
  /** Private method StepHistory
   **
   ** Print GEANT step information
   **/
  void StepHistory();
  
  /**  Private method FillHisto
  **
  **  Filling histos for analyze and write histos to param file
  **/
  void FillHisto();
  
  ClassDef(ExpertNeuRad,1);
};




#endif 
