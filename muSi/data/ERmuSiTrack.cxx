// -------------------------------------------------------------------------
// -----                      ERmuSiTrack source file                  -----
// -------------------------------------------------------------------------

#include "ERmuSiTrack.h"

#include<iostream>

Int_t ERmuSiTrack::AddHit(Int_t station, ERmuSiHit hit){
	if (station > 2){
		std::cerr << "Wrong station number in ERmuSiTrack::AddHit"  << std::endl;
		return -1;
	}
	fHits[station] = hit;
	return 0;
}