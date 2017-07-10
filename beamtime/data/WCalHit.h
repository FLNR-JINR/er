/*
 * WCalHit.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef DATA_BEAMTIME_WCALHIT_H_
#define DATA_BEAMTIME_WCALHIT_H_

#include "TObject.h"

//todo: delete
#include <iostream>

class WCalHit : public TObject {
public:
	WCalHit();
	WCalHit(UChar_t p_ch, Int_t p_adc, Float_t p_adcNonLin, Float_t p_adcNonLinCorr, Float_t p_adcCalib);
	virtual ~WCalHit();
	ClassDef(WCalHit,1);

	void Clear(Option_t *option="");

	WCalHit &operator=(const WCalHit &orig);

private:
	void SetXY();

private:
    UChar_t fChannel;
//    UShort_t fRawAdcVal;
    Int_t fAdcVal;			// (pedestal -raw adc val)
    Float_t fAdcNonLin;		//corrected for non-linearity
    Float_t fAdcNonLinCorr;	//corrected for non-linearity and pedestal shift
    Float_t fAdcCal;	// After calibration using LUTs

    Int_t fXpixel;			//number of x row (vertical)
    Int_t fYpixel;			//number of x row (or column) (horizontal)

    UInt_t fPixelFromFebCh[64]; //!
};

#endif /* DATA_BEAMTIME_WCALHIT_H_ */
