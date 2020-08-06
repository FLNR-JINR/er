/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERTelescopeParticle_H
#define ERTelescopeParticle_H

#include "TNamed.h"
#include "TLorentzVector.h"

/** @class ERTelescopeParticle
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERTelescopeParticle : public TNamed {
public:
  ERTelescopeParticle() = default;
  /** @brief Constructor 
   ** @param deadEloss - eloss in passive detector volumes;
   ** @param lvInteraction - Lorentz vector in interaction;
  **/
  ERTelescopeParticle(TLorentzVector lvinteraction, Double_t deadEloss,
                       Double_t kineticEnergy, Double_t edepInThickStation = -1.,
                       Double_t edepInThinStation = -1., Double_t correctedEdepInThickStation = -1.,
                       Double_t correctedEdepInThinStation = -1.);
  Double_t GetDeadEloss() const {return fDeadEloss;}
  Double_t GetKineticEnergy() const {return fKineticEnergy;}
  TLorentzVector GetLVInteraction() const {return fLVInteraction;}
  Double_t GetEdepInThickStation() const {return fEdepInThickStation;}
  Double_t GetEdepInThinStation() const {return fEdepInThinStation;}
  Double_t GetCorrectedEdepInThickStation() const {return fCorrectedEdepInThickStation;}
  Double_t GetCorrectedEdepInThinStation() const {return fCorrectedEdepInThinStation;}
private:
  TLorentzVector fLVInteraction;
  Double_t  fKineticEnergy = 0.;
  Double_t  fDeadEloss = 0.;
  Double_t  fEdepInThickStation = -1.;
  Double_t  fEdepInThinStation = -1.;
  Double_t  fCorrectedEdepInThickStation = -1.;
  Double_t  fCorrectedEdepInThinStation = -1.;
  ClassDef(ERTelescopeParticle, 1)
};

#endif
