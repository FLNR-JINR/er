/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERParticle_H
#define ERParticle_H

#include "TNamed.h"
#include "TString.h"
#include "TLorentzVector.h"

/** @class ERParticle
 ** @brief Base class for particle object in er;
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERParticle: public TNamed {

public:

  /** @brief Default constructor **/
  ERParticle(){}

  /** @brief Standard constructor
   ** @param name Particle name
   ** @param A Particle A
   ** @param Z Particle Z
   ** @param Q Particle Q
  **/
  ERParticle(TString name, Int_t A, Int_t Z, Int_t Q, Double_t mass = 0.);

  /** @brief Standard constructor
   ** @param name Particle name
   ** @param PDG Particle PDG code
  **/
  ERParticle(TString name, Int_t PDG, Double_t mass = 0.);

  /** @brief Destructor **/
  virtual ~ERParticle();

  /* Modifires */

  void SetExcitation(Double_t excitation) { fExcitation = excitation; }

  /* Accessors */

  Int_t GetPDG() const            { return fPDG; }
  Double_t GetMass() const        { return fMass; }
  Double_t GetExcitation() const  { return fExcitation; }

  virtual void Print() const;

  void DefineMass();

protected:

  Int_t fPDG;           ///< Particle PDG code
  Double_t fMass;       ///< Particle Mass / Energy of main state
  Double_t fExcitation; ///< Particle Excitation

  ClassDef(ERParticle,1);
};

#endif
