/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNeuRad_H
#define ERNeuRad_H

#include "ERDetector.h" // mother class

#include "TLorentzVector.h"
#include "TArrayI.h"

#include "ERMCTrack.h" // for ExpertTrackingStatus

class TClonesArray;
class FairVolume;
class ERNeuRadStep;
class ERNeuRadPoint;

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

    /** @brief Default constructor **/
    ERNeuRad();

    /** @brief Standard constructor
     ** @param name    ERNeuRad detector name
     ** @param active  sensitivity flag
     ** @param verbose Verbosity level. 1 - only standart logs,
     **                                 2 - Print points after each event,
     **                                 3 - GEANT Step information.
    **/
    ERNeuRad(const char* name, Bool_t active, Int_t verbose);

    /** @brief Destructor **/
    virtual ~ERNeuRad();

    /* Modifiers */

    /** @brief Method for switch on storing of first geant steps in sensetive volume. It is necessary
     ** to determine the coordinate of the input to the sensitive volume
    **/
    void SetStorePrimarySteps() { fStorePrimarySteps = kTRUE; }

    /** @brief Method for switch on storing of all geant steps in sensetive volume. It is necessary
     ** to determine geant4 processes in sensetive volume
    **/
    void SetStoreAllSteps() { fStoreAllSteps = kTRUE; }

    /*Accessors*/

    /** @brief Get array of ERNeuRadPoint
     ** @param iColl  number of point collection
     ** @return Pointer to ERNeuRadPoint array. NULL if iColl > 0.
     **
     ** Abstract from FairDetector.
    **/
    virtual TClonesArray* GetCollection(Int_t iColl) const;

public:

    /** @brief Initialisation
     ** class method FairDetector::Initialize() is called.
     ** NeuRadGeoPar init from RuntimeDB
     ** Virtual from FairDetector.
    **/
    virtual void Initialize();

    /** @brief Register output array (NeuRadPoint) to the I/O manager
     ** Abstract from FairDetector.
    **/
    virtual void Register();

    /** @brief Virtual method
     ** Defines the action to be taken when a step is inside the active volume.
     ** Creates an ERNeuRadPoint and adds it to the collection.
     ** @param vol Pointer to the active volume
     ** Virtual from FairDetector.
    **/
    virtual Bool_t ProcessHits(FairVolume* vol = 0);

    /** @brief Action at end of event
     ** Short status log and Reset().
     ** Virtual from FairDetector.
    **/
    virtual void EndOfEvent();

    /** @brief Screen log
     ** Prints NeuRadPoint information
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
     ** The decision is based on the volume name (has to contain "Sensor").
     ** Virtual from FairModule.
    **/
    virtual Bool_t CheckIfSensitive(std::string name);

private:

    /** @brief Start new point creation. Reinit current point data **/
    void StartNewPoint();

    /** @brief Finish point creation. Call AddPoint() **/
    void FinishNewPoint();

    /** @brief Adds a NeuRadPoint to the Point Collection **/
    ERNeuRadPoint* AddPoint();

    /** @brief Adds a NeuRadStep to the PrimaryStep Collection **/
    ERNeuRadStep* AddFirstStep();

    /** @brief Adds a NeuRadStep to the Steps Collection **/
    ERNeuRadStep* AddStep();

    /** @brief return current point length **/
    Double_t CurPointLen(TLorentzVector& posIn);

    /** @brief parse geometry path and compute pmt ID and channel ID **/
    /** This method fills fPmtId and fChId data members
    **/
    void ObtainChIdfromGMC();

private:

    TClonesArray*  fNeuRadPoints;     ///< The point collection
    TClonesArray*  fNeuRadFirstStep;  ///< The first step collection
    TClonesArray*  fNeuRadSteps;      ///< The steps collection

    Bool_t fStorePrimarySteps;        ///< Flag to storing firs step in sensetive volume
    Bool_t fStoreAllSteps;            ///< Flag to storing all steps in sensetive volume
    Int_t  fVerbose;                  ///< Verbosity level

    Float_t fFullEnergy;              ///< Sum Edep in event in sensetive volume
    Float_t fFullLY;                  ///< Sum Light Yield in event in sensetive volume

    /* current step information*/

    Int_t          fEventID;          ///< event index
    Int_t          fTrackID;          ///< track index
    Int_t          fMot0TrackID;      ///< 0th mother track index
    Double_t       fMass;             ///< mass
    TLorentzVector fPosIn;            ///< point start  position
    TLorentzVector fPosOut;           ///< point finish  position
    TLorentzVector fCurPosIn;         ///< current step position
    TVector3       fPosInLocal;       ///< point position in sensetive volume CS
    TLorentzVector fMomIn;            ///< point start momentum
    TLorentzVector fMomOut;           ///< point start momentum
    TLorentzVector fCurMomIn;         ///< current step momentum
    Double32_t     fTimeIn;           ///< point start time
    Double32_t     fTimeOut;          ///< point finish time
    Double32_t     fTrackLength;      ///< track length from his origin
    Double32_t     fELoss;            ///< energy loss
    Double_t       fLightYield;       ///< light yield

    //Int_t          fFiberNb;          ///< number of fiber in pixel
    //Int_t          fPixelNb;          ///< number of pixel in module
    //Int_t          fModuleNb;         ///< number of module in NeuRad
    UInt_t          fPmtId;           ///< current pmt ID
    UInt_t          fChId;            ///< current channel ID

    Int_t          fStepNb;           ///< current step numb in this active volumes
    ExpertTrackingStatus fTrackStatus; ///< curren track stutus (transport, stop, disappeared, ...)
    TArrayI        fProcessesID;      ///< VMC prcess IDs in step

    ClassDef(ERNeuRad, 1);
};

#endif // ERNeuRad_H
