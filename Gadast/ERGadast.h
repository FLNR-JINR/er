/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/


#ifndef ERGadast_H
#define ERGadast_H


#include "TVector3.h"

#include "ERDetector.h"
#include "ERGadastCsIPoint.h"
#include "ERGadastLaBrPoint.h"
#include "ERGadastStep.h"
#include "ERDetectorList.h"
#include "ERSupport.h"

/** @class ERGadast
 ** @brief Class for the MC transport of the Gadast
 ** @author V.Schetinin <sch_vitaliy@mail.ru>
 ** @version 1.0
 **
 ** The ERGadast defines the behaviour of the Gadast system during
 ** transport simulation. It constructs the Gadast transport geometry
 ** and creates objects of type ERGadastPoins and ERGadastSteps if requested.
**/

class ERGadast : public ERDetector
{
  
public:
  
  /** @brief Default constructor **/
  ERGadast();
  
  /** @brief Standard constructor
     ** @param name    ERNeuRad detector name
     ** @param active  sensitivity flag
     ** @param verbose Verbosity level. 1 - only standart logs,
     **                                 2 - Print points after each event, 
     **                                 3 - GEANT Step information.
  **/
  ERGadast(const char* name, Bool_t active, Int_t verbose);
   
  /** @brief Destructor **/
  virtual ~ERGadast();

  /* Modifiers */

  /** @brief Method for switch on storing of all geant steps in sensetive volume. It is necessary 
   ** to determine geant4 processes in sensetive volume
  **/
  void SetStoreSteps() {fStoreSteps = kTRUE;}

  /*Accessors*/

  /** @brief Get array of ERGadastPoint
   ** @param iColl  number of point collection
   ** @return Pointer to ERGadastPoint array. NULL if iColl > 0.
   **
   ** Abstract from FairDetector.
  **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;

public:

  /** @brief Initialisation
   ** class method FairDetector::Initialize() is called.
   ** GadastGeoPar init from RuntimeDB
   ** Virtual from FairDetector.
  **/
  virtual void Initialize();

  /** @brief Register output array (NeuRadPoint) to the I/O manager
   ** Abstract from FairDetector.
  **/
  virtual void Register();

  /** @brief Virtual method 
   ** Defines the action to be taken when a step is inside the active volume. Creates a 
   ** ERGadastPoint and adds it to the collection.
   ** @param vol  Pointer to the active volume
   ** Virtual from FairDetector.
  **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);

  /** @brief Action at end of event
   ** Short status log and Reset().
   ** Virtual from FairDetector.
  **/
  virtual void EndOfEvent();
  
  /** @brief Screen log
   ** Prints GadastPoint information
   ** Virtual from TObject.
  **/
  virtual void Print(Option_t *option="") const;

  /** @brief Clears the point and steps collections
   ** Virtual from FairDetector
  **/
  virtual void Reset();

  /** @brief Copies the points collection with a given track index offset
   ** @param cl1     Origin
   ** @param cl2     Target
   ** @param offset  Index offset
   ** Virtual from FairDetector
  **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);

  /** @brief Check whether a volume is sensitive.
   ** @param name  Volume name
   ** @value        kTRUE if volume is sensitive, else kFALSE
   ** The decision is based on the volume name (has to contain "LaBrcell_cell" for LaBrPoint,
   ** "onecell_cell" for CsI Point).
   ** Virtual from FairModule.
  **/
  virtual Bool_t CheckIfSensitive(std::string name);

private:

  /** @brief Adds a GadastCsIPoint to the Point Collection **/
  ERGadastCsIPoint* AddCsIPoint();

  /** @brief Adds a GadastLaBrPoint to the Point Collection **/
  ERGadastLaBrPoint* AddLaBrPoint();

  /** @brief Adds a GadastStep to the Step Collection **/
  ERGadastStep* AddStep();

  /** @brief Start new point creation. Reinit current point data **/
  void StartPoint();

  /** @brief Finish point creation. Call AddCsIPoint() or AddLaBrPoint() **/
  void FinishPoint();

private:

  int ParentGammaTrackId(int track_id) const;

  TClonesArray*  fCsIPoints = nullptr;                  ///< The CsI point collection
  TClonesArray*  fLaBrPoints = nullptr;                 ///< The LaBr point collection
  TClonesArray*  fGadastSteps = nullptr;                ///< The all steps collection
  TClonesArray*  fMCTracks = nullptr;                   ///< The mc tracks collection
  
  Bool_t fStoreSteps;                         ///< Flag to storing all steps in sensetive volume

  /* current point information*/

  Int_t fEventID;                             ///< event index
  Int_t fTrackID;                             ///< track index
  Int_t fParentGammaTrackId = -1;             ///< parent gamma track index
  Int_t fMot0TrackID;                         ///< 0th mother track index
  Double_t fMass;                             ///< mass
  TVector3 fPosIn;                            ///< point start  position
  TVector3 fPosOut;                           ///< point finish  position         
  TVector3 fMomIn;                            ///< point start momentum
  TVector3 fMomOut;                           ///< point finish momentum
  Double32_t     fTime;                       ///< point start time
  Double32_t     fLength;                     ///< track length from his origin
  Double32_t     fELoss;                      ///< energy loss
  Int_t fPDG;                                 ///< particle PDG
  Int_t  fStepNr;                             ///< current step numb in this active volumes
  SensetiveType  fDetectorType;               ///< CsI or LaBr point
  Int_t fCsIWall;                             ///< CsI Wall number
  Int_t fCsIBlock;                            ///< CsI Block number
  Int_t fCsICell;                             ///< CsI Cell number
  Int_t fLaBrCell;                            ///< laBr Cell number

  ClassDef(ERGadast,1);
};

#endif 
