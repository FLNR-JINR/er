#include "ERmuSiVertex.h"

#include "FairLogger.h"


ERmuSiVertex::ERmuSiVertex():
	fTracksNb(0)
{

}


void ERmuSiVertex::AddTrack(Int_t id){
	if (fTracksNb == 3){ 
		LOG(ERROR) << "Big track count in vertex!" << FairLogger::endl;
	}
	else{
		fTracksID[fTracksNb++] = id;
	}
}

Int_t ERmuSiVertex::Track(Int_t id){
	if (id >= fTracksNb){
		LOG(ERROR) << "There is no such number of tracks!" << FairLogger::endl;
	}
	return fTracksID[id];
}

