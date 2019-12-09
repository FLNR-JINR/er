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

class ERQTelescopeParticle : public TNamed{

public:
  /** @brief Default constructor **/
  ERQTelescopeParticle();

  /** @brief Constructor 
   ** @param deadEloss - eloss in passive detector volumes;
  **/
  ERQTelescopeParticle(TLorentzVector lvTelescope, TLorentzVector lvTarget,Double_t deadEloss);
  ERQTelescopeParticle(TLorentzVector lvTelescope, TLorentzVector lvTarget,Double_t deadEloss, Double_t T);
  Double_t GetDeadEloss() const {return fDeadEloss;}
  Double_t GetT() const {return fT;}
  TLorentzVector GetLVTarget() const {return fLVTarget;}
  TLorentzVector GetLVTelescope() const {return fLVTelescope;}
private:
  TLorentzVector fLVTarget;
  TLorentzVector fLVTelescope;
  Double_t  fDeadEloss;
  Double_t  fT;
  Double_t  fT_noCorrections;
  ClassDef(ERQTelescopeParticle, 1)
};

#endif
