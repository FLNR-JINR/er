// -------------------------------------------------------------------------
// -----                      ERNeuRadFiberPoint source file           -----
// -------------------------------------------------------------------------

#include "ERNeuRadFiberPoint.h"


// -----   Default constructor   -------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint():
    fSide(-1),fCathodeTime(0.), fAnodeTime(0.),fPhotonCount(-1),
	fPhotoElCathodeCount(-1), fPhotoElAnodeCount(-1) 
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERNeuRadFiberPoint::ERNeuRadFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
							Int_t photon_count, Int_t photoel_cathode_count, 
							Int_t photoel_anode_count):
	fSide(side),fCathodeTime(cathode_time), fAnodeTime(anode_time),fPhotonCount(photon_count),
	fPhotoElCathodeCount(photoel_cathode_count), fPhotoElAnodeCount(photoel_anode_count) 
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadFiberPoint::~ERNeuRadFiberPoint()
{
}
// -------------------------------------------------------------------------
ClassImp(ERNeuRadFiberPoint)
