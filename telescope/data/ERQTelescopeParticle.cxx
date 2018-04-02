/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERQTelescopeParticle.h"

ERQTelescopeParticle::ERQTelescopeParticle()
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeParticle::ERQTelescopeParticle(TLorentzVector lvTelescope, TLorentzVector lvTarget,Double_t deadEloss)
: fDeadEloss(deadEloss),
  fLVTarget(lvTarget),
  fLVTelescope(lvTarget)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeParticle)