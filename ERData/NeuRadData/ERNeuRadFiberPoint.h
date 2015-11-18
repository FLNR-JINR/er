// -------------------------------------------------------------------------
// -----                      ERNeuRadFiberPoint header file           -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadFiberPoint_H
#define ERNeuRadFiberPoint_H

#include "TObject.h"

class ERNeuRadFiberPoint : public TObject{
	private:
		Int_t fSide; //0 - front , 1 - back
		Double_t fCathodeTime;
		Double_t fAnodeTime;
		Int_t fPhotonCount;
		Int_t fPhotoElCount;
		Double_t fAmplitude;
	public:
		/** Default constructor **/
		ERNeuRadFiberPoint();
		ERNeuRadFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
							Int_t photon_count, Int_t photoel_count, 
							Double_t amplitude);

		/** Destructor **/
		~ERNeuRadFiberPoint();

    /** Accessors **/
    Int_t     Side()         {return fSide;}
    Double_t  CathodeTime()  {return fCathodeTime;}
    Double_t  AnodeTime()    {return fAnodeTime;}
    Int_t     PhotonCount()  {return fPhotonCount;}
    Int_t     PhotoElCount() {return fPhotoElCount;}
    Double_t  Amplitude()    {return fAmplitude;}

	ClassDef(ERNeuRadFiberPoint,1)
};

#endif

