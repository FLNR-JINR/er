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
  ERQTelescopeParticle(Double_t deadEloss);

private:
  Double_t  fDeadEloss;

  ClassDef(ERQTelescopeParticle, 1)
};

#endif
