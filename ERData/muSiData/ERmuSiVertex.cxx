#include "ERmuSiVertex.h"

#include<iostream>


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
		std::cerr << "Big track count in vertex!" << std::endl;
	}
}

Int_t ERmuSiVertex::Track(Int_t id) const{
	if (id >= fTracksNb){
		std::cerr << "There is no such number of tracks!" << std::endl;
	}
	return fTracksID[id];
}

