// -------------------------------------------------------------------------
// -----                      ERNeuRadFiberPoint header file           -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadFiberPoint_H
#define ERNeuRadFiberPoint_H

#include "FairMultiLinkedData.h"

class ERNeuRadFiberPoint : public FairMultiLinkedData{
	private:
    Int_t fIndex;
		Int_t fSide; //0 - front , 1 - back
    Int_t fOnePE; //1 - one photo electron in fiber point, 0 - not
		Double_t fCathodeTime;
		Double_t fAnodeTime;
		Int_t fPhotonCount;
		Int_t fPhotoElCount;
		Double_t fAmplitude;
      Double_t fLYTime;
	public:
		/** Default constructor **/
		ERNeuRadFiberPoint();
		ERNeuRadFiberPoint(Int_t index, Int_t side, Double_t lyTime, Double_t cathode_time, Double_t anode_time, Int_t photon_count, Int_t photoel_count, 
							Double_t amplitude,Int_t onePE);

		/** Destructor **/
		~ERNeuRadFiberPoint();

    /** Accessors **/
    Int_t     Index()        {return fIndex;}
    Int_t     Side()         {return fSide;}
    Double_t   LYTime()       {return fLYTime;}
    Double_t  CathodeTime()  {return fCathodeTime;}
    Double_t  AnodeTime()    {return fAnodeTime;}
    Int_t     PhotonCount()  {return fPhotonCount;}
    Int_t     PhotoElCount() {return fPhotoElCount;}
    Double_t  Amplitude()    {return fAmplitude;}
    Int_t     OnePE()        {return fOnePE;}

	ClassDef(ERNeuRadFiberPoint,1)
};

#endif

