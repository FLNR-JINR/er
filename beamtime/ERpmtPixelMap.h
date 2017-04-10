/*
 * ERpmtPixelMap.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERPMTPIXELMAP_H_
#define BEAMTIME_ERPMTPIXELMAP_H_

#include "Rtypes.h"

class ERpmtPixelMap {
public:
	ERpmtPixelMap();
	virtual ~ERpmtPixelMap();

private:
	// FEB channels are from 0 (incl.) to 63 (incl.)
	// Pixel IDs are from 1 (incl.) to 64 (incl.)

	UInt_t fPixelFromFebCh[64];
	UInt_t fFebChFromPixel[64+1];   // 0-th element not used
};

#endif /* BEAMTIME_ERPMTPIXELMAP_H_ */
