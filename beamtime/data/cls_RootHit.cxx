#include "cls_RootHit.h"

ClassImp(cls_RootHit)

// Default constructor should not be used
cls_RootHit::cls_RootHit() :
    fTimestamp(0),
    fChannel(0),
    fRawAdcVal(0),
    fAdcVal(0),
    fAdcCalibrated(0.)
{
}

cls_RootHit::cls_RootHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc, Float_t p_adcCalib) :
    fTimestamp(p_ts),
    fChannel(p_ch),
    fRawAdcVal(p_rawAdc),
    fAdcVal(p_adc),
    fAdcCalibrated(p_adcCalib)
{
}

cls_RootHit::cls_RootHit(cls_RootHit* p_sourceHit) :
    fTimestamp(p_sourceHit->fTimestamp),
    fChannel(p_sourceHit->fChannel),
    fRawAdcVal(p_sourceHit->fRawAdcVal),
    fAdcVal(p_sourceHit->fAdcVal),
    fAdcCalibrated(p_sourceHit->fAdcCalibrated)
{
}

cls_RootHit::~cls_RootHit()
{
    Clear();
}

void cls_RootHit::Clear(Option_t * /*option*/)
{
   TObject::Clear();
}

cls_RootHit &cls_RootHit::operator=(const cls_RootHit &orig)
{
   TObject::operator=(orig);

   fTimestamp = orig.fTimestamp;
   fChannel = orig.fChannel;
   fRawAdcVal = orig.fRawAdcVal;
   fAdcVal = orig.fAdcVal;
   fAdcCalibrated = orig.fAdcCalibrated;

   return *this;
}
