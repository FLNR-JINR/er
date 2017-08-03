/********************************************************************************
 *    Copyright (C) Joint Institute for Nuclear Research                        *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNeuRad_H
#define ERNeuRad_H

#include "TH1F.h"
#include "TLorentzVector.h"

#include "ERDetector.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadPoint.h"

class TClonesArray;
class FairVolume;
class TF1;

/** @class ERNeuRad
 ** @brief Class for the MC transport of the NeuRad
 ** @author V.Schetinin <sch_vitaliy@mail.ru>
 ** @version 1.0
 **
 ** The ERNeuRad defines the behaviour of the NeuRad system during
 ** transport simulation. It constructs the NeuRad transport geometry
 ** and creates objects of type ERNeuRadPoins and ERNeuRadSteps if requested.
**/
class ERNeuRad : public ERDetector
{
  
public:
  
  /** Default constructor **/
  ERNeuRad();
  
  
  /** Standard constructor.
   *@param name    ERNeuRad detector name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 
                                    2 - Print points after each event, 
                                    3 - GEANT Step information
   **/
  ERNeuRad(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERNeuRad();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERNeuRadPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  
  
  /** @brief Action at start of event
   **
   ** Short status log and Reset().
   ** Virtual from FairDetector.
  **/
  virtual void BeginEvent();
  
  /** @brief Action at end of event
   **
   ** Short status log and Reset().
   ** Virtual from FairDetector.
  **/
  virtual void EndOfEvent();
  
  /** @brief Register output array (NeuRadPoint) to the I/O manager
   **
   ** Abstract from FairDetector.
  **/
  virtual void Register();
  
  
  /** @brief Get array of ERNeuRadPoint
   ** @param iColl  number of point collection
   ** @return Pointer to ERNeuRadPoint array. NULL if iColl > 0.
   **
   ** Abstract from FairDetector.
  **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  
  
  /** @brief Screen log
    ** Prints NeuRadPoint information
    ** Virtual from TObject.
  **/
  virtual void Print(Option_t *option="") const;
  
  /** Virtual method Reset
   **
   ** Clears the point collection
   **/
  virtual void Reset();

  /** Virtual method CopyClones
   **
   ** Copies the points collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);

  /**  @brief Initialisation
   ** class method FairDetector::Initialize() is called.
   ** NeuRadGeoPar init from RuntimeDB
   ** Virtual from FairDetector.
   **/
  virtual void Initialize();

  /** @brief Check whether a volume is sensitive.
   ** @param name  Volume name
   ** @value        kTRUE if volume is sensitive, else kFALSE
   ** The decision is based on the volume name (has to contain "Sensor").
   ** Virtual from FairModule.
   **/
  virtual Bool_t CheckIfSensitive(std::string name);

  /** Method for switch on storing of first geant steps in sensetive volume.
  It is necessary to determine the coordinate of the input to the sensitive volume
  **/
  void SetStorePrimarySteps() {fStorePrimarySteps = kTRUE;}
  /** Method for switch on storing of all geant steps in sensetive volume.
  It is necessary to determine geant4 processes in sensetive volume
  **/
  void SetStoreAllSteps() {fStoreAllSteps = kTRUE;}
private:
  TClonesArray*  fNeuRadPoints;     //!  The point collection
  TClonesArray*  fNeuRadFirstStep;  //!  The first step collection
  TClonesArray*  fNeuRadSteps;      //!  The steps collection
    
  Bool_t fStorePrimarySteps;
  Bool_t fStoreAllSteps;
  Int_t   fVerbose;

  Float_t fFullEnergy;
  Float_t fFullLY;

  /* current step information*/
  Int_t fEventID;                   //!  event index
  Int_t fTrackID;                   //!  track index
  Int_t fMot0TrackID;               //!  0th mother track index
  Double_t fMass;                   //!  mass
  TLorentzVector fPosIn;            //!  point start  position
  TLorentzVector fPosOut;            //!  point finish  position
  TLorentzVector fCurPosIn;         //!  current step position
  TVector3 fPosInLocal;             //!  position
  TLorentzVector fMomIn;            //!  point start momentum
  TLorentzVector fMomOut;            //!  point start momentum
  TLorentzVector fCurMomIn;         //!  current step momentum
  Double32_t     fTimeIn;           //!  point start time
  Double32_t     fTimeOut;           //!  point finish time
  Double32_t     fTrackLength;      //!  track length from his origin
  Double32_t     fELoss;            //!  energy loss
  Double_t       fLightYield;       //!  light yield
  Int_t          fFiberNb;          //!  number of fiber in pixel
  Int_t          fPixelNb;          //!  number of pixel in module
  Int_t          fModuleNb;         //!  number of module in NeuRad
  Int_t          fStepNb;           //!  current step numb in this active volumes
  ExpertTrackingStatus fTrackStatus; //!  
  TArrayI  fProcessesID;              //!  VMC prcess IDs in step


private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
  ERNeuRadPoint* AddPoint();
  
  /** Private method AddFirstStep
  **
  ** Adds a NeuRadStep to the FirstStep Collection
  **/
  ERNeuRadStep* AddFirstStep();
        
  /** Private method AddStep
  **
  ** Adds a NeuRadStep to the Steps Collection
  **/      
  ERNeuRadStep* AddStep();
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  void StartNewPoint();
  void FinishNewPoint();
                            
  Double_t CurPointLen(TLorentzVector& posIn);
  
  ClassDef(ERNeuRad,1);
};

#endif 
