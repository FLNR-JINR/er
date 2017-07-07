#include "ERRTelescopeCalEvent.h"

ERRTelescopeCalEvent::ERRTelescopeCalEvent(){

}

ERRTelescopeCalEvent::~ERRTelescopeCalEvent(){

}

void ERRTelescopeCalEvent::Reset(){
	mC11 = -1;
	mC12 = -1;
	for (int i =0; i< 32; i++){
		eC11[i] = 0; eC12[i] = 0;
		nC11[i] = 0; nC12[i] = 0;
	}
}

ClassImp(ERRTelescopeCalEvent);