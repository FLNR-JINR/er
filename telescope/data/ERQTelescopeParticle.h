/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERQTelescopeParticle_H
#define ERQTelescopeParticle_H

#include "TNamed.h"
#include "TLorentzVector.h"

/** @class ERQTelescopeParticle
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERQTelescopeParticle : public TNamed {
public:
  ERQTelescopeParticle() = default;
  /** @brief Constructor 
   ** @param deadEloss - eloss in passive detector volumes;
   ** @param lvInteraction - Lorentz vector in interaction;
  **/
  ERQTelescopeParticle(TLorentzVector lvinteraction, Double_t deadEloss,
                       Double_t kineticEnergy);
  Double_t GetDeadEloss() const {return fDeadEloss;}
  Double_t GetKineticEnergy() const {return fKineticEnergy;}
  TLorentzVector GetLVInteraction() const {return fLVInteraction;}
private:
  TLorentzVector fLVInteraction;
  Double_t  fKineticEnergy = 0.;
  Double_t  fDeadEloss = 0.;
  ClassDef(ERQTelescopeParticle, 1)
};

#endif
