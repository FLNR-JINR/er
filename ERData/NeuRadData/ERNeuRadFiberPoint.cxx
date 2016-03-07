// -------------------------------------------------------------------------
// -----                      ERNeuRadFiberPoint source file           -----
// -------------------------------------------------------------------------

#include "ERNeuRadFiberPoint.h"


// -----   Default constructor   -------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint():
    fSide(-1),fLYTime(0.), fCathodeTime(0.), fAnodeTime(0.),fPhotonCount(-1),
	fPhotoElCount(-1), fAmplitude(0.), fOnePE(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint(Int_t index, Int_t side,Double_t lyTime,  Double_t cathode_time, Double_t anode_time, 
							Int_t photon_count, Int_t photoel_count, Double_t amplitude, Int_t onePE):
	fIndex(index),fSide(side),fLYTime(lyTime), fCathodeTime(cathode_time), fAnodeTime(anode_time),fPhotonCount(photon_count),
	fPhotoElCount(photoel_count), fAmplitude(amplitude), fOnePE(onePE)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadFiberPoint::~ERNeuRadFiberPoint()
{
}
// -------------------------------------------------------------------------
ClassImp(ERNeuRadFiberPoint)
