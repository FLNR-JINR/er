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
ERQTelescopeParticle::ERQTelescopeParticle(Double_t deadEloss)
: fDeadEloss(deadEloss)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeParticle)