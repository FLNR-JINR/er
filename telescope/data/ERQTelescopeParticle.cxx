/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERQTelescopeParticle.h"
//--------------------------------------------------------------------------------------------------
ERQTelescopeParticle::ERQTelescopeParticle(const TLorentzVector lvInteraction,
                                           const Double_t kineticEnergy,
                                           const Double_t deadEloss)
    : fLVInteraction(lvInteraction), fKineticEnergy(kineticEnergy), fDeadEloss(deadEloss) {
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeParticle)