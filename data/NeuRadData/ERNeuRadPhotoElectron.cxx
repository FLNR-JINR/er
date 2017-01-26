// -------------------------------------------------------------------------
// -----                      ERNeuRadPhotoElectron source file           -----
// -------------------------------------------------------------------------
#include "ERNeuRadPhotoElectron.h"

#include <iostream>
using namespace std;

// -----   Default constructor   -------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron():
    fSide(-1),fLYTime(0.), fCathodeTime(0.), fAnodeTime(0.),fPhotonCount(-1), fAmplitude(0.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadPhotoElectron::ERNeuRadPhotoElectron(Int_t index, Int_t side,Double_t lyTime,  Double_t cathode_time, Double_t anode_time, 
							Int_t photon_count, Double_t amplitude):
	fIndex(index),fSide(side),fLYTime(lyTime), fCathodeTime(cathode_time), fAnodeTime(anode_time),fPhotonCount(photon_count),fAmplitude(amplitude)
{
}
// -------------------------------------------------------------------------

void ERNeuRadPhotoElectron::Print(){
	cout << "PE: "<< fIndex << endl <<
		"Side: " << fSide << " Cathode Time: " << fCathodeTime << " AnodeTime: " << fAnodeTime << endl <<
		"Photon count: " << fPhotonCount<< " Amplitude: " << fAmplitude << " LYTime:" <<  fLYTime << endl;
}

// -----   Destructor   ----------------------------------------------------
ERNeuRadPhotoElectron::~ERNeuRadPhotoElectron()
{
}
// -------------------------------------------------------------------------
ClassImp(ERNeuRadPhotoElectron)
