// -------------------------------------------------------------------------
// -----                  ERNeuRadPhotoElectron header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadPhotoElectron_H
#define ERNeuRadPhotoElectron_H

#include "FairMultiLinkedData.h"

class ERNeuRadPhotoElectron : public FairMultiLinkedData {

private:

	Int_t fIndex;
	Int_t fSide; //0 - front , 1 - back
	Double_t fCathodeTime;
	Double_t fAnodeTime;
	Int_t fPhotonCount;
	Double_t fAmplitude;
	Double_t fLYTime;

public:

	/** Default constructor **/
	ERNeuRadPhotoElectron();
	ERNeuRadPhotoElectron(Int_t index, Int_t side, Double_t lyTime, Double_t cathode_time, Double_t anode_time, Int_t photon_count, 
	Double_t amplitude);

	/** Destructor **/
	~ERNeuRadPhotoElectron();

	/** Accessors **/
	Int_t     Index()        {return fIndex;}
	Int_t     Side()         {return fSide;}
	Double_t  LYTime()       {return fLYTime;}
	Double_t  CathodeTime()  {return fCathodeTime;}
	Double_t  AnodeTime()    {return fAnodeTime;}
	Int_t     PhotonCount()  {return fPhotonCount;}
	Double_t  Amplitude()    {return fAmplitude;}

	void Print();

	ClassDef(ERNeuRadPhotoElectron,1)
};

#endif // ERNeuRadPhotoElectron_H
