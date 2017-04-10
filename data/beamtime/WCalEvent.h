/*
 * WCalEvent.h
 *
 *  Created on: Apr 6, 2017
 *      Author: vratik
 */

#ifndef DATA_BEAMTIME_WCALEVENT_H_
#define DATA_BEAMTIME_WCALEVENT_H_

#include "TNamed.h"
#include "TClonesArray.h"
#include "WCalHit.h"

class WCalEvent : public TNamed {
public:
	WCalEvent();
	virtual ~WCalEvent();
	ClassDef(WCalEvent,1);

    void Clear(Option_t *option ="");

//    cls_RootHit* AddHit(cls_RootHit* p_sourceHit);
    WCalHit* AddHit(UChar_t p_ch, Float_t p_adcCalib);

//    void SetID(ULong_t p_id) { fEventID = p_id; }
	void SetNumOfHits(UShort_t nhits) {fNumOfHits = nhits;};

private: // data members

    TClonesArray* fHits; //-> TClonesArray of WCalHit objects
    UShort_t fNumOfHits; //   Number of hits - actually the size of the 'fHits'

//    ULong_t fEventID;

};

#endif /* DATA_BEAMTIME_WCALEVENT_H_ */
