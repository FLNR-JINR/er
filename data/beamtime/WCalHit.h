/*
 * WCalHit.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef DATA_BEAMTIME_WCALHIT_H_
#define DATA_BEAMTIME_WCALHIT_H_

#include "TObject.h"

class WCalHit : public TObject {
public:
	WCalHit();
	WCalHit(UChar_t p_ch, Float_t p_adcCalib);
	virtual ~WCalHit();
	ClassDef(WCalHit,1);

	void Clear(Option_t *option="");

	WCalHit &operator=(const WCalHit &orig);

private:
    UChar_t fChannel;
//    UShort_t fRawAdcVal;
//    Int_t fAdcVal;           // (pedestal -raw adc val)
    Float_t fAdcCalibrated; // After calibration using LUTs
};

#endif /* DATA_BEAMTIME_WCALHIT_H_ */
