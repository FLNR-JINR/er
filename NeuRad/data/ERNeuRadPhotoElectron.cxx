// -------------------------------------------------------------------------
// -----                   ERNeuRadPhotoElectron source file           -----
// -------------------------------------------------------------------------

#include "ERNeuRadPhotoElectron.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron():
    fSide(-1),
    fLYTime(0.),
    fCathodeTime(0.),
    fAnodeTime(0.),
    fPhotonCount(-1),
    fAmplitude(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron(Int_t index,
                                             Int_t side,
                                             Double_t lyTime,
                                             Double_t cathode_time,
                                             Double_t anode_time,
                                             Int_t photon_count,
                                             Double_t amplitude):
    fIndex(index),
    fSide(side),
    fLYTime(lyTime),
    fCathodeTime(cathode_time),
    fAnodeTime(anode_time),
    fPhotonCount(photon_count),
    fAmplitude(amplitude)
{
}
// -------------------------------------------------------------------------

void ERNeuRadPhotoElectron::Print() const {
	std::cout << "PE: " << fIndex << std::endl;
	std::cout << "Side: " << fSide << " Cathode Time: " << fCathodeTime << " AnodeTime: " << fAnodeTime << std::endl;
	std::cout << "Photon count: " << fPhotonCount << " Amplitude: " << fAmplitude << " LYTime:" << fLYTime << std::endl;
}

// -----   Destructor   ----------------------------------------------------
ERNeuRadPhotoElectron::~ERNeuRadPhotoElectron()
{
}
// -------------------------------------------------------------------------

ClassImp(ERNeuRadPhotoElectron)
