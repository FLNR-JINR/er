//===================================================================
//== TLiEvent.h:	exp0211
//== Version: 2011-02-01
//===================================================================

#ifndef TLiEVENT_H
#define TLiEVENT_H

#include "TGo4EventElement.h"

class TLiEvent : public TGo4EventElement 
{
public:
	TLiEvent();
	TLiEvent(const char* name);
	virtual ~TLiEvent();

	/** Method called by the framework to clear the event element. */
	void Clear(Option_t *t="");
	unsigned short SQ11[16];
	unsigned short SQ12[32];
	unsigned short SQ13[16];
	unsigned short SQ21[16];
	unsigned short SQ22[32];
	unsigned short SQ23[16];
	unsigned short ANR[16];
	unsigned short ANS[16];
	unsigned short ANC[32];
	unsigned short TSQ11[16];
	unsigned short TSQ12[32];
	unsigned short TSQ21[16];
	unsigned short TSQ22[32];
	unsigned short TANR[16];
	unsigned short TANS[16];
	unsigned short unusedQDC[32];
	unsigned short unusedTDC[16];
	unsigned short SCR[16];
	unsigned short nx1, ny1, nx2, ny2; // wires
	unsigned short x1[32],y1[32],x2[32],y2[32]; // wires
	unsigned short adc1[16];
	unsigned short adc2[16];
	unsigned short adc3[16];
	unsigned short adc4[16];
	unsigned short NeuTDC[32];
	unsigned short Amp_TDC[32];
	unsigned int time;
		
	int qdcF3L;
	int qdcF3R;
	int qdcF4L;
	int qdcF4R;
	int qdcF4U;
	int qdcF4D;
	int qdcF5L;
	int qdcF5R;
	int qdcF5U;
	int qdcF5D;
	int tdcF3L;
	int tdcF3R;
	int tdcF4L;
	int tdcF4R;
	int tdcF4U;
	int tdcF4D;
	int tdcF5L;
	int tdcF5R;
	int tdcF5U;
	int tdcF5D;
	int nevent;
	int trigger;
	int subevents;
	int evsize;
int mainTRIGGER;
int beamTRIGGER;
int annTRIGGER;
ClassDef(TLiEvent,1)
};
#endif //TEVENT_H



