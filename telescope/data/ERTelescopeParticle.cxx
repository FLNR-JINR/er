/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERTelescopeParticle.h"
//--------------------------------------------------------------------------------------------------
ERTelescopeParticle::ERTelescopeParticle(
    const TLorentzVector lvInteraction, const Double_t kineticEnergy,
    const Double_t deadEloss, const Double_t edepInThickStation /*= -1.*/,
    const Double_t edepInThinStation /*= -1.*/, const Double_t correctedEdepInThickStation /*= -1.*/,
    const Double_t correctedEdepInThinStation /*= -1.*/, 
    const ERChannel channelOfThinStation/*= consts::undefined_channel*/,
    const ERChannel channelOfThickStation/*= consts::undefined_channel*/)
    : fLVInteraction(lvInteraction), fKineticEnergy(kineticEnergy), fDeadEloss(deadEloss),
      fEdepInThickStation(edepInThickStation), fEdepInThinStation(edepInThinStation),
      fCorrectedEdepInThickStation(correctedEdepInThickStation), 
      fCorrectedEdepInThinStation(correctedEdepInThinStation),
      fChannelOfThinStation(channelOfThinStation),
      fChannelOfThickStation(channelOfThickStation)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeParticle)