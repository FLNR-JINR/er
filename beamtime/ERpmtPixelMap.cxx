/*
 * ERpmtPixelMap.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#include "ERpmtPixelMap.h"

ERpmtPixelMap::ERpmtPixelMap() {

	// Default map
	for (UInt_t i=0; i<8; i++) {
		fPixelFromFebCh[i*8+0] = i*8+1;
		fPixelFromFebCh[i*8+1] = i*8+5;
		fPixelFromFebCh[i*8+2] = i*8+3;
		fPixelFromFebCh[i*8+3] = i*8+7;
		fPixelFromFebCh[i*8+4] = i*8+2;
		fPixelFromFebCh[i*8+5] = i*8+6;
		fPixelFromFebCh[i*8+6] = i*8+4;
		fPixelFromFebCh[i*8+7] = i*8+8;

		fFebChFromPixel[i*8+1] = i*8+0;
		fFebChFromPixel[i*8+5] = i*8+1;
		fFebChFromPixel[i*8+3] = i*8+2;
		fFebChFromPixel[i*8+7] = i*8+3;
		fFebChFromPixel[i*8+2] = i*8+4;
		fFebChFromPixel[i*8+6] = i*8+5;
		fFebChFromPixel[i*8+4] = i*8+6;
		fFebChFromPixel[i*8+8] = i*8+7;
	}

}

ERpmtPixelMap::~ERpmtPixelMap() {
	// TODO Auto-generated destructor stub
}

