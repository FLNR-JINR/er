#include "cls_RootEvent.h"

#include <TClonesArray.h>

#include "cls_RootHit.h"

ClassImp(cls_RootEvent)

cls_RootEvent::cls_RootEvent() :
	fNumOfHits(0),
	fEventID(0)
{
	fHits = new TClonesArray("cls_RootHit", 5);
}

cls_RootEvent::~cls_RootEvent()
{
	Clear();
}

void cls_RootEvent::Clear(Option_t * /*option*/)
{
	fHits->Clear("C");
	fNumOfHits = 0;     // Hakuna Matata
}

/* https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#example-4-a-tree-with-an-event-class
 * Section 14.17
 */
cls_RootHit* cls_RootEvent::AddHit(cls_RootHit* p_sourceHit)
{
	TClonesArray &hits = *fHits;
	cls_RootHit *hit = new(hits[fNumOfHits++]) cls_RootHit(p_sourceHit);
	return hit;
}

cls_RootHit* cls_RootEvent::AddHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc, Float_t p_adcCalib)
{
	TClonesArray &hits = *fHits;
	cls_RootHit *hit = new(hits[fNumOfHits++]) cls_RootHit(p_ts, p_ch, p_rawAdc, p_adc, p_adcCalib);
	return hit;
}
