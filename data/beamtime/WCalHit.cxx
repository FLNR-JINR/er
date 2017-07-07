/*
 * WCalHit.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#include "WCalHit.h"

WCalHit::WCalHit() :
//fTimestamp(0),
fChannel(0),
//fRawAdcVal(0),
//fAdcVal(0),
fAdcCal(0.)
{
	//todo
//	fXpixel = -1;
//	fYpixel = -1;

	for (UInt_t i=0; i<8; i++) {
//		fPixelFromFebCh[i*8+0] = i*8+1;
//		fPixelFromFebCh[i*8+1] = i*8+5;
//		fPixelFromFebCh[i*8+2] = i*8+3;
//		fPixelFromFebCh[i*8+3] = i*8+7;
//		fPixelFromFebCh[i*8+4] = i*8+2;
//		fPixelFromFebCh[i*8+5] = i*8+6;
//		fPixelFromFebCh[i*8+6] = i*8+4;
//		fPixelFromFebCh[i*8+7] = i*8+8;

		fPixelFromFebCh[i*8+0] = i*8;
		fPixelFromFebCh[i*8+1] = i*8+4;
		fPixelFromFebCh[i*8+2] = i*8+2;
		fPixelFromFebCh[i*8+3] = i*8+6;
		fPixelFromFebCh[i*8+4] = i*8+1;
		fPixelFromFebCh[i*8+5] = i*8+5;
		fPixelFromFebCh[i*8+6] = i*8+3;
		fPixelFromFebCh[i*8+7] = i*8+7;
	}

}

WCalHit::WCalHit(UChar_t p_ch, Int_t p_adc, Float_t p_adcNonLin, Float_t p_adcNonLinCorr, Float_t p_adcCalib) :
//		fTimestamp(p_ts),
		fChannel(p_ch),
		fAdcVal(p_adc),
		fAdcNonLin(p_adcNonLin),
		fAdcNonLinCorr(p_adcNonLinCorr),
		fAdcCal(p_adcCalib)
{

	for (UInt_t i=0; i<8; i++) {
		fPixelFromFebCh[i*8+0] = i*8;
		fPixelFromFebCh[i*8+1] = i*8+4;
		fPixelFromFebCh[i*8+2] = i*8+2;
		fPixelFromFebCh[i*8+3] = i*8+6;
		fPixelFromFebCh[i*8+4] = i*8+1;
		fPixelFromFebCh[i*8+5] = i*8+5;
		fPixelFromFebCh[i*8+6] = i*8+3;
		fPixelFromFebCh[i*8+7] = i*8+7;
	}

	SetXY();

//	std::cout << "!!!!\t" << fAdcCalibrated << std::endl;
}

WCalHit::~WCalHit() {
	// TODO Auto-generated destructor stub
	Clear();
}

void WCalHit::Clear(Option_t * /*option*/)
{
   TObject::Clear();
}

WCalHit &WCalHit::operator=(const WCalHit &orig)
{
   TObject::operator=(orig);

//   fTimestamp = orig.fTimestamp;
   fChannel = orig.fChannel;
//   fRawAdcVal = orig.fRawAdcVal;
//   fAdcVal = orig.fAdcVal;
   fAdcCal = orig.fAdcCal;

   return *this;
}

void WCalHit::SetXY() {

	if (fChannel >= 0 && fChannel < 64) {
		UInt_t v_pixelID = fPixelFromFebCh[(Int_t)fChannel];
//		std::cout << (Int_t)fChannel << "\t" << v_pixelID << std::endl;

//		fXpixel = (fChannel)%8;
//		fYpixel = -(fChannel)/8;

		fXpixel = (v_pixelID)%8;
		fYpixel = -((v_pixelID)/8);

//		std::cout << fXpixel << "\t" << fYpixel << std::endl;

//		*o_row = (v_pixelID-1)/8;
//		*o_col = (v_pixelID-1)%8;
//		return 1; // OK

	} else {
//		return 0; // FAIL
	}

}
