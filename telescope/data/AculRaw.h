////////////////////////////////////////////////////////////////
////														////
////	Class with structure of the TTree for immediate		////
////	translation of the ACCULINNA raw data format to		////
////	ROOT format. This class correspond to the DAQ		////
////	structure.											////
////														////
////////////////////////////////////////////////////////////////



#ifndef AculRaw_H
#define AculRaw_H
#include <TObject.h>

#define	BLOCKSNUMBER 24
#define ADDRESSNUMBER 16


class AculRaw : public TObject
{
public:

	Int_t	mwpcReg[4];							//information about MWPCx plane in the register format
	Int_t	C3[BLOCKSNUMBER][ADDRESSNUMBER];	//information from crate C3
	Int_t	trigger;

	AculRaw();
	virtual ~AculRaw();
	ClassDef(AculRaw, 1);

	//functions
	Int_t	Reset();		//reseting of the read values

};
#endif