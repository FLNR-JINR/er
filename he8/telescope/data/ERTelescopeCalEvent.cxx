#include "ERTelescopeCalEvent.h"

ERTelescopeCalEvent::ERTelescopeCalEvent(){

}

ERTelescopeCalEvent::~ERTelescopeCalEvent(){

}

void ERTelescopeCalEvent::Reset(){
	mC21 = -1;
	mC22 = -1;
	mC23 = -1;
	mC24 = -1;
	mC25 = -1;
	mC26 = -1;
	mCT21 = -1;
	mCT22 = -1;
	for (int i =0; i< 32; i++){
		eC21[i] = 0; nC21[i] = 0;
		eC22[i] = 0; nC22[i] = 0;
		eC23[i] = 0; nC23[i] = 0;
		eC24[i] = 0; nC24[i] = 0;
		eC25[i] = 0; nC25[i] = 0;
		eC26[i] = 0; nC26[i] = 0;
		nCT21[i] = 0;
		nCT22[i] = 0;
		tC21[i] = 0;
		tC22[i] = 0;
	}
}

ClassImp(ERTelescopeCalEvent);