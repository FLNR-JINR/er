/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERInteraction_H
#define ERInteraction_H

#include "TNamed.h"

#include "ERInteractionParticle.h"

/** @class ERInteraction
 ** @brief Base class for interaction data store, simulation and reco procedures;
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/
class ERInteraction : public TNamed
{

public:

    /** @brief Default constructor **/
    ERInteraction();

    /** @brief Standard constructor
     ** @param name interaction name
    **/
    ERInteraction(const char* name);

    /** @brief Destructor **/
    virtual ~ERInteraction();

    /* Modifiers */

    /** @brief Set projectile of interaction by A,Z,Q.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void SetProjectile(Int_t A, Int_t Z, Int_t Q, Double_t mass = 0.);

    /** @brief Set projectile of interaction by PDG.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void SetProjectile(Int_t pdg, Double_t mass = 0.);

    /** @brief Set target of interaction by A,Z,Q.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void SetTarget(Int_t A, Int_t Z, Int_t Q, Double_t mass = 0.);

    /** @brief Set target of interaction by PDG.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void SetTarget(Int_t pdg, Double_t mass = 0.);

    /** @brief Add ejectile of interaction by A,Z,Q.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void AddEjectile(Int_t A, Int_t Z, Int_t Q, Double_t mass = 0.);

    /** @brief AddEjectile of interaction by PDG.
     ** If mass is`t defined, mass from Geant will be used.
    **/
    void AddEjectile(Int_t pdg, Double_t mass = 0.);

    /* Accessors */

    ERInteractionParticle GetProjectile() const    { return fProjectile; }
    ERInteractionParticle GetTarget() const        { return fTarget;     }
    ERInteractionParticle GetEjectile( Int_t index) const;

public:

    /** @brief Initialization and checking of interaction definintion
    **/
    void Init();

protected:

    ERInteractionParticle fProjectile;           ///< Projectile particle
    ERInteractionParticle fTarget;               ///< Target particle
    TClonesArray* fEjectiles;                    ///< Array of ejeciles

    ClassDef(ERInteraction,1)
};

#endif // ERInteraction_H
