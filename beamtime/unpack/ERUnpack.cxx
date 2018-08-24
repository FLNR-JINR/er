#include "ERUnpack.h"

ERUnpack::ERUnpack(TString detName):
FairUnpack(0,0,0,0,0),
fDetName(detName),
fInited(kFALSE),
fUnpacked(kFALSE)
{

}

ERUnpack::~ERUnpack(){

}

void ERUnpack::Reset(){
	fUnpacked = kFALSE;
	for (auto itCol : fDigiCollections)
		itCol.second->Clear();
}

ClassImp(ERUnpack)