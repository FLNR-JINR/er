/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERInteractionParticle_H
#define ERInteractionParticle_H

#include "TNamed.h"
#include "TLorentzVector.h"

#include "ERParticle.h"

/** @class ERInteractionParticle
 ** @brief Class of interaction particle object for simulation and reconstruction;
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/
class ERInteractionParticle : public ERParticle
{

public:

  /** @brief Default constructor **/
  ERInteractionParticle();

  /** @brief Standard constructor
  ** @param name Particle name
  ** @param A Particle A
  ** @param Z Particle Z
  ** @param Q Particle Q
  **/
  ERInteractionParticle(TString name, Int_t A, Int_t Z, Int_t Q, mass = 0.);

  /** @brief Standard constructor
  ** @param name Particle name
  ** @param PDG Particle PDG code
  **/
  ERInteractionParticle(TString name, Int_t PDG, mass = 0.);

  /** @brief Destructor **/
  virtual ~ERInteractionParticle();

  /* Modifiers */

  TLorentzVector SetLab( TLorentzVector lab) { fLab = lab; }
  TLorentzVector SetCM( TLorentzVector cm) { fCM = cm; }

  /* Accessors */

  TLorentzVector GetLab() const       { return fLab; }
  TLorentzVector GetCM() const       { return fCM; }

private:

  TLorentzVector fLab;           ///< Particle state in Laboratory System
  TLorentzVector fCM;            ///< Particle state in Center of Mass System

  ClassDef(ERInteractionParticle,1)
};

#endif // ERInteractionParticle_H
