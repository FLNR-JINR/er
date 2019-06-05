#include "AculRaw.h"

ClassImp(AculRaw);

AculRaw::AculRaw()		// it is possible to replace whole text in the constructor by use of the Reset() function
{
	trigger = 0;

	for (Int_t i = 0; i < 4; i++) {
		mwpcReg[i] = 0;
	}

	for (Int_t i = 0; i < BLOCKSNUMBER; i++) {
		for (Int_t j = 0; j < ADDRESSNUMBER; j++) {
			C3[i][j] = 0;
		}
	}

};

AculRaw::~AculRaw()
{
}

Int_t AculRaw::Reset()
{
	trigger = 0;

	for (Int_t i = 0; i < 4; i++) {
		mwpcReg[i] = 0;
	}

	for (Int_t i = 0; i < BLOCKSNUMBER; i++) {
		for (Int_t j = 0; j < ADDRESSNUMBER; j++) {
			C3[i][j] = 0;
		}
	}

	return 0;
}
