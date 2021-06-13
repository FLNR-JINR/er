/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERBeamDet_H
#define ERBeamDet_H

#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "ERDetector.h"
#include "ERBeamDetTOFPoint.h"
#include "ERBeamDetMWPCPoint.h"
#include "ERBeamDetTargetPoint.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetParticle.h"
#include "ERBeamDetSetup.h"

/** @class ERBeamDet
 ** @brief Class for the MC transport of the BeamDet
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERBeamDet defines the behaviour of the BeamDet system during
 ** transport simulation. It constructs the BeamDet transport geometry
 ** and creates objects of type ERBeamDetMWPCPoints, ERBeamDetToFPoints and ERBeamDetTargetPoints.**/
class ERBeamDet : public ERDetector {
  public:
    /** @brief Default constructor **/
    ERBeamDet();
    /** @brief Standard constructor.
     ** @param name    ERBeamDet ERBeamDet name
     ** @param active  sensitivity flag
     ** @param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 
                                        3 - GEANT Step information **/
    ERBeamDet(const char* name, Bool_t active, Int_t verbose);
    /** @brief Destructor **/
    virtual ~ERBeamDet();
    void SetIonName(const TString& ionName) {fIonName = ionName; fIonPIDIsSet = kTRUE;}
    void SetIonPID(int ionPID) {fIonPID = ionPID; fIonPIDIsSet = kTRUE;}
  public:
    /** @brief Defines the action to be taken when a step is inside the
     ** active volume. Creates a ERBeamDetPoint and adds it to the
     ** collection.
     ** @param vol  Pointer to the active volume **/
    virtual Bool_t ProcessHits(FairVolume* vol = 0);
    /** @brief If verbosity level is set, print point collection at the
     ** end of the event.
     ** Virtual from FairDetector. **/
    virtual void EndOfEvent();
    /** @brief Registers the point collection in the ROOT manager.
     ** Virtual from FairDetector. **/
    virtual void Register();
    /** @brief Accessor to the point collection .
     ** Abstract from FairDetector. **/
    virtual TClonesArray* GetCollection(Int_t iColl) const;
    /** @brief Screen output of hit collection.
     ** Virtual from TObject. **/
    virtual void Print(Option_t *option="") const;
    /** @brief Clears the point collection.
     ** Virtual from FairDetector. **/
    virtual void Reset();
    /** @brief Copies the hit collection with a given track index offset.
     ** @param cl1     Origin
     ** @param cl2     Target
     ** @param offset  Index offset
     ** Virtual from FairDetector. **/
    virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);
    /** @brief Initialize ERBeamDet data.
     ** Abstract from FairDetector. **/
    virtual void Initialize();
    /** @brief Check whether a volume is sensitive.
     ** @param name   Volume name
     ** @value        kTRUE if volume is sensitive, else kFALSE
     ** The decision is based on the volume name.
     ** Virtual from FairModule. **/
    virtual Bool_t CheckIfSensitive(std::string name);
    /** @brief Builds geometry and writes it to temporary file
     ** trough parameters from ERBeamDetSetup class object.
     ** Virtual from FairDetector. **/
    virtual void ConstructGeometry();
  private:
    ERBeamDetSetup* fSetup = nullptr;
    /** @brief Adds a ERBeamDetToFPoint to the ToFPoints Collection **/
    ERBeamDetTOFPoint*    AddTOFPoint();
    /** @brief Adds a ERBeamDetMWPCPoint to the MWPCPoints Collection **/
    ERBeamDetMWPCPoint*   AddMWPCPoint();
    /** @brief Adds a ERBeamDetTargetPoint to the TargetPoints Collection **/
    ERBeamDetTargetPoint* AddTargetPoint();
  private:
    TClonesArray* fToFPoints = nullptr; ///< The ToFPoint collection
    TClonesArray* fMWPCPoints = nullptr; ///< The MWPCPoint collection
    TClonesArray* fTargetPoints = nullptr; ///< The TargetPoint collection
    Int_t fEventID = -1; ///< event index
    Int_t fTrackID = -1; ///< track index
    Int_t Mot0TrackID = -1; //< mother track index
    Int_t fPID = -1; ///< particle PDG
    TLorentzVector fPosIn; ///< point start position
    TLorentzVector fPosOut; ///< point finish position
    TLorentzVector fMomIn; ///< point start momentum
    TLorentzVector fMomOut; ///< point finish momentum
    Double32_t fTime = 0.; ///< point start time
    Double32_t fLength = 0.; ///< length
    Double32_t ELoss = 0.; ///< energy loss
    Double32_t fLightYield = 0.; ///< light yield
    Int_t fToFNb = -1; ///< ToF plastic number
    Int_t fMWPCNb = -1; ///< MWPC station number
    Int_t fMWPCPlaneNb = -1; ///< gas strip array number in MWPC station
    Int_t fMWPCWireNb = -1; ///< wire number in gas strip array
    Bool_t fSensitiveTargetIsSet = kFALSE;
    TString fIonName;
    Int_t fIonPID = -1;
    Bool_t fIonPIDIsSet = kFALSE;
    ClassDef(ERBeamDet,1);
};

#endif