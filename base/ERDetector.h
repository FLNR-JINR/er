/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDetector_H
#define ERDetector_H

#include <map>

#include "TString.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TArrayI.h"

#include "FairVolume.h"
#include "FairDetector.h"

#include "ERMCTrack.h" //for ExpertTrackingStatus

#include "ERPoint.h"

/** @class ERDetector
 ** @brief The base class for detector simulation in er sim
 ** @author V.Schetinin <sch_vitaliy@mail.ru>
 ** @version 1.0
**/ 

class ERDetector : public FairDetector
{

public:

	/** @brief Default constructor **/
    ERDetector();

    /**
      constructor
      Name :  Detector Name
      Active: kTRUE for active detectors  (ProcessHits() will be called)
              kFALSE for inactive detectors
    */
	ERDetector(const char* Name, Bool_t Active, Int_t DetId=0);

	/** Virtaul method Construct geometry
	 **
	 ** Constructs the ERNeuRad geometry
	**/
	virtual void ConstructGeometry();

	/*Accessors*/

    /** @brief Get array of ERNeuRadPoint
     ** @param iColl  number of point collection
     ** @return Pointer to ERNeuRadPoint array. NULL if iColl > 0.
     **
     ** Abstract from FairDetector.
    **/
    virtual TClonesArray* GetCollection(Int_t iColl) const;

    /** @brief Add sensetive volume name.
     ** Create new points collection
    **/
	void AddSensetive(TString name);
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
     ** Defines the action to be taken when a step is inside the active volume. Creates a 
     ** ERNeuRadPoint and adds it to the collection.
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
    virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
          Int_t offset);

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

    /** @brief Adds a Point to the Point Collection **/
    ERPoint* AddPoint(TClonesArray* points);
protected:
	//map of sensetive volumes points collection collection, fSenVolumes[senVol] = points
	std::map<TString,TClonesArray*> fSenVolumes;

    std::vector<TString> fSenNames;   ///< Sensetive volumes sustring, that user set 

	Int_t   fVerbose;                 ///< Verbosity level                

    Float_t fFullEnergy;              ///< Sum Edep in event in sensetive volume
    Float_t fFullLY;                  ///< Sum Light Yield in event in sensetive volume

    /* current step information*/
    Int_t fEventID;                   ///< event index
    Int_t fTrackID;                   ///< track index
    Int_t fMot0TrackID;               ///< 0th mother track index
    Double_t fMass;                   ///< mass
    TLorentzVector fPosIn;            ///< point start  position
    TLorentzVector fPosOut;           ///< point finish  position
    TLorentzVector fCurPosIn;         ///< current step position
    TVector3 fPosInLocal;             ///< point position in sensetive volume CS
    TLorentzVector fMomIn;            ///< point start momentum
    TLorentzVector fMomOut;           ///< point start momentum
    TLorentzVector fCurMomIn;         ///< current step momentum
    Double32_t     fTimeIn;           ///< point start time
    Double32_t     fTimeOut;          ///< point finish time
    Double32_t     fTrackLength;      ///< track length from his origin
    Double32_t     fELoss;            ///< energy loss
    Double_t       fLightYield;       ///< light yield
    Int_t          fVolNb;          ///< number of fiber in pixel
    ExpertTrackingStatus fTrackStatus; ///< curren track stutus (transport, stop, disappeared, ...) 
    TArrayI  fProcessesID;            ///< VMC prcess IDs in step

    ClassDef(ERDetector,1)
};

#endif /* ERDetector_H */

