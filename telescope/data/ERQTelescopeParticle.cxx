/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERQTelescopeParticle.h"
//--------------------------------------------------------------------------------------------------
ERQTelescopeParticle::ERQTelescopeParticle(
    const TLorentzVector lvInteraction, const Double_t kineticEnergy,
    const Double_t deadEloss, const Double_t edepInThickStation /*= -1.*/,
    const Double_t edepInThinStation /*= -1.*/, const Double_t correctedEdepInThickStation /*= -1.*/,
    const Double_t correctedEdepInThinStation /*= -1.*/)
    : fLVInteraction(lvInteraction), fKineticEnergy(kineticEnergy), fDeadEloss(deadEloss),
      fEdepInThickStation(edepInThickStation), fEdepInThinStation(edepInThinStation),
      fCorrectedEdepInThickStation(correctedEdepInThickStation), 
      fCorrectedEdepInThinStation(correctedEdepInThinStation)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeParticle)