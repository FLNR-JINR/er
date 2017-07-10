#ifndef CLS_ROOTEVENT_H
#define CLS_ROOTEVENT_H

#include "TNamed.h"
#include <TClonesArray.h>

class cls_RootHit;

class cls_RootEvent : public TNamed
{

public: // methods

	cls_RootEvent();
	virtual ~cls_RootEvent();

	void Clear(Option_t *option ="");

	cls_RootHit* AddHit(cls_RootHit* p_sourceHit);
	cls_RootHit* AddHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc, Float_t p_adcCalib);

	void SetID(ULong_t p_id) { fEventID = p_id; }

	UShort_t GetNumOfHits() {return fNumOfHits;}
//	Int_t GetAdcVal(UChar_t channel);
	TClonesArray* GetHits() {return fHits;}

private: // data members

    TClonesArray* fHits; //-> TClonesArray of cls_RootHit objects
    UShort_t fNumOfHits; //   Number of hits - actually the size of the 'fHits'

    ULong_t fEventID;

public:
    ClassDef(cls_RootEvent,1)
};

#endif // CLS_ROOTEVENT_H
