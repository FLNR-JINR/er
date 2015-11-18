// -------------------------------------------------------------------------
// -----                      ERNeuRadFiberPoint source file           -----
// -------------------------------------------------------------------------

#include "ERNeuRadFiberPoint.h"


// -----   Default constructor   -------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint():
    fSide(-1),fCathodeTime(0.), fAnodeTime(0.),fPhotonCount(-1),
	fPhotoElCount(-1), fAmplitude(0.) 
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
							Int_t photon_count, Int_t photoel_count, 
							Double_t amplitude):
	fSide(side),fCathodeTime(cathode_time), fAnodeTime(anode_time),fPhotonCount(photon_count),
	fPhotoElCount(photoel_count), fAmplitude(amplitude) 
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadFiberPoint::~ERNeuRadFiberPoint()
{
}
// -------------------------------------------------------------------------
ClassImp(ERNeuRadFiberPoint)
