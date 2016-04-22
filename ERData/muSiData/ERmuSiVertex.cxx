#include "ERmuSiVertex.h"

#include "FairLogger.h"


ERmuSiVertex::ERmuSiVertex():
	fTracksNb(0)
{

}

ERmuSiVertex::ERmuSiVertex(Float_t x, Float_t y, Float_t z):
	fTracksNb(0), fX(x), fY(y), fZ(z)
{

}

void ERmuSiVertex::AddTrack(Int_t id){
	Bool_t founded = kFALSE;
	for (Int_t iTrack = 0; iTrack < fTracksNb; iTrack++){
		if (fTracksID[iTrack] == id){
			founded = kTRUE;
		}
	}
	if (fTracksNb < 3){
		if (!founded)
			fTracksID[fTracksNb++] = id;
	}
	
	if (fTracksNb > 3){ 
		LOG(ERROR) << "Big track count in vertex!" << FairLogger::endl;
	}
}

Int_t ERmuSiVertex::Track(Int_t id) const{
	if (id >= fTracksNb){
		LOG(ERROR) << "There is no such number of tracks!" << FairLogger::endl;
	}
	return fTracksID[id];
}

