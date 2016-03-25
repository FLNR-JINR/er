// -------------------------------------------------------------------------
// -----                      ERmuSiTrack source file                  -----
// -------------------------------------------------------------------------

#include "ERmuSiTrack.h"

#include "FairLogger.h"

Int_t ERmuSiTrack::AddHit(Int_t station, ERmuSiHit hit){
	if (station > 2){
		LOG(ERROR) << "Wrong station number in ERmuSiTrack::AddHit"  << FairLogger::endl;
		return -1;
	}
	fHits[station] = hit;
	return 0;
}