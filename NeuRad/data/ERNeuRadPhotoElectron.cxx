/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadPhotoElectron.h"

#include "FairLogger.h"

// -----   Default constructor   ----------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron():
//TODO mother class initialization
    fIndex(-1),
    fSide(-1),
    fLYTime(0.),
    fCathodeTime(0.),
    fAnodeTime(0.),
    fPhotonCount(-1),
    fAmplitude(0.)
{
}
// ----------------------------------------------------------------------------

// -----   Standard constructor   ---------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron(Int_t index,
                                             Int_t side,
                                             Double_t lyTime,
                                             Double_t cathode_time,
                                             Double_t anode_time,
                                             Int_t photon_count,
                                             Double_t amplitude):
//TODO mother class initialization
    fIndex(index),
    fSide(side),
    fLYTime(lyTime),
    fCathodeTime(cathode_time),
    fAnodeTime(anode_time),
    fPhotonCount(photon_count),
    fAmplitude(amplitude)
{
}
// ----------------------------------------------------------------------------

// -----   Destructor   -------------------------------------------------------
ERNeuRadPhotoElectron::~ERNeuRadPhotoElectron()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadPhotoElectron::Print() const {
	LOG(INFO) << "PE: " << fIndex << FairLogger::endl;
	LOG(INFO) << "Side: " << fSide << " Cathode Time: " << fCathodeTime << " AnodeTime: " << fAnodeTime << FairLogger::endl;
	LOG(INFO) << "Photon count: " << fPhotonCount << " Amplitude: " << fAmplitude << " LYTime:" << fLYTime << FairLogger::endl;
}
// ----------------------------------------------------------------------------

ClassImp(ERNeuRadPhotoElectron)
