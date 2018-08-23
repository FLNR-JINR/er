#include "ERUnpack.h"

ERUnpack::ERUnpack(TString name):
FairUnpack(0,0,0,0,0),
fName(name),
fInited(kFALSE),
fUnpacked(kFALSE)
{

}

ERUnpack::~ERUnpack(){

}

ClassImp(ERUnpack)